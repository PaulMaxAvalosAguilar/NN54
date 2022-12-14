#include "FreeRTOS.h"
#include "portable.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/exti.h>
#include "lcd.h"
#include "ring.h"
#include "main.h"
#include "bluetooth.h"
#include "CUSTOM/MATH/math.h"
#include <string.h>

//-------------------------PROTOCOL--------------------------
//*******************************************************************

//Queue handles----------------------------------
QueueHandle_t humanInterfaceDisplayRequestQueue;
QueueHandle_t protocolMessagesTXRequestQueue;

//Task Handles-----------------------------------
TaskHandle_t encoderTaskHandle = NULL;
TaskHandle_t batteryFreeTaskHandle = NULL;
TaskHandle_t batteryWaitTaskHandle = NULL;

//Ring buffer handles----------------------------
ring_t encoder_ring;

//Buffers-----------------------------
encoderValues_t encoder_buffer[ENCODER_BUFFER_SIZE];

//encoderTask variables--------------------------
uint8_t (*goingDesiredCountDir[2])(uint32_t, uint32_t) = {descendente, ascendente};
uint8_t (*hasTraveledMinDist[2])(uint32_t, uint32_t) = {minDistTraveledDes,
						      minDistTraveledAs};
uint8_t (*hasReturnedToInitial[2])(uint32_t, uint32_t) =
  {returnedToInitialDes,returnedToInitialAs};
uint8_t (*newMaxRomDetected[2])(uint32_t,uint32_t) = {maxRomDes, maxRomAs};

//ISR variables----------------------------------
volatile encoderValues_t encInterruptValues;
volatile uint16_t encoderTimerCounter = 0;
volatile uint32_t capturedTime = 0;
volatile uint32_t overflowCounter=0 ;

//Helper functions--------------------------------
void sendToHumanInterfaceDisplayRequestQueue(humanInterfaceDisplayRequest_Codes
					     humanInterfaceDisplayRequest_Code,
					     uint32_t firstParameter){
  humanInterfaceDisplayRequest_Message dataToSend;
  dataToSend.humanInterfaceDisplayRequest_Code = humanInterfaceDisplayRequest_Code;
  dataToSend.firstParameter = firstParameter;

  xQueueSendToBack(humanInterfaceDisplayRequestQueue,&dataToSend,0);
}

void sendToProtocolMessagesTXRequestQueue(protocolMessagesTXRequest_Codes
					  protocolMessageTXRequest_Code,
					  uint16_t firstParameter,
					  uint16_t secondParameter,
					  uint16_t thirdParameter){
  protocolMessagesTXRequest_Message dataToSend;
  dataToSend.protocolMessageTXRequest_Code = protocolMessageTXRequest_Code;
  dataToSend.firstParameter = firstParameter;
  dataToSend.secondParameter = secondParameter;
  dataToSend.thirdParameter = thirdParameter;
  xQueueSendToBack(protocolMessagesTXRequestQueue,&dataToSend,0);

}

void createTask(TaskFunction_t pvTaskCode,
			      const char *const pcName,
			      configSTACK_DEPTH_TYPE usStackDepth,
			      void *pvParameters,
			      UBaseType_t uxPriority,
			      TaskHandle_t *taskHandle){
  
  taskENTER_CRITICAL();

  //Two instances of the same task are not allowed
  if(*taskHandle != NULL){
    vTaskDelete(*taskHandle);
    *taskHandle = NULL;
  }

  xTaskCreate(pvTaskCode, pcName, usStackDepth,
	      pvParameters, uxPriority, taskHandle);

  taskEXIT_CRITICAL();

}

void deleteTask(TaskHandle_t *pxTask){
  
  taskENTER_CRITICAL();

  if(*pxTask != NULL){
    vTaskDelete(*pxTask); 
    *pxTask = NULL;
  }

  taskEXIT_CRITICAL();
  
}

void encodeTwoBytes(char *twoByteBuffer, uint32_t numberToEncode){
  uint8_t lowPart = 0;
  uint8_t highPart = 0;
  
  lowPart = ((numberToEncode & 0x7F) << 1) | 1;
  highPart = (numberToEncode >>6) | 1;
  twoByteBuffer[0] = highPart;
  twoByteBuffer[1] = lowPart;
  twoByteBuffer[2] = 0;
}

uint16_t decodeTwoBytes(uint8_t msb, uint8_t lsb){
   return (lsb>>1) | ((msb & 0xFE)<<6);
}

void encodeOneByte(char *oneByteBuffer, unsigned int numberToEncode){
  uint8_t lowPart = 0;
  lowPart = (numberToEncode<<1) | 1;
  oneByteBuffer[0] = (uint8_t)lowPart;
  oneByteBuffer[1] = 0;
}

uint8_t decodeOneByte(uint8_t byte){
    return byte>>1;
}

void initializeTimers(void){

  rcc_periph_clock_enable(RCC_TIM1);    // TIM1
  rcc_periph_clock_enable(RCC_TIM2);    // TIM2

  TIM1_CNT = 0;
  overflowCounter = 0;
  TIM2_CNT = ENCODERINITIAL_VALUE;
}

void stopTimers(void){

  rcc_periph_clock_disable(RCC_TIM1);    // TIM1
  rcc_periph_clock_disable(RCC_TIM2);    // TIM2  
}

void turnOnEncoderSensors(void){
    gpio_clear(GPIOA, GPIO4);
}

void turnOffEncoderSensors(void){
    gpio_set(GPIOA, GPIO4);
}

uint32_t readBattery(void){

  uint32_t voltageSupply;
  uint32_t adcData;
  uint8_t channel[1];

  adc_set_sample_time(ADC1,ADC_CHANNEL_VREF,ADC_SMPR_SMP_239DOT5CYC);
  channel[0] = ADC_CHANNEL_VREF;
  adc_set_regular_sequence(ADC1,1,channel);
  adc_start_conversion_direct(ADC1);
  while ( !adc_eoc(ADC1) );
  voltageSupply = 4914000/adc_read_regular(ADC1);

  adc_set_sample_time(ADC1,ADC_CHANNEL1,ADC_SMPR_SMP_239DOT5CYC);
  channel[0] = ADC_CHANNEL1;
  adc_set_regular_sequence(ADC1,1,channel);
  adc_start_conversion_direct(ADC1);
  while ( !adc_eoc(ADC1) );
  adcData = (adc_read_regular(ADC1) * voltageSupply / 4095) *2;

  return adcData;
}

uint32_t isEncoderCharging(void){
  uint32_t returnedValue;
  
  if( (GPIOB_IDR & (1 <<12)) != 0){
    returnedValue = 1;
  }else{
    returnedValue = 0;
  }
  
  return returnedValue;
}

//EncoderHelper functions-------------------------
uint8_t descendente(uint32_t a, uint32_t b){
  return a<b;
}

uint8_t ascendente(uint32_t a, uint32_t b){
  return a>b;
}

uint8_t minDistTraveledDes(uint32_t counter, uint32_t minDistToTravel){
  return (counter <= ENCODERINITIAL_VALUE - minDistToTravel);
}

uint8_t minDistTraveledAs(uint32_t counter, uint32_t minDistToTravel){
  return (counter >= ENCODERINITIAL_VALUE + minDistToTravel);
}

uint8_t returnedToInitialDes(uint32_t counter, uint32_t minDistToTravel){
  return (counter > ENCODERINITIAL_VALUE - minDistToTravel);
}

uint8_t returnedToInitialAs(uint32_t counter, uint32_t minDistToTravel){
  return (counter < ENCODERINITIAL_VALUE + minDistToTravel);
}

uint8_t maxRomDes(uint32_t counter, uint32_t lastMaxDist){
  return (counter < lastMaxDist);
}

uint8_t maxRomAs(uint32_t counter, uint32_t lastMaxDist){
  return (counter > lastMaxDist);
}

//Processes---------------------------------------
__attribute__((optimize("O0"))) void encoderTask(void *args __attribute__((unused))){

  encoderValues_t receiveEncValues;  
  encoderTaskParamTypes_t taskParam;
  taskParam = *(encoderTaskParamTypes_t *)args;

  //Direction variables
  uint32_t desiredDirFollowed = 0;

  //Distance calculation variables
  int32_t lastPosition = ENCODERINITIAL_VALUE;
  uint32_t elapsedPosition = 0;
  uint32_t elapsedDistance = 0;

  //Time calculation variables
  uint32_t lastTime = 0;
  uint32_t elapsedTime = 0;

  //Velocity Calulcation Variables
  uint32_t currentVelocity = 0;
  uint32_t lastVelocity = 0;
  
  uint32_t meanPropVelCount = 0;
  uint32_t meanPropVel = 0;
  uint32_t peakVel = 0;

  //ROM variables
  uint16_t maxROM = ENCODERINITIAL_VALUE;

  //Repetition algorithm variables
  uint32_t canCountRep = 0;
  uint32_t  minDistTraveled= 0;

  for(;;){

    while(ring_buffer_get(&encoder_ring, &receiveEncValues) != -1){

      //VELOCITY ALGORITHM-------------------------------------------------------------------
      if( (*goingDesiredCountDir[taskParam.desiredCounterDirection])(receiveEncValues.encoderCounter,
					   lastPosition) ){
	desiredDirFollowed = 1;
	elapsedPosition = custom_abs((int32_t)receiveEncValues.encoderCounter - lastPosition);	
      }else{
	desiredDirFollowed = 0;
      }

      lastPosition = receiveEncValues.encoderCounter;
      elapsedTime = receiveEncValues.inputCapture - lastTime;
      lastTime = receiveEncValues.inputCapture;
      
      if(desiredDirFollowed){
	elapsedDistance = ENCODERSTEPDISTANCE_MICROS * elapsedPosition;
	currentVelocity = (elapsedDistance * 100) / elapsedTime;

	//Mean Propulsive Velocity Calculation
	if(currentVelocity > lastVelocity){
	  meanPropVelCount++;
	  meanPropVel += currentVelocity;
	}
	lastVelocity = currentVelocity;
	
	//Peak Velocity Calculation
	if(currentVelocity > peakVel) peakVel = currentVelocity;
      }
      //VELOCITY ALGORITHM-----------------------------------------------------------------

      //REPETITION ALGORITHM---------------------------------------------------------------

      if( (*newMaxRomDetected[taskParam.desiredRepDirection])(receiveEncValues.encoderCounter,
								 maxROM) ){
	maxROM = receiveEncValues.encoderCounter;
      }


      if((*hasTraveledMinDist[taskParam.desiredRepDirection])
	(receiveEncValues.encoderCounter, taskParam.minDistToTravel)){
	minDistTraveled = 1;
      }


      if(minDistTraveled && (*hasReturnedToInitial[taskParam.desiredRepDirection])
	 (receiveEncValues.encoderCounter, taskParam.minDistToTravel)){
	canCountRep = 1;
      }
      
      if(canCountRep){

	sendToProtocolMessagesTXRequestQueue(protocolMessagesTXRequestCode_encoderData,
			  (custom_abs(maxROM-ENCODERINITIAL_VALUE)* ENCODERSTEPDISTANCE_MICROS)/1000,
			  meanPropVel/meanPropVelCount,
			  peakVel);
	
	//Velocity Calulcation Variables reset
	lastVelocity = 0;
  
	meanPropVelCount = 0;
	meanPropVel = 0;
	peakVel = 0;

	//ROM variables reset
	maxROM = ENCODERINITIAL_VALUE;

	//Repetition algorithm variables
	canCountRep = 0;
	minDistTraveled= 0;
      }
      //REPETITION ALGORITHM---------------------------------------------------------------
    }
    
    vTaskDelay(pdMS_TO_TICKS(ENCODER_TASK_DELAY_MS));    
  }
}

void batteryFreeTask(void *args __attribute__((unused))){

  uint32_t adcData = 0;
  
  for(;;){
    
    adcData = readBattery();

    sendToHumanInterfaceDisplayRequestQueue(humanInterfaceDisplayRequestCode_batteryLevel
					    ,adcData);

    vTaskDelay(pdMS_TO_TICKS(BATTERY_FREE_TASK_DELAY_MS));
  }
}

void batteryWaitTask(void *args __attribute__((unused))){
  uint32_t adcData = 0;
  
  for(;;){

    ulTaskNotifyTake(pdTRUE,portMAX_DELAY);//Should go first
    
    adcData = readBattery();

    sendToProtocolMessagesTXRequestQueue(protocolMessagesTXRequestCode_BatteryLevel,
				 (uint16_t)adcData, 0,0);
    sendToHumanInterfaceDisplayRequestQueue(humanInterfaceDisplayRequestCode_batteryLevel
					    , adcData);
  }  
}

void humanInterfaceDisplayRequestTask(void *args __attribute__((unused))){
  humanInterfaceDisplayRequest_Message receivedData;
  char buffer[22];

  uint32_t lastVoltage = 0;
  
  lcd_init();
  
  for(;;){
    
    xQueueReceive(humanInterfaceDisplayRequestQueue,
		  &receivedData, portMAX_DELAY);
    
    if(receivedData.humanInterfaceDisplayRequest_Code ==
       humanInterfaceDisplayRequestCode_startup){
      lcdPutsBlinkFree("     SDT ENCODER",0);
      
    }else if(receivedData.humanInterfaceDisplayRequest_Code == 
	     humanInterfaceDisplayRequestCode_connectionStatus){
      lcdPutsBlinkFree(receivedData.firstParameter?
		       "Connected":"         ",
		       2);
      
    }else if(receivedData.humanInterfaceDisplayRequest_Code ==
	     humanInterfaceDisplayRequestCode_batteryLevel){

      if(isEncoderCharging()){
	sprintf(buffer,"   ** Batt: %u", receivedData.firstParameter);
	lcdPutsBlinkFree(buffer,7);
      }else{
	sprintf(buffer,"      Batt: %u", receivedData.firstParameter);
	lcdPutsBlinkFree(buffer,7);
      }
      lastVoltage = receivedData.firstParameter;
      
    }else if(receivedData.humanInterfaceDisplayRequest_Code ==
	     humanInterfaceDisplayRequestCode_chargingStatus){
      sprintf(buffer, receivedData.firstParameter?
	      "   ** Batt: %lu":
	      "      Batt: %lu", lastVoltage);
      lcdPutsBlinkFree(buffer, 7);
      
    }
  }
}
//*******************************************************************

//-----------------PROTOCOL IMPLEMENTATION DEPENDENT-----------------
//*******************************************************************

//Handles-------------
QueueSetHandle_t protocolMessagesTXRXQueueSet;
SemaphoreHandle_t protocolMessagesRXSemaphore;

//Implementation dependent proceses--------------
void  __attribute__((optimize("O0"))) protocolMessagesTXRXTask(void *args __attribute__((unused))) {

  protocolMessagesTXRequest_Message receivedData;
  QueueSetMemberHandle_t xHandle;
  char twoByteBuffers[3][3];
  char oneByteBuffer[2];


  sendToHumanInterfaceDisplayRequestQueue(humanInterfaceDisplayRequestCode_startup,0);
  
  
  for (;;) {
    xHandle = xQueueSelectFromSet(protocolMessagesTXRXQueueSet,portMAX_DELAY);//Should go first

    if( xHandle == ( QueueSetMemberHandle_t ) protocolMessagesTXRequestQueue){
      xQueueReceive(protocolMessagesTXRequestQueue, &receivedData,0);//Should go first


      if(receivedData.protocolMessageTXRequest_Code ==
	 protocolMessagesTXRequestCode_encoderData){
	
	encodeOneByte(oneByteBuffer, forEncoderControllerPCMCode_encoderData);
	encodeTwoBytes(twoByteBuffers[0],receivedData.firstParameter);
	encodeTwoBytes(twoByteBuffers[1],receivedData.secondParameter);
	encodeTwoBytes(twoByteBuffers[2],receivedData.thirdParameter);
	
	writeEncoderData(oneByteBuffer[0],
			   twoByteBuffers[0],
			   twoByteBuffers[1],
			   twoByteBuffers[2]);
	

      }else if(receivedData.protocolMessageTXRequest_Code ==
	       protocolMessagesTXRequestCode_start){
	
	encodeOneByte(oneByteBuffer, forEncoderControllerPCMCode_encoderStart);
	
	writeEncoderStartValue(oneByteBuffer[0]);

      }else if(receivedData.protocolMessageTXRequest_Code ==
	       protocolMessagesTXRequestCode_BatteryLevel){

	encodeOneByte(oneByteBuffer,
		      forEncoderControllerPCMCode_encoderBatteryLevel);
	encodeTwoBytes(twoByteBuffers[0],receivedData.firstParameter);
	
	writeBatteryLevel(oneByteBuffer[0],
			  twoByteBuffers[0]);


      }else if(receivedData.protocolMessageTXRequest_Code ==
	       protocolMessagesTXRequestCode_Stop){
	encodeOneByte(oneByteBuffer, forEncoderControllerPCMCode_encoderStop);	
	writeEncoderStop(oneByteBuffer[0]
			 );
	
      }else if(receivedData.protocolMessageTXRequest_Code ==
	       protocolMessagesTXRequestCode_chargingStatus){

	encodeOneByte(oneByteBuffer,
		      forEncoderControllerPCMCode_encoderChargingStatus);
	encodeTwoBytes(twoByteBuffers[0], receivedData.firstParameter);
	writeEncoderChargingStatus(oneByteBuffer[0],
				twoByteBuffers[0]);

      }
      
    }else if ( xHandle == (QueueSetMemberHandle_t ) protocolMessagesRXSemaphore){
      xSemaphoreTake(protocolMessagesRXSemaphore,0);//Should go first

      while(receiveBuffer[receiveBufferPos]){
	getLine();
	genericParsing(parseBuffer);
      }
    }

  }
}

//Interrupt handlers-----------------------------

void tim1_cc_isr(){

  if(TIM1_SR & TIM_SR_CC1IF){
    //Rising Edge B
    capturedTime = (overflowCounter << 16) | TIM1_CCR1;
  }else if(TIM1_SR & TIM_SR_CC2IF){
    //Falling Edge B
    capturedTime = (overflowCounter << 16) | TIM1_CCR2;
  }else if(TIM1_SR & TIM_SR_CC3IF){
    //Rising Edge A
    capturedTime = (overflowCounter << 16) | TIM1_CCR3;
  }else if(TIM1_SR & TIM_SR_CC4IF){
    //Falling Edge A
    capturedTime = (overflowCounter << 16) | TIM1_CCR4;
  }

  encInterruptValues.inputCapture = capturedTime;
  
  encoderTimerCounter = (uint16_t)TIM2_CNT;
  encInterruptValues.encoderCounter = encoderTimerCounter;
  
  ring_buffer_put(&encoder_ring, (encoderValues_t*)&encInterruptValues);

}

void exti15_10_isr(){
  
  exti_reset_request(EXTI12);//Clear flag
  
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint32_t chargingStatus = 0;
  humanInterfaceDisplayRequest_Message hIDRPdataToSend;
  protocolMessagesTXRequest_Message pMTRdataToSend;

  hIDRPdataToSend.humanInterfaceDisplayRequest_Code =
    humanInterfaceDisplayRequestCode_chargingStatus;
  pMTRdataToSend.protocolMessageTXRequest_Code =
    protocolMessagesTXRequestCode_chargingStatus;

  chargingStatus = isEncoderCharging();  
  hIDRPdataToSend.firstParameter = chargingStatus;
  pMTRdataToSend.firstParameter = chargingStatus;

  xQueueSendToBackFromISR(humanInterfaceDisplayRequestQueue,
			  &hIDRPdataToSend,&xHigherPriorityTaskWoken);
  xQueueSendToBackFromISR(protocolMessagesTXRequestQueue,
			  &pMTRdataToSend,&xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

//*******************************************************************


//---------------------------CUSTOM----------------------------------
//*******************************************************************

//Custom Buffers---------------------------------
char receiveBuffer[UART_RX_BUFFER_SIZE] = {0};
char parseBuffer[PARSE_BUFFER_SIZE] = {0};

//Custom Buffers positions-----------------------
uint32_t receiveBufferPos = 0;

//Custom helper functions------------------------
void printStringUART(const char myString[]) {

  dma_disable_channel(DMA1, DMA_CHANNEL4);
  dma_set_number_of_data(DMA1, DMA_CHANNEL4, strlen(myString));
  dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)myString);
  dma_enable_channel(DMA1, DMA_CHANNEL4);

  while(!dma_get_interrupt_flag(DMA1, DMA_CHANNEL4, DMA_TCIF));
  dma_clear_interrupt_flags(DMA1, DMA_CHANNEL4, DMA_TCIF); 

}

void cleanAdvanceBuffer(char *buffer, uint32_t *bufferPosition, uint32_t bufferLength){
  buffer[*bufferPosition] = 0;
  *bufferPosition = (*bufferPosition + 1) % bufferLength;
}

void  __attribute__((optimize("O0"))) getLine(){
  int parseBufferPos = 0;

  while(1){
    while(!receiveBuffer[receiveBufferPos]);
    if(receiveBuffer[receiveBufferPos] != '\n'){
      parseBuffer[parseBufferPos++] = receiveBuffer[receiveBufferPos];
      cleanAdvanceBuffer(receiveBuffer, &receiveBufferPos, UART_RX_BUFFER_SIZE);
    }else{
      break;
    }
  }
	
  parseBuffer[parseBufferPos++] = 0;
  cleanAdvanceBuffer(receiveBuffer, &receiveBufferPos, UART_RX_BUFFER_SIZE);
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime){

  //Formula used to determine ARR value:
  // #ticksMissing * (1 second/#Ticks) * (1 timer_counter_Unit/time_in_seconds)

  //Formula used to determine elapsed ticks
  // timCounter * (time_in_seconds/1 time_counter_Unit) * (#ticks/1second)

  eSleepModeStatus eSleepStatus;

  portNVIC_SYSTICK_CTRL_REG &= ~portNVIC_SYSTICK_ENABLE_BIT;  //Stop the SysTick
  //Disable interrupts
  __asm volatile( "cpsid i" ::: "memory" );
  __asm volatile( "dsb" );
  __asm volatile( "isb" );

  eSleepStatus = eTaskConfirmSleepModeStatus();

  if( eSleepStatus == eAbortSleep ){

    portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;//Restart systick
    __asm volatile( "cpsie i" ::: "memory" );//reenable interrupts
    
  }else{

    rcc_periph_clock_enable(RCC_TIM3);//Enable TIM3 clock
    lcdPutsBlinkFree("S",4);    
      
    if(eSleepStatus == eNoTasksWaitingTimeout){
      
      TIM3_ARR = 65535;
      TIM3_CR1 |= TIM_CR1_CEN;
      
      //Wait for interrupt
      __asm volatile( "dsb" ::: "memory" );
      __asm volatile( "wfi" );
      __asm volatile( "isb" );
    }else{

      TIM3_ARR = (xExpectedIdleTime * (10000 / configTICK_RATE_HZ) )/ 9;
      TIM3_CR1 |= TIM_CR1_CEN;
      
      //Wait for interrupt
      __asm volatile( "dsb" ::: "memory" );
      __asm volatile( "wfi" );
      __asm volatile( "isb" );
    }

    TIM3_CR1 &= ~TIM_CR1_CEN;


    ((TIM3_SR & TIM_SR_UIF) && (TIM3_CNT == 0))?
      vTaskStepTick(xExpectedIdleTime):
      vTaskStepTick((TIM3_CNT * 9 * configTICK_RATE_HZ)/10000);

    TIM3_CNT = 0;

    lcdPutsBlinkFree("",4);    

    //Reenable interrupts
    __asm volatile( "cpsie i" ::: "memory" );
    __asm volatile( "dsb" );
    __asm volatile( "isb" );

    rcc_periph_clock_disable(RCC_TIM3);//Enable TIM3 clock
    portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;//Restart systick


  }  
}

//Custom interrupt handlers----------------------

void tim1_up_isr(){
  TIM1_SR &= ~TIM_SR_UIF;//Should go first
  overflowCounter++;  
}

void usart1_isr(void){
  if (((USART_CR1(USART1) & USART_CR1_IDLEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_IDLE) != 0)) {
    usart_recv(USART1);//Interrupt clears at reading rx register


    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(protocolMessagesRXSemaphore,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
  }
}

void tim3_isr(void){
  if (timer_get_flag(TIM3, TIM_SR_UIF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM3, TIM_SR_UIF);
  }
}

//*******************************************************************


int main(void)
{

  //---------------------INCREASE CPU SPEED-------------------
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"

  //---------------------CONFIGURE GPIO-----------------------
  rcc_periph_clock_enable(RCC_GPIOA);   // TIM1 TIM2 ADC
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM2 UART
  rcc_periph_clock_enable(RCC_GPIOC);	// I2C TIM2
  rcc_periph_clock_enable(RCC_AFIO);    // I2C UART ADC EXTI

  //I2C GPIO CONFIGURATION
  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO8|GPIO9);			
  gpio_set(GPIOB,GPIO8|GPIO9);

  //UART1 GPIO CONFIGURATION
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_PULL_UPDOWN , GPIO7);
  gpio_clear(GPIOB, GPIO7);

  //BLUETOOTH GPIO CONFIGURATION
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);


  //TIM2 GPIO CONFIGURATION
  gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO15);  
  gpio_set_mode(GPIOB,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO3);
  
  //ADC GPIO CONFIGURATION
  gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_ANALOG,
		GPIO1);

  //ENCODER ENABLE GPIO CONFIGURATION
  gpio_set_mode(GPIOA,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO4);

  //ENCODER CHARGING GPIO CONFIGURATION
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO12);

  //REMAPS  
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
		     AFIO_MAPR_I2C1_REMAP |  //I2C PB8 + PB9
		     AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 |
		     AFIO_MAPR_USART1_REMAP  ); //USART PB6 + PB7

  //---------------------CONFIGURE I2C-------------------------
  rcc_periph_clock_enable(RCC_I2C1); //Enable I2C clock
  
  i2c_peripheral_disable(I2C1);  
  i2c_reset(I2C1);
  I2C_CR1(I2C1) &= ~I2C_CR1_STOP;	// Clear stop
  i2c_set_standard_mode(I2C1);	// 100 kHz mode
  i2c_set_clock_frequency(I2C1,I2C_CR2_FREQ_36MHZ); // APB Freq
  i2c_set_trise(I2C1,36);		// 1000 ns
  i2c_set_dutycycle(I2C1,I2C_CCR_DUTY_DIV2);
  i2c_set_ccr(I2C1,180);		// 100 kHz <= 180 * 1 /36M
  i2c_peripheral_enable(I2C1);
  //I2C

  //---------------------CONFIGURE UART-------------------------

  rcc_periph_clock_enable(RCC_USART1); //Enable UART clock
  
  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  //  usart_set_flow_control(USART1,USART_FLOWCONTROL_RTS_CTS);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  USART_CR1(USART1) |= USART_CR1_IDLEIE;  //Enable USART1 Receive interrupt.

  //-------------------CONFIGURE DMA---------------------------
  rcc_periph_clock_enable(RCC_DMA1); //Enable DMA clock
  
  //Write DMA
  usart_enable_tx_dma(USART1);
  dma_set_priority(DMA1, DMA_CHANNEL4, DMA_CCR_PL_LOW );
  dma_set_read_from_memory(DMA1, DMA_CHANNEL4);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL4);
  
  dma_set_memory_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);

  dma_set_peripheral_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);

  dma_set_peripheral_address(DMA1, DMA_CHANNEL4, (uint32_t)&USART1_DR);
  //Write DMA

  //Receive DMA
  usart_enable_rx_dma(USART1);
  dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_VERY_HIGH);
  dma_set_read_from_peripheral(DMA1, DMA_CHANNEL5);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);
  dma_enable_circular_mode(DMA1, DMA_CHANNEL5);
  
  dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);

  dma_set_memory_address(DMA1, DMA_CHANNEL5, (uint32_t)receiveBuffer);
  dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t)&USART1_DR);


  dma_set_number_of_data(DMA1, DMA_CHANNEL5, UART_RX_BUFFER_SIZE);
  
  dma_enable_channel(DMA1, DMA_CHANNEL5);
  //Receive DMA
  
  usart_enable(USART1);//Enable USART
  gpio_set(GPIOB, GPIO5);//Initialize RN4020 

  //---------------------CONFIGURE TIM1-------------------------
  rcc_periph_clock_enable(RCC_TIM1);    // TIM1

  TIM1_CCMR1 |= (TIM_CCMR1_CC1S_IN_TI1);//TI1 for CaptureChannel 1
  TIM1_CCER &= ~(TIM_CCER_CC1P);//Rising Edge
  TIM1_CCER |= (TIM_CCER_CC1E);//Enable capture on CaptureChannel1

  TIM1_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI1);//TI1 for CaptureChannel 2
  TIM1_CCER |= (TIM_CCER_CC2P);//Falling Edge
  TIM1_CCER |= (TIM_CCER_CC2E);//Enable capture on CaptureChannel2

  TIM1_CCMR2 |= (TIM_CCMR2_CC3S_IN_TI3);//TI3 for CaptureChannel 3
  TIM1_CCER &= ~(TIM_CCER_CC3P);//Rising Edge
  TIM1_CCER |= (TIM_CCER_CC3E);//Enable capture on CaptureChannel 3  

  TIM1_CCMR2 |= (TIM_CCMR2_CC4S_IN_TI3);//TI3 for CaptureChannel 4
  TIM1_CCER |= (TIM_CCER_CC4P);//Falling Edge
  TIM1_CCER |= (TIM_CCER_CC4E);//Enable capture on CaptureChannel 4

  timer_enable_irq(TIM1, TIM_DIER_CC1IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_CC2IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_CC3IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_CC4IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_UIE);//Interrupts

  TIM1_PSC = 72;
  TIM1_CR1 |= TIM_CR1_CEN;

  rcc_periph_clock_disable(RCC_TIM1);    

  //---------------------CONFIGURE TIM2-------------------------
  rcc_periph_clock_enable(RCC_TIM2);   
  
  TIM2_ARR = 0xFFFF;
  TIM2_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI2 | TIM_CCMR1_CC1S_IN_TI1);
  TIM2_CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM2_SMCR |= TIM_SMCR_SMS_EM3;
  TIM2_CR1 = TIM_CR1_CEN;

  rcc_periph_clock_disable(RCC_TIM2);    

  //---------------------CONFIGURE TIM3-------------------------
  rcc_periph_clock_enable(RCC_TIM3);//Enable TIM3 clock

  TIM3_CR1 |= TIM_CR1_OPM;//One pulse mode
  timer_enable_irq(TIM3, TIM_DIER_UIE);//Enable update interrupt
  TIM3_PSC = 65535;
  TIM3_CNT = 0;

  rcc_periph_clock_disable(RCC_TIM3);//Disable TIM3 clock
  
  //---------------------CONFIGURE EXTI-------------------------
  exti_select_source(EXTI12, GPIOB);
  exti_set_trigger(EXTI12, EXTI_TRIGGER_BOTH);
  exti_enable_request(EXTI12);
      
  //---------------------CONFIGURE ADC---------------------------
  rcc_peripheral_enable_clock(&RCC_APB2ENR,RCC_APB2ENR_ADC1EN); //Enable ADC clock
  
  adc_power_off(ADC1);
  rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);	// Set. 12MHz, Max. 14MHz
  adc_set_dual_mode(ADC_CR1_DUALMOD_IND);       // Independent mode
  adc_set_right_aligned(ADC1);
  adc_set_single_conversion_mode(ADC1);// Continous mode disabled
  adc_enable_temperature_sensor();

  adc_power_on(ADC1);
  adc_reset_calibration(ADC1);
  adc_calibrate_async(ADC1);
  while ( adc_is_calibrating(ADC1) );

  //---------------------CONFIGURE RTOS-------------------------

  protocolMessagesTXRequestQueue = xQueueCreate(PROTOCOL_MESSAGES_TX_REQUEST_QUEUE_SIZE,
					sizeof(protocolMessagesTXRequest_Message));
  humanInterfaceDisplayRequestQueue =
    xQueueCreate(HUMAN_INTERFACE_DISPLAY_REQUEST_QUEUE_SIZE,
		 sizeof(humanInterfaceDisplayRequest_Message));

  protocolMessagesRXSemaphore = xSemaphoreCreateBinary();
  
  protocolMessagesTXRXQueueSet =    
    xQueueCreateSet(PROTOCOL_MESSAGESTXRX_QUEUE_SET_SIZE);
  xQueueAddToSet( protocolMessagesRXSemaphore, protocolMessagesTXRXQueueSet);
  xQueueAddToSet( protocolMessagesTXRequestQueue, protocolMessagesTXRXQueueSet);


  xTaskCreate(protocolMessagesTXRXTask,"protocolMessagesTXRXTask",
	      PROTOCOLMESSAGETXRXTASK_SIZE,NULL,3,NULL);
  xTaskCreate(humanInterfaceDisplayRequestTask,"humanInterfaceDisplayRequestTask",
	      HUMANINTERFACEDISPLAYREQUESTTASK_SIZE, NULL,
	      HUMANINTERFACEDISPLAYREQUESTTASK_PRIORITY, NULL);
  xTaskCreate(batteryFreeTask,"batteryFreeTask1",
	      BATTERYFREETASK_SIZE,NULL,BATTERYFREETASK_PRIORITY
	      ,&batteryFreeTaskHandle);

  ring_buffer_init(&encoder_ring, encoder_buffer, sizeof(encoder_buffer[0]),
		   sizeof(encoder_buffer));

  //---------------------CONFIGURE NVIC---------------------------
  
  nvic_set_priority(NVIC_TIM1_CC_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM1_UP_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM3_IRQ,(0 << 4));
  nvic_set_priority(NVIC_USART1_IRQ,(1 << 4));
  nvic_set_priority(NVIC_EXTI15_10_IRQ, (2 << 4));

  nvic_enable_irq(NVIC_TIM1_CC_IRQ);
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);
  nvic_enable_irq(NVIC_TIM3_IRQ);
  nvic_enable_irq(NVIC_EXTI15_10_IRQ);
  nvic_enable_irq(NVIC_USART1_IRQ);

  //---------------------START RTOS-------------------------------
  vTaskStartScheduler();

  for(;;);

  return 0;
}



