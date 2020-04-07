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

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter()			\
  KIN1_DEMCR |= KIN1_TRCENA_BIT
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
 
#define KIN1_ResetCycleCounter()		\
  KIN1_DWT_CYCCNT = 0
/*!< Reset cycle counter */
 
#define KIN1_EnableCycleCounter()		\
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
/*!< Enable cycle counter */
 
#define KIN1_DisableCycleCounter()		\
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
/*!< Disable cycle counter */
 
#define KIN1_GetCycleCounter()			\
  KIN1_DWT_CYCCNT
/*!< Read cycle counter register */

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

#define INTERRUPTS_BUFFER 256
ring_t interrupts_ring;
uint32_t interrupts_buffer[INTERRUPTS_BUFFER];
#if ((INTERRUPTS_BUFFER - 1) & INTERRUPTS_BUFFER) == 0
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

  rcc_periph_clock_enable(RCC_TIM1);    // TIM1
  rcc_periph_clock_enable(RCC_TIM2);    // TIM2


  //32bit Timer

  //TIM1 
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
  //TIM1

  //TIM2
  gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO15);  
  gpio_set_mode(GPIOB,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO3);
  TIM2_ARR = 0xFFFF;
  TIM2_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI2 | TIM_CCMR1_CC1S_IN_TI1);
  TIM2_CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM2_SMCR |= TIM_SMCR_SMS_EM3;
  TIM2_CNT = 32767;
  TIM2_CR1 = TIM_CR1_CEN;

  //EXTI

  //EXTI

  //I2C
  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO8|GPIO9);			
  gpio_set(GPIOB,GPIO8|GPIO9);
  //I2C

  //UART

  //UART

  //Bluetooth

  //Bluetooth

  //ADC

  //ADC

  //Encoder Sensors Powering up
  gpio_set_mode(GPIOA,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO4);

  gpio_clear(GPIOA, GPIO4);

  //Encoder Sensors Powering up

  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
		     AFIO_MAPR_I2C1_REMAP |  //I2C PB8 + PB9
		     AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 |
		     AFIO_MAPR_USART1_REMAP  ); //USART PB6 + PB7  

  nvic_enable_irq(NVIC_TIM1_CC_IRQ);
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);
  nvic_set_priority(NVIC_TIM1_CC_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM1_UP_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM3_IRQ,(0 << 4));

  KIN1_InitCycleCounter(); /* enable DWT hardware */
  KIN1_ResetCycleCounter(); /* reset cycle counter */
  KIN1_EnableCycleCounter(); /* start counting */

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

      }else if(dataStruct.eDataSource == adcSender){
	sendToLCDQueue(batteryLevel,dataStruct.traveledDistanceOrADC);
      }
      
    }else if ( xHandle == (QueueSetMemberHandle_t ) communicationSemaphore){
      xSemaphoreTake(communicationSemaphore,0);//Should go first

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

  uint32_t cyclesBefore = 0;
  uint32_t cyclesAfter = 0;
  uint32_t cyclesDif = 0;

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

      cyclesAfter = KIN1_GetCycleCounter();
      cyclesDif = cyclesAfter - cyclesBefore;

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

      cyclesBefore = KIN1_GetCycleCounter();
    }


    vTaskDelay(pdMS_TO_TICKS(50));
    
  }
}

void tim1_cc_isr(){

  uint32_t cyclesBefore = KIN1_GetCycleCounter();

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

  uint32_t cyclesAfter = KIN1_GetCycleCounter();
  uint32_t cycleDiff = cyclesBefore - cyclesAfter;
  
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

  ring_buffer_init(&interrupts_ring, interrupts_buffer, sizeof(interrupts_buffer[0]),
		   sizeof(interrupts_buffer));

  communicationQueue =  xQueueCreate(COMMUNICATION_QUEUE_SIZE, sizeof(commData_t));
  lcdQueue = xQueueCreate(LCD_QUEUE_SIZE, sizeof(lcdData_t));

  communicationQueueSet = xQueueCreateSet(COMMUNICATION_QUEUE_SET_SIZE);
  xQueueAddToSet( communicationQueue, communicationQueueSet);
  xQueueAddToSet( communicationSemaphore, communicationQueueSet);
  
  xTaskCreate(communicationTask,"communicationTask",800,NULL,1,NULL);
  xTaskCreate(lcdTask,"lcdTask",200, NULL, 2, NULL);
  xTaskCreate(encoderTask,"encoderTask",300,NULL,4,NULL);

  vTaskStartScheduler();

  for(;;);

  return 0;
}

