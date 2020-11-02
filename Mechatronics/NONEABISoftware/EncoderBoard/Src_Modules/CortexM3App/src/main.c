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

//UART RX------------------------------------
char receiveBuffer[UART_RX_BUFFER_LEN] = {0};

void uartRxTask(void *args);//DELETE!!!

#define UART_RX_BUFFER 256//DELETE!!!
ring_t usart_rx_ring;//DELETE!!!
char usart_rx_buffer[UART_RX_BUFFER];//DELETE!!!
#if ((UART_RX_BUFFER - 1) & UART_RX_BUFFER) == 0//DELETE!!!
#else//DELETE!!!
	#warning UART_RX_BUFFER NOT POWER OF 2//DELETE!!!
#endif//DELETE!!!
SemaphoreHandle_t idleSmphr;//DELETE!!!
SemaphoreHandle_t communicationSemaphore;//DELETE!!!
charLineBuffer_t charLineBuffer;//DELETE!!!

//Ring buffer handles------------------------
ring_t encoder_ring;

//Queue handles------------------------------
QueueHandle_t uartTXQueue;
QueueHandle_t lcdQueue;
QueueHandle_t communicationQueue;
QueueSetHandle_t communicationQueueSet;

//Task Handles-------------------------------
TaskHandle_t encoderTaskHandle = NULL;
TaskHandle_t adcFreeTaskHandle = NULL;
TaskHandle_t adcWaitTaskHandle = NULL;

//Encoder variables--------------------------
uint8_t (*goingDesiredCountDir[2])(uint32_t, uint32_t) = {descendente, ascendente};
uint8_t (*hasTraveledMinDist[2])(uint32_t, uint32_t) = {minDistTraveledDes,
						      minDistTraveledAs};
uint8_t (*hasReturnedToInitial[2])(uint32_t, uint32_t) =
  {returnedToInitialDes,returnedToInitialAs};
uint8_t (*newMaxRomDetected[2])(uint32_t,uint32_t) = {maxRomDes, maxRomAs};

//ISR variables------------------------------
//Timer ISR variables
static volatile encoderValues_t encInterruptValues;
static volatile uint16_t tim2Counter = 0;
static volatile uint32_t capturedTime = 0;
static volatile uint32_t overflowCounter=0 ;

//RingBuffer buffers------------------------------------
encoderValues_t encoder_buffer[ENCODER_BUFFER_LEN];

//Internal functions-------------------------
void encoderTask(void *args);
void uartRXTask(void *args);
void uartTXTask(void *args);
void lcdTask(void *args);
void adcFreeTask(void *args);
void adcWaitTask(void *args);

void sendToUARTTXQueue(messageTypes_t messageType,
		       uint16_t traveledDistanceOrADC,
		       uint16_t meanPropulsiveVelocity,
		       uint16_t peakVelocity){
  uartTXData_t dataToSend;
  dataToSend.messageType = messageType;
  dataToSend.traveledDistanceOrADC = traveledDistanceOrADC;
  dataToSend.meanPropulsiveVelocity = meanPropulsiveVelocity;
  dataToSend.peakVelocity = peakVelocity;

  xQueueSendToBack(uartTXQueue,&dataToSend,0);

}
void sendToLCDQueue(messageTypes_t messageType,
		    uint32_t displayValue){
  lcdData_t dataToSend;
  dataToSend.messageType = messageType;
  dataToSend.displayValue = displayValue;

  xQueueSendToBack(lcdQueue,&dataToSend,0);
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

uint32_t readADC(void){

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

void printStringUART(const char myString[]) {

  dma_disable_channel(DMA1, DMA_CHANNEL4);
  dma_set_number_of_data(DMA1, DMA_CHANNEL4, strlen(myString));
  dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)myString);
  dma_enable_channel(DMA1, DMA_CHANNEL4);

  while(!dma_get_interrupt_flag(DMA1, DMA_CHANNEL4, DMA_TCIF));
  dma_clear_interrupt_flags(DMA1, DMA_CHANNEL4, DMA_TCIF); 

}

int serialAvailable(void){
  return  (!ring_buffer_empty(&usart_rx_ring))? 1:0;
}

char get_char(void){
  char c = 0;
  
  ring_buffer_get(&usart_rx_ring, &c);
  
  return c;
}

charLineBuffer_t *forceReadCharLineUsart(){

  int i = 0;
  char c;
  char *buffer = charLineBuffer.buf;

  while((c = get_char()) != '\n'){

    //if characters were found
    if(c != 0){
      //Add characters to buffer;
      buffer[i] = c;
      i++;
    }
  }

  buffer[i] = c;//Write string termination chacter (\n)
  charLineBuffer.terminatorcharposition = i;

  return &charLineBuffer;
}

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

  //UART

  //Configure ring buffer
  ring_buffer_init(&usart_rx_ring, usart_rx_buffer, sizeof(usart_rx_buffer[0]),
		   sizeof(usart_rx_buffer));
  
  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);

  /* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_PULL_UPDOWN , GPIO7);
  gpio_clear(GPIOB, GPIO7);

  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  //  usart_set_flow_control(USART1,USART_FLOWCONTROL_RTS_CTS);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  

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

  dma_set_number_of_data(DMA1, DMA_CHANNEL5, UART_RX_BUFFER_LEN);
  
  dma_enable_channel(DMA1, DMA_CHANNEL5);
  //Receive DMA

  idleSmphr = xSemaphoreCreateBinary();
  communicationSemaphore = xSemaphoreCreateBinary();
  USART_CR1(USART1) |= USART_CR1_IDLEIE;  //Enable USART1 Receive interrupt.

  usart_enable(USART1);//Enable USART
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

  //TIM 1
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

  //TIM2
  rcc_periph_clock_enable(RCC_TIM2);    // TIM2
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
  TIM2_CR1 = TIM_CR1_CEN;

  rcc_periph_clock_disable(RCC_TIM1);    // TIM1
  rcc_periph_clock_disable(RCC_TIM2);    // TIM2
    
  
  nvic_enable_irq(NVIC_USART1_IRQ);
  nvic_enable_irq(NVIC_EXTI15_10_IRQ);
  nvic_enable_irq(NVIC_TIM1_CC_IRQ);
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);
  nvic_set_priority(NVIC_TIM1_CC_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM1_UP_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM3_IRQ,(0 << 4));
  nvic_set_priority(NVIC_USART1_IRQ,(1 << 4));
  nvic_set_priority(NVIC_EXTI15_10_IRQ, (2 << 4));


  //  KIN1_InitCycleCounter(); /* enable DWT hardware */
  //  KIN1_ResetCycleCounter(); /* reset cycle counter */
  //  KIN1_EnableCycleCounter(); /* start counting */

}

void communicationTask(void *args __attribute__((unused))) {

  uartTXData_t receivedData;

  charLineBuffer_t *charLineBuffer;
  QueueSetMemberHandle_t xHandle;

  gpio_set(GPIOB, GPIO5);//initializing rn4020
  sendToLCDQueue(turnOnMessage,0);
  
  for (;;) {

    xHandle = xQueueSelectFromSet(communicationQueueSet,portMAX_DELAY);//Should go first

    if( xHandle == ( QueueSetMemberHandle_t ) uartTXQueue){
      xQueueReceive(uartTXQueue, &receivedData,0);//Should go first
      
      
      if(receivedData.messageType == encoderData){
	writeEncoderValues(receivedData.traveledDistanceOrADC,
			   receivedData.meanPropulsiveVelocity,
			   receivedData.peakVelocity);
      }else if(receivedData.messageType == batteryLevel){

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

void lcdTask(void *args __attribute__((unused))){
  lcdData_t receivedData;
  char buffer[22];
  char numBuffer[5];

  uint16_t lineState = 0;
  uint32_t lastVoltage = 0;
  
  lcd_init();
  
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
      
    }else if(receivedData.messageType == encoderData){
      sprintf(buffer, "%lu", receivedData.displayValue);
      lcdPutsBlinkFree(buffer,5);
    }
  }
}

void adcFreeTask(void *args __attribute__((unused))) {

  uint32_t adcData = 0;
  
  for(;;){
    
    adcData = readADC();

    sendToLCDQueue(batteryLevel,adcData);

    vTaskDelay(pdMS_TO_TICKS(4000));
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
  
  tim2Counter = (uint16_t)TIM2_CNT;
  encInterruptValues.encoderCounter = tim2Counter;
  
  ring_buffer_put(&encoder_ring, (encoderValues_t*)&encInterruptValues);

}

void tim1_up_isr(){
  TIM1_SR &= ~TIM_SR_UIF;//Should go first
  overflowCounter++;  
}

void exti15_10_isr(){
  
  exti_reset_request(EXTI12);//Clear flag
  
  uint16_t lineState = GPIOB_IDR;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  lcdData_t dataToSend;

  dataToSend.messageType = chargingStatus;  

  if( (lineState & (1 <<12)) != 0){
    dataToSend.displayValue = 1;
    xQueueSendToBackFromISR(lcdQueue, &dataToSend,&xHigherPriorityTaskWoken);
  }else{
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

  uartTXQueue = xQueueCreate(TX_QUEUE_SIZE, sizeof(uartTXData_t));
  lcdQueue = xQueueCreate(LCD_QUEUE_SIZE, sizeof(lcdData_t));

  communicationQueueSet = xQueueCreateSet(COMMUNICATION_QUEUE_SET_SIZE);
  xQueueAddToSet( uartTXQueue, communicationQueueSet);
  xQueueAddToSet( communicationSemaphore, communicationQueueSet);
  
  xTaskCreate(communicationTask,"communicationTask",800,NULL,1,NULL);
  xTaskCreate(lcdTask,"lcdTask",200, NULL, 2, NULL);
  xTaskCreate(adcFreeTask,"adcFreeTask",100,NULL,2,&adcFreeTaskHandle);
  xTaskCreate(uartRxTask, "uartRxTask", 300,NULL, 3, NULL);

  vTaskStartScheduler();

  for(;;);

  return 0;
}


void uartRxTask(void *args __attribute__((unused))){
  
  int i = 0;
  char bufferP[3];
  char c = 0;

  for(;;){
    xSemaphoreTake(idleSmphr,portMAX_DELAY); //Should go first

    while(receiveBuffer[i]){
      c = receiveBuffer[i];
      ring_buffer_put(&usart_rx_ring,&c);

      receiveBuffer[i] = 0;
      i = (i+1) % UART_RX_BUFFER_LEN;
    }

    xSemaphoreGive(communicationSemaphore);
  }
}

void usart1_isr(void)
{
  if (((USART_CR1(USART1) & USART_CR1_IDLEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_IDLE) != 0)) {
    usart_recv(USART1);//Interrupt clears at reading rx register

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(idleSmphr,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
  }
}

