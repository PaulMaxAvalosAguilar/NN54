#include "FreeRTOS.h"
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
#include "printf.h"
#include "lcd.h"
#include "uart.h"
#include "ring.h"
#include "queueSending.h"
#include "bluetooth.h"
#include <string.h>

//encoderTask -------------------------------
typedef struct encoderValues_t{
  int16_t encoderCounter;
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

static volatile encoderValues_t encInterruptValues;//Only accessed form tim1_cc_isr
static volatile int16_t tim2Counter = 0;
static volatile uint32_t capturedTime = 0;
static volatile uint32_t overflowCounter=0 ;

static void configurePeriphereals(void){

  rcc_periph_clock_enable(RCC_AFIO);    // I2C UART ADC
  rcc_periph_clock_enable(RCC_GPIOA);   // TIM1 TIM2 ADC
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM2 UART
  rcc_periph_clock_enable(RCC_GPIOC);	// I2C TIM2
  rcc_periph_clock_enable(RCC_I2C1);	// I2C
  rcc_periph_clock_enable(RCC_TIM1);    // TIM1
  rcc_periph_clock_enable(RCC_TIM2);    // TIM2
  rcc_periph_clock_enable(RCC_USART1);  // UART
  rcc_peripheral_enable_clock(&RCC_APB2ENR,RCC_APB2ENR_ADC1EN); //ADC
  rcc_periph_clock_enable(RCC_DMA1);    // DMA

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
  //TIM2  

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
  gpio_clear(GPIOA, GPIO4);
  //Encoder Sensors Powering up

  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
		     AFIO_MAPR_I2C1_REMAP |  //I2C PB8 + PB9
		     AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 |
		     AFIO_MAPR_USART1_REMAP  ); //USART PB6 + PB7
  
  nvic_enable_irq(NVIC_TIM1_CC_IRQ);
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);
  nvic_enable_irq(NVIC_USART1_IRQ);
  nvic_set_priority(NVIC_TIM1_CC_IRQ,(0 << 4));
  nvic_set_priority(NVIC_TIM1_UP_IRQ,(0 << 4));
  nvic_set_priority(NVIC_USART1_IRQ,(1 << 4));

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


static void lcdPutsBlinkFree(const char *g, int ypos){
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

  char buffer[3];
  charLineBuffer_t *charLineBuffer;
  QueueSetMemberHandle_t xHandle;

  #ifndef BLUETOOTHUARTONLY
  printString("InitializingBluetooth\n");  //Clear for bluetooth Config
  #endif
  
  gpio_set(GPIOB, GPIO5);
  sendToLCDQueue(turnOnMessage,0);
  
  for (;;) {

    xHandle = xQueueSelectFromSet(communicationQueueSet,portMAX_DELAY);//Should go first

    if( xHandle == ( QueueSetMemberHandle_t ) communicationQueue){
      xQueueReceive(communicationQueue, &dataStruct,0);//Should go first
      
      if(dataStruct.eDataSource == adcSender){
	sendToLCDQueue(batteryLevel,dataStruct.uValue);
      }else if(dataStruct.eDataSource == encoderSender){
	sendToLCDQueue(encoder, dataStruct.uValue);
      }
      
    }else if ( xHandle == (QueueSetMemberHandle_t ) communicationSemaphore){
      xSemaphoreTake(communicationSemaphore,0);//Should go first
      while(serialAvailable()){
	charLineBuffer = forceReadCharLineUsart();
	genericLineParsing(charLineBuffer);
      }
    }

    if(characteristicStatus.handleFound){

    }

    //    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static void lcdTask(void *args __attribute__((unused))){
  lcdData_t receivedData;
  char buffer[22];
  lcd_init(LCD_DISP_ON);
  
  for(;;){
    xQueueReceive(lcdQueue,&receivedData, portMAX_DELAY);
    if(receivedData.messageType == turnOnMessage){
      lcdPutsBlinkFree(" SDT ENCODER",0);
      
    }else if(receivedData.messageType == bleConfig){
      lcdPutsBlinkFree(" SDT ENCODER     BLE",0);

    }else if(receivedData.messageType == connectedStatus){
      lcdPutsBlinkFree(receivedData.displayValue?
		       "Connected":"Disconnected",
		       2);
      
    }else if(receivedData.messageType == batteryLevel){
      sprintf(buffer,"    Battery: %lu", receivedData.displayValue);
      lcdPutsBlinkFree(buffer,7);
      
    }else if(receivedData.messageType == chargingStatus){
      lcdPutsBlinkFree(receivedData.displayValue?
		       "Charging":
		       "Not Charging",
		       4);
      
    }else if(receivedData.messageType == encoder){
      sprintf(buffer, "%ld", receivedData.displayValue);
      lcdPutsBlinkFree(buffer,5);
    }
  }
}

static void adcTask(void *args __attribute__((unused))) {

  uint16_t voltageSupply = 0;
  uint16_t adc1Voltage = 0;

  adc_start_conversion_direct(ADC1);
  
  for(;;){
    //1.20 - 4095             INVERSE RULE OF 3
    //V+   - ADC_CHANNEL_VREF
    
    //V+ - 4095              DIRECT RULE OF 3
    // x  - ADC_CHANNEL1
    
    voltageSupply = 4914000/read_adc(ADC_CHANNEL_VREF);
    adc1Voltage = (read_adc(ADC_CHANNEL1) * voltageSupply / 4095) *2;

    sendToCommunicationQueue(adcSender,adc1Voltage);
    
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}



static void encoderTask(void *args __attribute__((unused))){

  int16_t encCounter = 0;
  uint32_t inputCapture = 0;

  encoderValues_t receiveEncValues;
    
  for(;;){
    while(ring_buffer_get(&encoder_ring, &receiveEncValues) != -1){
      encCounter = receiveEncValues.encoderCounter;
      inputCapture = receiveEncValues.inputCapture;

      sendToCommunicationQueue(encoderSender, encCounter);
    }
    vTaskDelay(pdMS_TO_TICKS(30));
  }
}

void tim1_cc_isr(){

  tim2Counter = (uint16_t)TIM2_CNT - (int16_t)32767;
  
  if(TIM1_SR & TIM_SR_CC1IF){
    capturedTime = (overflowCounter << 16) | TIM1_CCR1;
  }else if(TIM1_SR & TIM_SR_CC2IF){
    capturedTime = (overflowCounter << 16) | TIM1_CCR2;
  }else if(TIM1_SR & TIM_SR_CC3IF){
    capturedTime = (overflowCounter << 16) | TIM1_CCR3;
  }else if(TIM1_SR & TIM_SR_CC4IF){
    capturedTime = (overflowCounter << 16) | TIM1_CCR4;
  }

  encInterruptValues.encoderCounter = tim2Counter;
  encInterruptValues.inputCapture = capturedTime;

  ring_buffer_put(&encoder_ring, (encoderValues_t*)&encInterruptValues);
}

void tim1_up_isr(){
  TIM1_SR &= ~TIM_SR_UIF;//Should go first
  overflowCounter++;  
}


int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  configurePeriphereals();

  ring_buffer_init(&encoder_ring, encoder_buffer, sizeof(encoder_buffer[0]),
							 sizeof(encoder_buffer));

  communicationQueue =  xQueueCreate(COMMUNICATION_QUEUE_SIZE, sizeof(commData_t));
  lcdQueue = xQueueCreate(LCD_QUEUE_SIZE, sizeof(lcdData_t));

  communicationQueueSet = xQueueCreateSet(COMMUNICATION_QUEUE_SET_SIZE);
  xQueueAddToSet( communicationQueue, communicationQueueSet);
  xQueueAddToSet( communicationSemaphore, communicationQueueSet);
  
  xTaskCreate(communicationTask,"communicationTask",800,NULL,1,NULL);
  xTaskCreate(lcdTask,"lcdTask",200, NULL, 1, NULL);
  xTaskCreate(adcTask,"adcTask",200,NULL,1,NULL);
  xTaskCreate(uartRxTask, "uartRxTask", 300,NULL, 2, NULL);
  xTaskCreate(encoderTask,"encoderTask",300,NULL,3,NULL);

  vTaskStartScheduler();

  for(;;);

  return 0;
}





