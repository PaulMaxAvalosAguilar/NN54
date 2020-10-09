#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32g431xx.h"
#include "LCD/lcd.h"
#include "main.h"
#include "CUSTOM/itoa.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//UART RX------------------------------------
char receiveBuffer[UART_RX_BUFFER_LEN] = {0};

//Queue and Semaphore handles----------------
SemaphoreHandle_t adcSemaphore;
SemaphoreHandle_t encoderSemaphore;
SemaphoreHandle_t uartRXSemaphore;
QueueHandle_t uartTXQueue;
QueueHandle_t lcdQueue;

static volatile uint32_t counter = 0;
uint32_t bluetoothConnected = 0;
uint32_t minDistToTravel = 0;
uint32_t desiredCounterDirection = 0;
uint32_t desiredRepDir = 0;


void USART1_IRQHandler(void);
void TIM2_IRQHandler(void);
void EXTI15_10_IRQHandler(void);


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

void setBLEConnected(uint8_t boolean){
  taskENTER_CRITICAL();
  bluetoothConnected = boolean;
  taskEXIT_CRITICAL();
}

uint8_t getBLEConnected(){
  uint8_t bleConnectionStatus;
  
  taskENTER_CRITICAL();
  bleConnectionStatus = bluetoothConnected;
  taskEXIT_CRITICAL();

  return bleConnectionStatus;
}

void printString(const char myString[]){
  DMA1_Channel2->CCR &= ~DMA_CCR_EN;//Channel disable
  DMA1_Channel2->CMAR = (uint32_t)myString;//DMA source address
  DMA1_Channel2->CNDTR = strlen(myString);//DMA length
  DMA1_Channel2->CCR |= DMA_CCR_EN;//Channel enable
  while(!(DMA1->ISR & DMA_ISR_TCIF2));//Wait till transfere complete
  DMA1->IFCR |= DMA_IFCR_CTCIF2;//Clear transfere complete  
}

static inline void initializeTimers(void){
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;//Enable TIM2 clock
}

static inline void stopTimers(void){
  RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;//Enable TIM2 clock
}

static inline void encodeTwoBytes(char *twoByteBuffer, uint32_t numberToEncode){
  static uint8_t lowPart = 0;
  static uint8_t highPart = 0;
  
  lowPart = ((numberToEncode & 0x7F) << 1) | 1;
  highPart = (numberToEncode >>6) | 1;
  twoByteBuffer[0] = highPart;
  twoByteBuffer[1] = lowPart;
  twoByteBuffer[2] = 0;
}

static void uartRXTask(void *args __attribute__((unused))){

  uint32_t receiveBufferPos = 0;

  for(;;){
    
    xSemaphoreTake(uartRXSemaphore,portMAX_DELAY); //Should go first
    
    while(receiveBuffer[receiveBufferPos]){
      if((receiveBuffer[receiveBufferPos] == '%') ||
	 receiveBuffer[receiveBufferPos] == '|'){
	char secondToken = receiveBuffer[receiveBufferPos];
	char parseBuffer[100] = {0};
	int parseBufferPos = 0;
	receiveBuffer[receiveBufferPos] = 0;
	receiveBufferPos = (receiveBufferPos + 1) % UART_RX_BUFFER_LEN;

      ENTER:
	while(!receiveBuffer[receiveBufferPos]);
	if(receiveBuffer[receiveBufferPos] != secondToken){
	  parseBuffer[parseBufferPos++] = receiveBuffer[receiveBufferPos];
	  receiveBuffer[receiveBufferPos] = 0;
	  receiveBufferPos = (receiveBufferPos + 1) % UART_RX_BUFFER_LEN;
	}else{
	  goto EXIT;
	}
	goto ENTER;
      EXIT:

	parseBuffer[parseBufferPos++]= 0;

	//INTERPRET
	if(strncmp(parseBuffer, "CONNECT",7) == 0){
	  setBLEConnected(1);
	  sendToLCDQueue(connectedStatus, 1);
	}else if(strncmp(parseBuffer, "DISCONNECT", 10) == 0){
	  sendToLCDQueue(connectedStatus, 0);
	  stopTimers();
	  setBLEConnected(0);
	  xSemaphoreGive(adcSemaphore);//Should go after setBLEConnected(0)
	}else if(secondToken == '|'){

	  uint8_t messageType = parseBuffer[0];
	  if(messageType == 1){
	    minDistToTravel = (parseBuffer[2]>>1) | ((parseBuffer[1] & 0xFE)<<6);
	    desiredCounterDirection = parseBuffer[3]-1;
	    desiredRepDir = parseBuffer[4]-1;

	    initializeTimers();
	    xSemaphoreGive(encoderSemaphore);
	    sendToUARTTXQueue(encoderStart,0,0,0);
	    
	  }else if(messageType == 2){
	    stopTimers();
	    sendToUARTTXQueue(encoderStop,0,0,0);
	  }else if(messageType == 3){
	    xSemaphoreGive(adcSemaphore);
	  }	  
	}

	receiveBuffer[receiveBufferPos] = 0;
	receiveBufferPos = (receiveBufferPos + 1) % UART_RX_BUFFER_LEN;
      }else{
	
	receiveBuffer[receiveBufferPos] = 0;
	receiveBufferPos = (receiveBufferPos + 1) % UART_RX_BUFFER_LEN;
      }
    }
  }
}

static void uartTXTask(void *args __attribute__((unused))){

  uartTXData_t receivedData;
  char buffer[20];
  char twoByteBuffer[3];

  
  for(;;){
    xQueueReceive(uartTXQueue, &receivedData, portMAX_DELAY);
    
    strcpy(buffer,"|");
      
    if(receivedData.messageType == encoderData){
      twoByteBuffer[0] = 129;
      twoByteBuffer[1] = 0;
      strcat(buffer, twoByteBuffer);
      
      encodeTwoBytes(twoByteBuffer, receivedData.traveledDistanceOrADC);
      strcat(buffer, twoByteBuffer);

      encodeTwoBytes(twoByteBuffer, receivedData.meanPropulsiveVelocity);
      strcat(buffer, twoByteBuffer);

      encodeTwoBytes(twoByteBuffer, receivedData.peakVelocity);
      strcat(buffer, twoByteBuffer);

    }else if(receivedData.messageType == encoderStart){
      twoByteBuffer[0] = 130;
      twoByteBuffer[1] = 0;
      strcat(buffer, twoByteBuffer);
      
    }else if(receivedData.messageType == batteryLevel){

      twoByteBuffer[0] = 131;
      twoByteBuffer[1] = 0;
      strcat(buffer, twoByteBuffer);

      encodeTwoBytes(twoByteBuffer, receivedData.traveledDistanceOrADC);
      strcat(buffer, twoByteBuffer);
      
    }else if(receivedData.messageType == encoderStop){
      twoByteBuffer[0] = 132;
      twoByteBuffer[1] = 0;
      strcat(buffer, twoByteBuffer);
      
    }

    strcat(buffer,"|");
    printString(buffer);
  }
}

static void lcdTask(void *args __attribute__((unused))){

  lcdData_t receivedData;
  char buffer[22];
  char numBuffer[5];

  uint16_t lineState = 0;
  uint32_t lastVoltage = 0;

  lcd_init();
  lcdPutsBlinkFree("  Paul's Inventions   ",0);

  
  for(;;){

    xQueueReceive(lcdQueue,&receivedData, portMAX_DELAY);

    if(receivedData.messageType == connectedStatus){//Connected status message
      lcdPutsBlinkFree(receivedData.displayValue?
		       "Connected":"         ",
		       2);

    }else if(receivedData.messageType == batteryLevel){//Battery level message
      lineState = GPIOB->IDR;
      if(lineState & GPIO_IDR_ID12){
	strcpy(buffer, "    ** Batt: ");
      }else{
	strcpy(buffer, "       Batt: ");
      }

      itoa(receivedData.displayValue,numBuffer,10);
      strcat(buffer, numBuffer);
      lcdPutsBlinkFree(buffer, 7);
      
      lastVoltage = receivedData.displayValue;//save value

    }else if(receivedData.messageType == chargingStatus){//Charging status message
      if(receivedData.displayValue){
	strcpy(buffer, "    ** Batt: ");
      }else{
	strcpy(buffer, "       Batt: ");
      }

      itoa(lastVoltage,numBuffer,10);
      strcat(buffer, numBuffer);
      lcdPutsBlinkFree(buffer, 7);
      
    }
  }    
}


static void adcTask(void *args __attribute__((unused))){
  
  uint32_t adcData = 0;

  for(;;){
    
    taskENTER_CRITICAL();
    ADC1->CFGR2 |= ADC_CFGR2_BULB;//Bulb sampling
    ADC1->CFGR2 |= (ADC1->CFGR2 & (~ADC_CFGR2_OVSS)) | (0b1000 << ADC_CFGR2_OVSS_Pos);//Shift 8 bits
    ADC1->CFGR2 |= (ADC1->CFGR2 & (~ADC_CFGR2_OVSR)) | (0b111 << ADC_CFGR2_OVSR_Pos);//Oversampling ratio 256x
    ADC1->CFGR2 &= ~ADC_CFGR2_ROVSE;//Regular oversampling disabled

    ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_L)) | (0b0000 << ADC_SQR1_L_Pos);//1 conversion
    ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_SQ1)) | (18 << ADC_SQR1_SQ1_Pos);//1st conversion on IN18
    ADC1->SMPR2 = (ADC1->SMPR2 & (~ADC_SMPR2_SMP18)) | (0b000 << ADC_SMPR2_SMP18_Pos);//Sample time 2.5 clock cycles
    
    ADC1->CR |= ADC_CR_ADSTART;//Start conversion
    while(!(ADC1->ISR & ADC_ISR_EOC));//Wait till conversion finished
    adcData = ADC1->DR;//Read data register and clear EOC flag
    while(ADC1->CR & ADC_CR_ADSTART);

    ADC1->CFGR2 |= ADC_CFGR2_ROVSE;//Regular oversampling enabled
    
    ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_L)) | (0b0000 << ADC_SQR1_L_Pos);//1 conversion
    ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_SQ1)) | (5 << ADC_SQR1_SQ1_Pos);//1st conversion on IN5
    ADC1->SMPR1 = (ADC1->SMPR1 & (~ADC_SMPR1_SMP5)) | (0b111 << ADC_SMPR1_SMP5_Pos);//Sample time 640.5 clock cycles
    
    ADC1->CR |= ADC_CR_ADSTART;//Start conversion
    taskEXIT_CRITICAL();
    
    while(!(ADC1->ISR & ADC_ISR_EOC));//Wait till conversion finished
    adcData = ADC1->DR;//Read data register and clear EOC flag
    while(ADC1->CR & ADC_CR_ADSTART);

    adcData = (adcData * 6104/10000) * 2;

    sendToLCDQueue(batteryLevel, adcData);
    sendToUARTTXQueue(batteryLevel, (uint16_t)adcData, 0,0);

    if(getBLEConnected()){
      xSemaphoreTake(adcSemaphore,portMAX_DELAY);      
    }else{
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}

/*

  float angle = 75.1;
  float angleRad = degToRads(angle);
  float angleDiv = (angleRad/PI) * BITS31;

  float cosine;
  float sine;


  CORDIC->WDATA = (int32_t)angleDiv;
  cosine = (CORDIC->RDATA)/BITS31;//Read first result
  sine = (CORDIC->RDATA)/BITS31;//Read second result

  itoa((int32_t)(cosine*1000000), buffer, 10);
  lcdPutsBlinkFree(buffer,3);
    
  itoa((int32_t)(sine*1000000), buffer, 10);
  lcdPutsBlinkFree(buffer, 5);
*/

int main(void)
{
  //OPTION BYTES
  //PG10 reset Input/Output
  //nBOOT0 = 1
  //nSWBOOT0  = 1 (BOOT0 taken from PB8/BOOT0 pin)
  //SRAM1 and CCM SRAM parity check disable
  //nBOOT1 = 1
  //Software window watchdog
  //Independent watchdog counter is running in stop mode
  //Software independent watchdog
  //No reset generated when entering the Stop mode
  //BOR_LEV threshold 1.7
  //Read protection not active
  


  //---------------------INCREASE CPU SPEED-------------------------

  //The HSI16 is used as system clock source after startup from Reset
  //Flash erase and programming is only possible in voltage scale range 1

  //PWR_CR1 Low power run not set, Voltage scaling range 1, low power mode= Stop 0 mode
  //PWR_CR5 Main regulator in range 1 normal mode
  //FLASH_ACR Instruction cache and data cache enabled, zero wait state*
  //RCC_CR All clocks off and not ready(hsi*), HSE not bypassed
  //RCC_CFGR HSI16 selected as system clock, sysclk & PCLK1 & PCLK2 not divided, MCO output disabled 
  //RCC_PLLCFGR PLLn mult by 8, PLLm div by 1, no PLL sourcce, PLLR div by 2, PLLR disabled    


  //Wait states for less than 90 MHz at VCore Range 1 normal mode
  FLASH->ACR |= FLASH_ACR_PRFTEN;
  FLASH->ACR = (FLASH->ACR & (~FLASH_ACR_LATENCY)) | FLASH_ACR_LATENCY_1WS; //1 wait states
    
  //Turn HSE and wait till is ready
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR & (RCC_CR_HSERDY)));

  //Turn PLL on, configure it and wait till ready
  RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN))
    | (0b01 << RCC_PLLCFGR_PLLM_Pos) | (25 << RCC_PLLCFGR_PLLN_Pos); //M = 2, N = 25 50 Mhz SYSCLK
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;    
    
  RCC->CR |= RCC_CR_PLLON;
  while(!RCC_CR_PLLRDY);

  RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    
  //Select PLL as system clocksource and wait till selected
  RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_SW_PLL)) | (RCC_CFGR_SW_PLL);
  while( ((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

  //-----------------Enable independent clocks-----------------
  
  RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_USART1SEL)) | (0b01 << RCC_CCIPR_USART1SEL_Pos);//System clock as USART1 clock
  RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_I2C2SEL)) | (0b01 << RCC_CCIPR_I2C2SEL_Pos);//System clock as I2C2 clock
  RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_LPTIM1SEL)) | (0b00 << RCC_CCIPR_LPTIM1SEL_Pos);//PCLK selected as LPTIM1 clock
  RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_ADC12SEL)) | (0b10 <<RCC_CCIPR_ADC12SEL_Pos);//Sysclock selected as ADC 1 & 2 clock

  //---------------------CONFIGURE GPIO---------------------
  
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

  /*
  //TEST PINS CONFIGURATION
  //PA8 
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE8)) | (0b01 << GPIO_MODER_MODE8_Pos); //General Purpose Output mode
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED8)) | (0b00 << GPIO_OSPEEDR_OSPEED8_Pos); //Low speed
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT8; //Push pull
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD8)) | (0b00 << GPIO_PUPDR_PUPD8_Pos); //No Pull up down
  GPIOA->BSRR |= GPIO_BSRR_BS8;//Turn on PA8
  
  //PA9 
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE9)) | (0b01 << GPIO_MODER_MODE9_Pos); //General Purpose Output mode
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED9)) | (0b00 << GPIO_OSPEEDR_OSPEED9_Pos); //Low speed
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT9; //Push pull
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD9)) | (0b00 << GPIO_PUPDR_PUPD9_Pos); //No Pull up down
  GPIOA->BSRR |= GPIO_BSRR_BS9;//Turn on PA9
  */

  //I2C GPIO CONFIGURATION
  //PA8 I2C2_SDA
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE8)) | (0b10 << GPIO_MODER_MODE8_Pos); //Alternate function mode
  GPIOA->OTYPER |= GPIO_OTYPER_OT8; //Open drain
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED8)) | (0b00 << GPIO_OSPEEDR_OSPEED8_Pos); //Low speed
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD8)) | (0b01 << GPIO_PUPDR_PUPD8_Pos); //Pull up
  GPIOA->AFR[1] = (GPIOA->AFR[1] & (~GPIO_AFRH_AFSEL8)) | (4 << GPIO_AFRH_AFSEL8_Pos); //Alternate function 4 

  //PA9 I2C2_SCL
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE9)) | (0b10 << GPIO_MODER_MODE9_Pos); //Alternate function mode
  GPIOA->OTYPER |= GPIO_OTYPER_OT9; //Open drain
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED9)) | (0b00 << GPIO_OSPEEDR_OSPEED9_Pos); //Low speed
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD9)) | (0b01 << GPIO_PUPDR_PUPD9_Pos); //Pull up
  GPIOA->AFR[1] = (GPIOA->AFR[1] & (~GPIO_AFRH_AFSEL9)) | (4 << GPIO_AFRH_AFSEL9_Pos); //Alternate function 4 

  //UART1 GPIO CONFIGURATION
  //PA10 UART1_RX
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE10)) | (0b10 << GPIO_MODER_MODE10_Pos) ; //Alternate function mode
  GPIOA->OTYPER &= (~GPIO_OTYPER_OT10);//Push pull  
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED10)) | (0b00 << GPIO_OSPEEDR_OSPEED10_Pos); //Low speed
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD10)) | (0b00 << GPIO_PUPDR_PUPD10_Pos); //No pull up, no pull down
  GPIOA->AFR[1] = (GPIOA->AFR[1] & (~GPIO_AFRH_AFSEL10)) | (7 << GPIO_AFRH_AFSEL10_Pos); //Alternate function 7 

  //PB6 UART1_TX
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE6)) | (0b10 << GPIO_MODER_MODE6_Pos) ; //Alternate function mode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT6);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED6)) | (0b00 << GPIO_OSPEEDR_OSPEED6_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD6)) | (0b00 << GPIO_PUPDR_PUPD6_Pos); //No pull up, no pull down
  GPIOB->AFR[0] = (GPIOB->AFR[0] & (~GPIO_AFRL_AFSEL6)) | (7 << GPIO_AFRL_AFSEL6_Pos); //Alternate function 7 

  //LPTIM1 GPIO CONFIGURATION
  //PB5 LPTIM_IN1
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE5)) | (0b10 << GPIO_MODER_MODE5_Pos) ; //Alternate function mode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT5);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED5)) | (0b00 << GPIO_OSPEEDR_OSPEED5_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD5)) | (0b00 << GPIO_PUPDR_PUPD5_Pos); //No pull up, no pull down
  GPIOB->AFR[0] = (GPIOB->AFR[0] & (~GPIO_AFRL_AFSEL5)) | (11 << GPIO_AFRL_AFSEL5_Pos); //Alternate function 11 

  //PB7 LPTIM_IN2
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE7)) | (0b10 << GPIO_MODER_MODE7_Pos) ; //Alternate function mode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT7);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED7)) | (0b00 << GPIO_OSPEEDR_OSPEED7_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD7)) | (0b00 << GPIO_PUPDR_PUPD7_Pos); //No pull up, no pull down
  GPIOB->AFR[0] = (GPIOB->AFR[0] & (~GPIO_AFRL_AFSEL7)) | (11 << GPIO_AFRL_AFSEL7_Pos); //Alternate function 11


  //TIM2 GPIO CONFIGURATION
  //PA15 TIM2_CH1
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE15)) | (0b10 << GPIO_MODER_MODE15_Pos) ; //Alternate function mode
  GPIOA->OTYPER &= (~GPIO_OTYPER_OT15);//Push pull  
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED15)) | (0b00 << GPIO_OSPEEDR_OSPEED15_Pos); //Low speed
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD15)) | (0b00 << GPIO_PUPDR_PUPD15_Pos); //No pull up, no pull down
  GPIOA->AFR[1] = (GPIOA->AFR[1] & (~GPIO_AFRH_AFSEL15)) | (1 << GPIO_AFRH_AFSEL15_Pos); //Alternate function 1

  //PB3 TIM2_CH2
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE3)) | (0b10 << GPIO_MODER_MODE3_Pos) ; //Alternate function mode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT3);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED3)) | (0b00 << GPIO_OSPEEDR_OSPEED3_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD3)) | (0b00 << GPIO_PUPDR_PUPD3_Pos); //No pull up, no pull dowxn
  GPIOB->AFR[0] = (GPIOB->AFR[0] & (~GPIO_AFRL_AFSEL3)) | (1 << GPIO_AFRL_AFSEL3_Pos); //Alternate function 11

  //ENC ENABLE CONFIGURATION
  //PB9 
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE9)) | (0b01 << GPIO_MODER_MODE9_Pos) ; //General purpose outputMode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT9);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED9)) | (0b00 << GPIO_OSPEEDR_OSPEED9_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD9)) | (0b00 << GPIO_PUPDR_PUPD9_Pos); //No pull up, no pull down
  GPIOB->BSRR |= GPIO_BSRR_BR9;//Turn on encoder

  //ENC CONNECTED CONFIGURATION
  //PB12
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE12)) | (0b00 << GPIO_MODER_MODE12_Pos) ; //Input mode
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD12)) | (0b10 << GPIO_PUPDR_PUPD12_Pos); //Pull down

  //ANALOG CONFIGURATION
  //PB14
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE14)) | (0b11 << GPIO_MODER_MODE14_Pos) ; //Analog mode
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD14)) | (0b00 << GPIO_PUPDR_PUPD14_Pos); //No pull up, no pull down


  //---------------------CONFIGURE I2C-------------------------

  RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN; //Enable I2C2 clock

  //I2C2 CONFIGURATION
  I2C2->CR1 &= ~I2C_CR1_ANFOFF; //Aanalog noise filter enabled
  I2C2->CR1 &= ~I2C_CR1_NOSTRETCH; //Clock stretching enabled
  I2C2->CR1 |= I2C_CR1_TXDMAEN;//Enable DMA transmit
  I2C2->TIMINGR = 0x00401959 & 0xF0FFFFFFU; //Set timings 400kz
  I2C2->CR2 &= ~I2C_CR2_ADD10;//The master operatines in 7 bit addressing mode
  I2C2->CR1 |= I2C_CR1_PE; //Enable periphereal

  //---------------------CONFIGURE UART-------------------------

  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Enable UART1 clock
  
  //UART1 CONFIGURATION
  USART1->CR1 &= ~USART_CR1_FIFOEN;//FIFO mode disabled
  USART1->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);//1 start bit, 8 data bits
  USART1->CR1 &= ~USART_CR1_OVER8;//Oversampling by 16
  USART1->CR1 &= ~USART_CR1_PCE;//No parity
  USART1->CR1 |= USART_CR1_IDLEIE;//IDLE interrupt enable
  USART1->CR1 |= USART_CR1_TE;//Enable transmiter
  USART1->CR1 |= USART_CR1_RE;//Enable receiver
  USART1->CR2 = (USART1->CR2 & ~(USART_CR2_STOP)) | (0b00 << USART_CR2_STOP_Pos);//1 Stop bit
  USART1->CR3 &= ~USART_CR3_ONEBIT;//Three sample bit method
  USART1->CR3 &= ~USART_CR3_CTSE;//CTS disabled
  USART1->CR3 &= ~USART_CR3_RTSE;//RTS disabled
  USART1->CR3 |= USART_CR3_OVRDIS;//Overrun Disable
  USART1->CR3 |= USART_CR3_DMAT;//Enable DMA transmit
  USART1->CR3 |= USART_CR3_DMAR;//Enable DMA receive
  USART1->PRESC = (USART1->PRESC & (~USART_PRESC_PRESCALER)) | (0b0000 << USART_PRESC_PRESCALER_Pos);//Input clock not divided
  USART1->BRR = 434;//50,000,000/ 434 = 115,200
  USART1->CR1 |= USART_CR1_UE;//Enable USART
  

  //-------------------CONFIGURE DMA---------------------------

  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//Enable DMA1 clock
  RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;//Enable DMA multiplexer clock


  //DMA UART1 RX CONFIGURATION
  DMA1_Channel1->CCR &= ~DMA_CCR_DIR; //read from periphereal
  DMA1_Channel1->CCR = (DMA1_Channel1->CCR & (~DMA_CCR_PL)) | (0b10 << DMA_CCR_PL_Pos); //High Priority level
  DMA1_Channel1->CCR |= DMA_CCR_CIRC;//circular mode enabled
  DMA1_Channel1->CNDTR = UART_RX_BUFFER_LEN;//DMA length

  DMA1_Channel1->CPAR = (uint32_t)&USART1->RDR;//DMA source address
  DMA1_Channel1->CCR &= ~(DMA_CCR_PINC);//Periphereal increment mode disabled
  DMA1_Channel1->CCR = (DMA1_Channel1->CCR &= (~DMA_CCR_PSIZE)) | (0b00 << DMA_CCR_PSIZE_Pos); //Perihphereal size 8 bits

  DMA1_Channel1->CMAR = (uint32_t)receiveBuffer;//DMA destination address
  DMA1_Channel1->CCR |= (DMA_CCR_MINC);//Memory increment mode enabled
  DMA1_Channel1->CCR = (DMA1_Channel1->CCR &= (~DMA_CCR_MSIZE)) | (0b00 << DMA_CCR_MSIZE_Pos); //Memory size 8 bits

  DMAMUX1_Channel0->CCR = (DMAMUX1_Channel0->CCR & (~DMAMUX_CxCR_DMAREQ_ID)) | (24 << DMAMUX_CxCR_DMAREQ_ID_Pos);//Requppest 24

  DMA1_Channel1->CCR |= DMA_CCR_EN;//Channel enable

  //DMA UART1 TX CONFIGURATION
  DMA1_Channel2->CCR |= DMA_CCR_DIR; //read from memory
  DMA1_Channel2->CCR = (DMA1_Channel2->CCR & (~DMA_CCR_PL)) | (0b01 << DMA_CCR_PL_Pos); //Medium Priority level
  DMA1_Channel2->CCR &= ~(DMA_CCR_CIRC);//circular mode disabled

  DMA1_Channel2->CPAR = (uint32_t)&USART1->TDR;//DMA destination address
  DMA1_Channel2->CCR &= ~(DMA_CCR_PINC);//Periphereal increment mode disabled
  DMA1_Channel2->CCR = (DMA1_Channel2->CCR &= (~DMA_CCR_PSIZE)) | (0b00 << DMA_CCR_PSIZE_Pos); //Perihphereal size 8 bits

  DMA1_Channel2->CCR |= (DMA_CCR_MINC);//Memory increment mode enabled
  DMA1_Channel2->CCR = (DMA1_Channel2->CCR &= (~DMA_CCR_MSIZE)) | (0b00 << DMA_CCR_MSIZE_Pos); //Memory size 8 bits

  DMAMUX1_Channel1->CCR = (DMAMUX1_Channel1->CCR & (~DMAMUX_CxCR_DMAREQ_ID)) | (25 << DMAMUX_CxCR_DMAREQ_ID_Pos);//Request 25

  //DMA I2C2 TRANSMIT CONFIGURATION
  DMA1_Channel3->CCR |= DMA_CCR_DIR; //read from memory
  DMA1_Channel3->CCR = (DMA1_Channel3->CCR & (~DMA_CCR_PL)) | (0b00 << DMA_CCR_PL_Pos); //Low Priority level
  DMA1_Channel3->CCR &= ~(DMA_CCR_CIRC);//circular mode disabled

  DMA1_Channel3->CPAR = (uint32_t)&I2C2->TXDR;//DMA destination address
  DMA1_Channel3->CCR &= ~(DMA_CCR_PINC);//Periphereal increment mode disabled
  DMA1_Channel3->CCR = (DMA1_Channel3->CCR &= (~DMA_CCR_PSIZE)) | (0b00 << DMA_CCR_PSIZE_Pos); //Perihphereal size 8 bits

  DMA1_Channel3->CCR |= (DMA_CCR_MINC);//Memory increment mode enabled
  DMA1_Channel3->CCR = (DMA1_Channel3->CCR &= (~DMA_CCR_MSIZE)) | (0b00 << DMA_CCR_MSIZE_Pos); //Memory size 8 bits

  DMAMUX1_Channel2->CCR = (DMAMUX1_Channel2->CCR & (~DMAMUX_CxCR_DMAREQ_ID)) | (19 << DMAMUX_CxCR_DMAREQ_ID_Pos);//Request 19

  //---------------------CONFIGURE LPTIM-------------------------

  RCC->APB1ENR1 |= RCC_APB1ENR1_LPTIM1EN;//Enable LPTIM1 clock

  //LPTIM1 CONFIGURATION
  LPTIM1->CFGR = (LPTIM1->CFGR & (~LPTIM_CFGR_CKFLT)) | (0b11 << LPTIM_CFGR_CKFLT_Pos);//8 clock periods for valid transition
  LPTIM1->CFGR &= ~LPTIM_CFGR_CKSEL;////LPTIM clocked by internal clock source
  LPTIM1->CFGR = (LPTIM1->CFGR & (~LPTIM_CFGR_CKPOL)) | (0b10 << LPTIM_CFGR_CKPOL_Pos);//both edges are active edges
  LPTIM1->CFGR |= LPTIM_CFGR_COUNTMODE;//External input 1
  LPTIM1->CFGR |= LPTIM_CFGR_ENC;//Encoder mode enable
  LPTIM1->CR |= LPTIM_CR_ENABLE;//LPTIM1 Enable
  LPTIM1->ICR |= LPTIM_ICR_ARROKCF;
  LPTIM1->ARR = 65535;
  while(!(LPTIM1->ISR & LPTIM_ISR_ARROK));
  LPTIM1->CR |= LPTIM_CR_CNTSTRT; //Timer start in Continuous mode

  //---------------------CONFIGURE TIM2-------------------------

  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;//Enable TIM2 clock

  //TIM2 CONFIGURATION

  TIM2->TISEL = (TIM2->TISEL & (~TIM_TISEL_TI1SEL)) | (0b0000 << TIM_TISEL_TI1SEL_Pos);//tim_ti1 input on TIMx_CH1
  TIM2->TISEL = (TIM2->TISEL & (~TIM_TISEL_TI2SEL)) | (0b0000 << TIM_TISEL_TI2SEL_Pos);//tim_ti2 input on TIMx_CH2
  TIM2->CCER |= (TIM_CCER_CC1NP | TIM_CCER_CC1P);//edge selection both edges for tim_ti1 source
  //TIM2->CCER |= (TIM_CCER_CC2NP | TIM_CCER_CC2P);//edge selection both edges for tim_ti2 source
  TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_IC1F)) | (0b0011 << TIM_CCMR1_IC1F_Pos);//Tim_ti1 filtered for 8 clock cycles
  //TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_IC2F)) | (0b0011 << TIM_CCMR1_IC2F_Pos);//Tim_ti2 filtered for 8 clock cycles
  TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_CC1S)) | (0b01 << TIM_CCMR1_CC1S_Pos);//(input mode)Tim_ic1 mapping on tim_ti1 
  //TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_CC2S)) | (0b01 << TIM_CCMR1_CC2S_Pos);//(input mode)Tim_ic2 mapping on tim_ti2
  TIM2->CCER |= (TIM_CCER_CC1E);//Capture enabled for Capture register 1 and 2
  
  TIM2->CR2 |= TIM_CR2_TI1S;// tim_ti1 and tim_ti2 inputs XORed on tim_ti1
  TIM2->DIER = TIM_DIER_CC1IE;//Enable Interrupts
  TIM2->ARR = 0XFFFFFFFF;//Preescaler
  TIM2->PSC = 4;//Preescaler / actual value = TIM2->PSC + 1 
  TIM2->CNT = 0XFFFFFFFF;//For PSC value to be accounted inmediatly after update
  TIM2->CR1 |= TIM_CR1_CEN; //Start tim2Counter

  RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;//Disable TIM2 clock  

  //---------------------CONFIGURE EXTI-------------------------
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;//Enable SYSCFG clock

  SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & (~SYSCFG_EXTICR4_EXTI12)) | (0b0001 << SYSCFG_EXTICR4_EXTI12_Pos);

  EXTI->IMR1 |= 1 << EXTI_IMR1_IM12_Pos;//Enable Exti 12
  EXTI->RTSR1 |= 1 << EXTI_RTSR1_RT12_Pos;//Enable Exti 12 rising trigger
  EXTI->FTSR1 |= 1 << EXTI_FTSR1_FT12_Pos;//Enable Exti 12 falling trigger  

  //---------------------CONFIGURE PWR---------------------------

  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;//Enable PWR clock

  //---------------------CONFIGURE ADC---------------------------

  
  RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;//Enable ADC12 clock
  ADC12_COMMON->CCR = (ADC12_COMMON->CCR & (~ADC_CCR_CKMODE)) | (0b00 << ADC_CCR_CKMODE_Pos);//Adc generated at product level
  ADC12_COMMON->CCR = (ADC12_COMMON->CCR & (~ADC_CCR_PRESC)) | (0b0000 << ADC_CCR_PRESC_Pos);//Input ADC clock not divided
  ADC12_COMMON->CCR |= ADC_CCR_VREFEN;//Vrefint enable

  
  VREFBUF->CSR = (VREFBUF->CSR & (~VREFBUF_CSR_VRS)) | (0b01 << VREFBUF_CSR_VRS_Pos);//Voltage reference set to 2.5V
  VREFBUF->CSR &= ~VREFBUF_CSR_HIZ;//Disable High Impedance
  VREFBUF->CSR |= VREFBUF_CSR_ENVR;//Enable VREFBUF
  while(!(VREFBUF->CSR & VREFBUF_CSR_VRR));//Wait till VREFBUF ready


  //ADC1 CONFIGURATION
  ADC1->CR &= ~ADC_CR_DEEPPWD;//Exit deep power down mode
  ADC1->CR |= ADC_CR_ADVREGEN;//Enable ADC internal voltage regulator
  for(int i = 0; i<500; i++);//TADCVREG_STUP 20us wait

  //  ADC1->CR |= ADC_CR_DEEPPWD;//Writing DEEPPWD=1 automatically disables the ADC voltage regulator and bit ADVREGEN
  ADC1->CR &= ~ADC_CR_ADCALDIF;//Calibration in single ended mode
  ADC1->CR |= ADC_CR_ADCAL;//Initiate calibration
  while(ADC1->CR & ADC_CR_ADCAL);//Wait while calibratoin is in progress

  ADC1->CR |= ADC_CR_ADCALDIF;//Calibration in differential mode
  ADC1->CR |= ADC_CR_ADCAL;//Initiate calibration
  while(ADC1->CR & ADC_CR_ADCAL);//Wait while calibratoin is in progress

  ADC1->DIFSEL |= ADC_DIFSEL_DIFSEL_1;//IN1 Differential
  ADC1->DIFSEL |= ADC_DIFSEL_DIFSEL_3;//IN3 Differential
  ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_5;//IN5 Single ended
  ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_18;//IN18 Single ended

  ADC1->ISR |= ADC_ISR_ADRDY;//Clear ADRDY bit
  ADC1->CR |= ADC_CR_ADEN;//Enable ADC1;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));//Wait till ADC is ready

  
  //ADC2 CONFIGURATION
  ADC2->CR &= ~ADC_CR_DEEPPWD;//Exit deep power down mode
  ADC2->CR |= ADC_CR_ADVREGEN;//Enable ADC internal voltage regulator
  for(int i = 0; i<500; i++);//TADCVREG_STUP 20us wait

  ADC2->CR |= ADC_CR_ADCALDIF;//Calibration in differential mode
  ADC2->CR |= ADC_CR_ADCAL;//Initiate calibration
  while(ADC2->CR & ADC_CR_ADCAL);//Wait while calibration is in progress

  ADC2->DIFSEL |= ADC_DIFSEL_DIFSEL_3;//IN3 Differential
  ADC2->DIFSEL |= ADC_DIFSEL_DIFSEL_12;//IN12 Differential

  ADC1->CR |= ADC_CR_ADEN;//Enable ADC2;
  //---------------------CONFIGURE CORDIC------------------------

  RCC->AHB1ENR |= RCC_AHB1ENR_CORDICEN;

  //CORDIC CONFIGURATION
  CORDIC->CSR = (CORDIC->CSR & (~CORDIC_CSR_FUNC)) | (0 << CORDIC_CSR_FUNC_Pos);//Cosine function selected
  CORDIC->CSR &= ~CORDIC_CSR_ARGSIZE;//Arg width 32 bit
  CORDIC->CSR &= ~CORDIC_CSR_RESSIZE;//Arg width 32 bit
  CORDIC->CSR &= ~CORDIC_CSR_NARGS;//Only one 32 bit write
  CORDIC->CSR |= CORDIC_CSR_NRES;//Two results, two reads necessary
  
  //---------------------CONFIGURE RTOS-------------------------

  adcSemaphore = xSemaphoreCreateBinary();
  encoderSemaphore = xSemaphoreCreateBinary();
  uartRXSemaphore = xSemaphoreCreateBinary();
  uartTXQueue = xQueueCreate(TX_QUEUE_SIZE, sizeof(uartTXData_t));
  lcdQueue = xQueueCreate(LCD_QUEUE_SIZE, sizeof(lcdData_t));

  xTaskCreate(uartRXTask, "uartRXTask",100, NULL, 3, NULL);
  xTaskCreate(uartTXTask, "uartTXTask",100, NULL, 2, NULL);
  xTaskCreate(lcdTask,"lcdTask",100, NULL, 2, NULL);
  xTaskCreate(adcTask,"adcTask",100,NULL,1,NULL);

  //---------------------CONFIGURE NVIC---------------------------

  NVIC_SetPriorityGrouping(0); //4 bits for pre-emption 0 bit for subpriority

  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));
  
  //  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_EnableIRQ(USART1_IRQn);

  //---------------------START RTOS-------------------------------
  vTaskStartScheduler();
  
  while (1)
  {

    /*
    uint32_t count=LPTIM1->CNT;

    sprintf(buffer, "%lu", counter);
    lcdPutsBlinkFree(buffer,2);

    sprintf(buffer, "%lu", charging);
    lcdPutsBlinkFree(buffer,3);

    intoa(count,buffer,10);
    lcdPutsBlinkFree(buffer,5);
    */


    /*
    for(int i = 1; i < 3000000;i++);
    sprintf(buffer, "1");
    lcdPutsBlinkFree(buffer,3);

    for(int i = 1; i < 3000000;i++);
    sprintf(buffer, "2");
    lcdPutsBlinkFree(buffer,3);

    for(int i = 1; i < 3000000;i++);
    sprintf(buffer, "3");
    lcdPutsBlinkFree(buffer,3);

    for(int i = 1; i < 3000000;i++);
    sprintf(buffer, "4");
    lcdPutsBlinkFree(buffer,3);

    for(int i = 1; i < 3000000;i++);
    sprintf(buffer, "5");
    lcdPutsBlinkFree(buffer,3);

    __asm volatile( "cpsid i" ::: "memory" );
    __asm volatile( "dsb" );
    __asm volatile( "isb" );
    RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_LPTIM1SEL)) | (0b00 << RCC_CCIPR_LPTIM1SEL_Pos);//PCLK selected as LPTIM1 clock
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_HPRE)) | (0b1110 << RCC_CFGR_HPRE_Pos);//CPU freq 195.312 Khz
    PWR->CR1 |= PWR_CR1_LPR;
    while(!(PWR->SR2 & PWR_SR2_REGLPS));//Wait till low power regulator started
    while(!(PWR->SR2 & PWR_SR2_REGLPF));//Wait till regulator is in low power mode
    printString("\x31""Hola\n");
    lcdPutsBlinkFree("9",3);
    __WFI();
    PWR->CR1 &= ~PWR_CR1_LPR;
    while((PWR->SR2 & PWR_SR2_REGLPF));
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_HPRE)) | (0b0000 << RCC_CFGR_HPRE_Pos);//CPU freq 50 mhz
    RCC->CCIPR = (RCC->CCIPR & (~RCC_CCIPR_LPTIM1SEL)) | (0b10 << RCC_CCIPR_LPTIM1SEL_Pos);//PCLK selected as LPTIM1 clock
    printString("\x31""Adios\n");
    lcdPutsBlinkFree("10",3);
    __asm volatile( "cpsie i" ::: "memory" );
    __asm volatile( "dsb" );
    __asm volatile( "isb" );


    uint32_t count=LPTIM1->CNT;

    sprintf(buffer, "%lu", count);
    lcdPutsBlinkFree(buffer,2);

    */
  }
}

void TIM2_IRQHandler(){
  TIM2->SR &= ~TIM_SR_CC1IF;
  counter = TIM2->CCR1;
}

void USART1_IRQHandler(){
  USART1->ICR |= USART_ICR_IDLECF;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  configASSERT(uartRXSemaphore );
  xSemaphoreGiveFromISR(uartRXSemaphore,&xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

void EXTI15_10_IRQHandler(){

  EXTI->PR1 |= EXTI_PR1_PIF12;//Clear flag

  uint16_t lineState = GPIOB->IDR;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  lcdData_t dataToSend;

  dataToSend.messageType = chargingStatus;
  
  if(lineState & GPIO_IDR_ID12){
    dataToSend.displayValue = 1;
    xQueueSendToBackFromISR(lcdQueue, &dataToSend,&xHigherPriorityTaskWoken);
  }else{
    dataToSend.displayValue = 0;
    xQueueSendToBackFromISR(lcdQueue, &dataToSend,&xHigherPriorityTaskWoken);
  }

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  
}
