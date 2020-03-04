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
#include <libopencm3/stm32/exti.h>
#include "printf.h"
#include "lcd.h"
#include "uart.h"
#include "ring.h"
#include "queueSending.h"
#include "bluetooth.h"
#include <string.h>

//encoderTask -------------------------------
typedef struct encoderValues_t{
  uint16_t encoderCounter;
  uint32_t inputCapture;
} encoderValues_t;
//encoderTask -------------------------------

#define ENCODER_BUFFER_LEN 256
ring_t encoder_ring;
encoderValues_t encoder_buffer[ENCODER_BUFFER_LEN];
#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
#else
	#warning ENCODER_BUFFER NOT POWER OF 2
#endif

//Only accessed form tim1_cc_isr
static volatile encoderValues_t encInterruptValues;
static volatile uint16_t tim2Counter = 0;
static volatile uint32_t capturedTime = 0;
static volatile uint32_t overflowCounter=0 ;

#define ENCODERINITIAL_VALUE 32767

uint8_t descendente(uint16_t a, uint16_t b);
uint8_t ascendente(uint16_t a, uint16_t b);
uint8_t minDistTraveledDes(uint16_t counter, uint16_t minDistToTravel);
uint8_t minDistTraveledAs(uint16_t counter, uint16_t minDistToTravel);
uint8_t returnToInitialDes(uint16_t counter, uint16_t minDistToTravel);
uint8_t returnToInitialAs(uint16_t counter, uint16_t minDistToTravel);
uint8_t maxDistAs(uint16_t counter, uint16_t lastMaxDist);
uint8_t maxDistDes(uint16_t counter, uint16_t lastMaxDist);

void lcdPutsBlinkFree(const char *g, int ypos);

uint8_t (*goingDesiredCountDir[2])(uint16_t, uint16_t) = {descendente, ascendente};
uint8_t (*hasTraveledMinDist[2])(uint16_t, uint16_t) = {minDistTraveledDes,
						      minDistTraveledAs};
uint8_t (*hasReturnedToInitial[2])(uint16_t, uint16_t) =
  {returnToInitialDes,returnToInitialAs};
uint8_t (*canCountMaxDist[2])(uint16_t,uint16_t) = {maxDistDes, maxDistAs};

uint8_t descendente(uint16_t a, uint16_t b){
  return a<b;
}

uint8_t ascendente(uint16_t a, uint16_t b){
  return a>b;
}

uint8_t minDistTraveledDes(uint16_t counter, uint16_t minDistToTravel){
  return (counter <= ENCODERINITIAL_VALUE - minDistToTravel);
}

uint8_t minDistTraveledAs(uint16_t counter, uint16_t minDistToTravel){
  return (counter >= ENCODERINITIAL_VALUE + minDistToTravel);
}

uint8_t returnToInitialDes(uint16_t counter, uint16_t minDistToTravel){
  return (counter > ENCODERINITIAL_VALUE - minDistToTravel);
}

uint8_t returnToInitialAs(uint16_t counter, uint16_t minDistToTravel){
  return (counter < ENCODERINITIAL_VALUE + minDistToTravel);
}

uint8_t maxDistAs(uint16_t counter, uint16_t lastMaxDist){
  return (counter > lastMaxDist);
}

uint8_t maxDistDes(uint16_t counter, uint16_t lastMaxDist){
  return (counter < lastMaxDist);
}

static void configurePeriphereals(void){

  rcc_periph_clock_enable(RCC_AFIO);    // I2C UART ADC EXTI
  rcc_periph_clock_enable(RCC_GPIOA);   // TIM1 TIM2 ADC
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM2 UART
  rcc_periph_clock_enable(RCC_GPIOC);	// I2C TIM2
  rcc_periph_clock_enable(RCC_I2C1);	// I2C
  rcc_periph_clock_enable(RCC_USART1);  // UART
  rcc_peripheral_enable_clock(&RCC_APB2ENR,RCC_APB2ENR_ADC1EN); //ADC
  rcc_periph_clock_enable(RCC_DMA1);    // DMA

  //EXTI


  gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO12);

  exti_select_source(EXTI12, GPIOB);
  exti_set_trigger(EXTI12, EXTI_TRIGGER_BOTH);
  exti_enable_request(EXTI12);
  //EXTI

  //I2C
  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO8|GPIO9);			
  gpio_set(GPIOB,GPIO8|GPIO9);
  //I2C

  //UART
  uart_configure();
  //UART

  //Bluetooth
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);
  //Bluetooth

  //ADC
  gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_ANALOG,
		GPIO1);	
  
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
  //ADC

  //Encoder Sensors Powering up
  gpio_set_mode(GPIOA,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO4);

  //Encoder Sensors Powering up

  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
		     AFIO_MAPR_I2C1_REMAP |  //I2C PB8 + PB9
		     AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 |
		     AFIO_MAPR_USART1_REMAP  ); //USART PB6 + PB7  
    
  
  nvic_enable_irq(NVIC_USART1_IRQ);
  nvic_enable_irq(NVIC_EXTI15_10_IRQ);
  nvic_set_priority(NVIC_TIM1_CC_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM1_UP_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM3_IRQ,(0 << 4));
  nvic_set_priority(NVIC_USART1_IRQ,(1 << 4));
  nvic_set_priority(NVIC_EXTI15_10_IRQ, (2 << 4));

}

static uint16_t
read_adc(uint8_t channel) {

	adc_set_sample_time(ADC1,channel,ADC_SMPR_SMP_239DOT5CYC);
	adc_set_regular_sequence(ADC1,1,&channel);
	adc_start_conversion_direct(ADC1);
	while ( !adc_eoc(ADC1) )
		taskYIELD();
	return adc_read_regular(ADC1);
}


void lcdPutsBlinkFree(const char *g, int ypos){
  int i = 0;
  char text[22];//max used characters used plus null terminator
  int blankchars;

  while(*(g + i)){
    if(i >20) break;
    text[i] = *(g +i);
    i++;
  }

  blankchars = 21 -i;
  while(blankchars){
    text[i] = ' ';
    blankchars--;
    i++;
  }
  text[i] = '\0';
  lcd_gotoxy(0, ypos);
  lcd_puts(text);
  
}

static void communicationTask(void *args __attribute__((unused))) {

  commData_t dataStruct;

  charLineBuffer_t *charLineBuffer;
  QueueSetMemberHandle_t xHandle;

  gpio_set(GPIOB, GPIO5);//initializing rn4020
  sendToLCDQueue(turnOnMessage,0);
  
  for (;;) {

    xHandle = xQueueSelectFromSet(communicationQueueSet,portMAX_DELAY);//Should go first

    if( xHandle == ( QueueSetMemberHandle_t ) communicationQueue){
      xQueueReceive(communicationQueue, &dataStruct,0);//Should go first
      
      
      if(dataStruct.eDataSource == encoderSender){
	writeEncoderValues(dataStruct.traveledDistanceOrADC,
			   dataStruct.meanPropulsiveVelocity,
			   dataStruct.peakVelocity);
      }else if(dataStruct.eDataSource == adcSender){
	sendToLCDQueue(batteryLevel,dataStruct.traveledDistanceOrADC);
      }
      
    }else if ( xHandle == (QueueSetMemberHandle_t ) communicationSemaphore){
      xSemaphoreTake(communicationSemaphore,0);//Should go first
      while(serialAvailable()){
	charLineBuffer = forceReadCharLineUsart();
	genericLineParsing(charLineBuffer);
      }
    }

  }
}

static void lcdTask(void *args __attribute__((unused))){
  lcdData_t receivedData;
  char buffer[22];
  lcd_init(LCD_DISP_ON);
  uint16_t lineState = 0;
  uint32_t lastVoltage = 0;
  
  for(;;){
    xQueueReceive(lcdQueue,&receivedData, portMAX_DELAY);
    if(receivedData.messageType == turnOnMessage){
      lcdPutsBlinkFree(" SDT ENCODER",0);
      
    }else if(receivedData.messageType == bleConfig){
      lcdPutsBlinkFree(" SDT ENCODER     BLE",0);

    }else if(receivedData.messageType == connectedStatus){
      lcdPutsBlinkFree(receivedData.displayValue?
		       "Connected":"         ",
		       2);
      
    }else if(receivedData.messageType == batteryLevel){
      lineState = GPIOB_IDR;
      if( (lineState & (1 <<12)) !=0){
	sprintf(buffer,"    ** Batt: %lu", receivedData.displayValue);
	lcdPutsBlinkFree(buffer,7);
      }else{
	sprintf(buffer,"       Batt: %lu", receivedData.displayValue);
	lcdPutsBlinkFree(buffer,7);
      }
      lastVoltage = receivedData.displayValue;
      
    }else if(receivedData.messageType == chargingStatus){
      sprintf(buffer, receivedData.displayValue?
	      "    ** Batt: %lu":
	      "       Batt: %lu", lastVoltage);
      lcdPutsBlinkFree(buffer, 7);
      
    }else if(receivedData.messageType == encoder){
      sprintf(buffer, "%lu", receivedData.displayValue);
      lcdPutsBlinkFree(buffer,5);
    }
  }
}

static void adcTask(void *args __attribute__((unused))) {

  uint16_t voltageSupply = 0;
  uint16_t adc1Voltage = 0;
  adc_start_conversion_direct(ADC1);
  voltageSupply = 4914000/read_adc(ADC_CHANNEL_VREF);
  adc1Voltage = (read_adc(ADC_CHANNEL1) * voltageSupply / 4095) *2;
  
  for(;;){
    
    //1.20 - 4095             INVERSE RULE OF 3
    //V+   - ADC_CHANNEL_VREF
    
    //V+ - 4095              DIRECT RULE OF 3
    // x  - ADC_CHANNEL1
    
    voltageSupply = 4914000/read_adc(ADC_CHANNEL_VREF);
    adc1Voltage = (read_adc(ADC_CHANNEL1) * voltageSupply / 4095) *2;

    sendToCommunicationQueue(adcSender,adc1Voltage,0,0);

    if(getBLEConnected()){
      xSemaphoreTake(adcSemaphore,portMAX_DELAY);      
    }else{
      vTaskDelay(pdMS_TO_TICKS(4000));
    }

  }
}



static void encoderTask(void *args __attribute__((unused))){

  encoderValues_t receiveEncValues;

  uint16_t minDistToTravel = 0;
  uint8_t desiredCountDir = 1; //1 = Ascendent, 0 = Descendent
  uint8_t desiredRepDir = 1; //1 = Ascendent, 0 = Descendent

  uint16_t lastPosition = ENCODERINITIAL_VALUE;
  uint8_t elapsedPosition = 0;
  uint32_t elapsedDistance = 0;
  uint32_t lastTime = 0;
  uint32_t elapsedTime = 0;
  uint8_t desiredDirFollowed = 0;

  //Velocity Calulcation Variables
  uint16_t lastMaxCounter = ENCODERINITIAL_VALUE;
  uint32_t lastVelocity = 0;
  uint32_t currentVelocity = 0;
  
  uint32_t meanPropVelCount = 0;
  uint32_t meanPropVel = 0;
  uint32_t peakVel = 0;

  //Repetition algorithm variables
  uint8_t canCountRep = 0;
  uint8_t  minDistTraveled= 0;

  for(;;){

    if(getENCODERStarted()){

    }else{
      xSemaphoreTake(encoderSemaphore, portMAX_DELAY);

      minDistToTravel = getMinDistToTravel();
      desiredCountDir = getDesiredCountDir();
      desiredRepDir = getDesiredRepDir();
      
      //Reset initial Values
      lastPosition = ENCODERINITIAL_VALUE;
      lastTime = 0;

      lastMaxCounter = ENCODERINITIAL_VALUE;
      lastVelocity = 0;

      meanPropVelCount = 0;
      meanPropVel = 0;
      peakVel = 0;
    }
    
    while(ring_buffer_get(&encoder_ring, &receiveEncValues) != -1){

      //VELOCITY ALGORITHM
      if( (*goingDesiredCountDir[desiredCountDir])(receiveEncValues.encoderCounter,
					   lastPosition) ){
	desiredDirFollowed = 1;
	if(desiredCountDir){
	  elapsedPosition = receiveEncValues.encoderCounter - lastPosition;
	}else{
	  elapsedPosition = lastPosition - receiveEncValues.encoderCounter;
	}
      }else{
	desiredDirFollowed = 0;
      }

      if( (*canCountMaxDist[desiredRepDir])(receiveEncValues.encoderCounter,
					    lastMaxCounter) ){
	lastMaxCounter = receiveEncValues.encoderCounter;
      }

      lastPosition = receiveEncValues.encoderCounter;
      elapsedTime = receiveEncValues.inputCapture - lastTime;
      lastTime = receiveEncValues.inputCapture;
      
      if(desiredDirFollowed){
	elapsedDistance = 4084 * elapsedPosition;
	currentVelocity = (elapsedDistance * 100) / elapsedTime;

	//Peak Velocity Calculation
	if(currentVelocity > peakVel) peakVel = currentVelocity;

	//Mean Propulsive Velocity Calculation
	if(currentVelocity > lastVelocity){
	  meanPropVelCount++;
	  meanPropVel += currentVelocity;
	}
	lastVelocity = currentVelocity;
      }
      //VELOCITY ALGORITHM

      //REPETITION ALGORITHM
      if((*hasTraveledMinDist[desiredRepDir])
	(receiveEncValues.encoderCounter, minDistToTravel)){
	minDistTraveled = 1;
      }

      if(minDistTraveled && (*hasReturnedToInitial[desiredRepDir])
	 (receiveEncValues.encoderCounter, minDistToTravel)){
	canCountRep = 1;
      }
      
      if(canCountRep){
	sendToCommunicationQueue(encoderSender,
				 lastMaxCounter,
				 meanPropVel/meanPropVelCount,
				 peakVel);
	minDistTraveled = 0;
	canCountRep = 0;

	//Reset Velocity Calculation Variables
	lastMaxCounter = ENCODERINITIAL_VALUE;
	lastVelocity = 0;
	
	meanPropVelCount = 0;
	meanPropVel = 0;
	peakVel = 0;
      }
      //REPETITION ALGORITHM
    }


    vTaskDelay(pdMS_TO_TICKS(50));
    
  }
}

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
  
  // while((uint16_t)TIM2_CNT == tim2Counter);
  tim2Counter = (uint16_t)TIM2_CNT;
  encInterruptValues.encoderCounter = tim2Counter;
  
  ring_buffer_put(&encoder_ring, (encoderValues_t*)&encInterruptValues);
}

void tim1_up_isr(){
  TIM1_SR &= ~TIM_SR_UIF;//Should go first
  overflowCounter++;  
}

void exti15_10_isr(){
  exti_reset_request(EXTI12);
  uint16_t lineState = GPIOB_IDR;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  lcdData_t dataToSend;

  if( (lineState & (1 <<12)) != 0){
    dataToSend.messageType = chargingStatus;
    dataToSend.displayValue = 1;
    xQueueSendToBackFromISR(lcdQueue, &dataToSend,&xHigherPriorityTaskWoken);
  }else{
    dataToSend.messageType = chargingStatus;
    dataToSend.displayValue = 0;
    xQueueSendToBackFromISR(lcdQueue, &dataToSend,&xHigherPriorityTaskWoken);
  }


  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  configurePeriphereals();

  ring_buffer_init(&encoder_ring, encoder_buffer, sizeof(encoder_buffer[0]),
							 sizeof(encoder_buffer));

  communicationQueue =  xQueueCreate(COMMUNICATION_QUEUE_SIZE, sizeof(commData_t));
  lcdQueue = xQueueCreate(LCD_QUEUE_SIZE, sizeof(lcdData_t));

  adcSemaphore = xSemaphoreCreateBinary();
  encoderSemaphore = xSemaphoreCreateBinary();

  communicationQueueSet = xQueueCreateSet(COMMUNICATION_QUEUE_SET_SIZE);
  xQueueAddToSet( communicationQueue, communicationQueueSet);
  xQueueAddToSet( communicationSemaphore, communicationQueueSet);
  
  xTaskCreate(communicationTask,"communicationTask",800,NULL,1,NULL);
  xTaskCreate(lcdTask,"lcdTask",200, NULL, 2, NULL);
  xTaskCreate(adcTask,"adcTask",200,NULL,2,NULL);
  xTaskCreate(uartRxTask, "uartRxTask", 300,NULL, 3, NULL);
  xTaskCreate(encoderTask,"encoderTask",300,NULL,4,NULL);

  vTaskStartScheduler();

  for(;;);

  return 0;
}

