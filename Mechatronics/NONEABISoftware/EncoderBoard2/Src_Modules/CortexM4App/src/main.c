#include "FreeRTOS.h"
#include "task.h"
#include "stm32g431xx.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#define UART_RX_BUFFER_LEN 256
char receiveBuffer[UART_RX_BUFFER_LEN] = {0};

static volatile uint32_t counter = 0;
static volatile uint32_t charging = 0;

void USART1_IRQHandler(void);
void TIM2_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void lcdPutsBlinkFree(const char *g, int ypos);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base);
void printString(const char myString[]);

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

// inline function to swap two numbers
static inline void myswap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j){
	while (i < j)
		myswap(&buffer[i++], &buffer[j--]);
	return buffer;
}

// Iterative function to implement itoa() function in C
char* intoa(int value, char* buffer, int base){
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = value;

	int i = 0;
	while (n)
	{
		int r = n % base;
		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;
		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';
	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';
	buffer[i] = '\0'; // null terminate string
	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

static void mainTask(void *args __attribute__((unused))){
  
  
  
  int g = 0;

  lcd_init();
  lcd_gotoxy(0,0);

  lcd_puts("Encoder");
  lcd_gotoxy(1,7);
  lcd_puts("Saint Germain");

  char buffer[30];
  
  for(;;){
    sprintf(buffer, "%d", g++);
    lcdPutsBlinkFree(buffer,3);


    
  }
}

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
  GPIOB->MODER = (GPIOB->MODER & (~GPIO_MODER_MODE14)) | (0b00 << GPIO_MODER_MODE14_Pos) ; //General purpose inputMode
  GPIOB->OTYPER &= (~GPIO_OTYPER_OT14);//Push pull  
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & (~GPIO_OSPEEDR_OSPEED14)) | (0b00 << GPIO_OSPEEDR_OSPEED14_Pos); //Low speed
  GPIOB->PUPDR = (GPIOB->PUPDR & (~GPIO_PUPDR_PUPD14)) | (0b10 << GPIO_PUPDR_PUPD14_Pos); //Pull down


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

  DMAMUX1_Channel2->CCR = (DMAMUX1_Channel2->CCR & (~DMAMUX_CxCR_DMAREQ_ID)) | (19 << DMAMUX_CxCR_DMAREQ_ID_Pos);//Request 29

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
  TIM2->TISEL = (TIM2->TISEL & (~TIM_TISEL_TI1SEL)) | (0b0000 << TIM_TISEL_TI1SEL_Pos);//tim_ti1_in source
  TIM2->TISEL = (TIM2->TISEL & (~TIM_TISEL_TI2SEL)) | (0b0000 << TIM_TISEL_TI2SEL_Pos);//tim_ti2_in source
  TIM2->CCER |= (TIM_CCER_CC1NP | TIM_CCER_CC1P);//edge selection both edges for tim_ti1 source
  //TIM2->CCER |= (TIM_CCER_CC2NP | TIM_CCER_CC2P);//edge selection both edges for tim_ti2 source
  TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_IC1F)) | (0b0011 << TIM_CCMR1_IC1F_Pos);//Tim_ti1 filtered for 8 clock cycles
  //TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_IC2F)) | (0b0011 << TIM_CCMR1_IC2F_Pos);//Tim_ti2 filtered for 8 clock cycles
  TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_CC1S)) | (0b01 << TIM_CCMR1_CC1S_Pos);//(input mode)Tim_ic1 mapping on tim_ti1 
  //TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_CC2S)) | (0b01 << TIM_CCMR1_CC2S_Pos);//(input mode)Tim_ic2 mapping on tim_ti2
  TIM2->CCER |= (TIM_CCER_CC1E);//Capture enabled for Capture register 1 and 2

  TIM2->CR2 |= TIM_CR2_TI1S;// ti1_in ti2_in source XORed
  TIM2->ARR = 0xFFFFFFFF;//Autoreload register
  TIM2->PSC = 5;//Preescaler
  TIM2->CNT = 0;
  TIM2->DIER = TIM_DIER_CC1IE;//Enable Interrupts
  TIM2->CR1 |= TIM_CR1_CEN; //Counter enable

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


  //ADC1 CONFIGURATION
  uint16_t adcCalFactD = 0;
  uint16_t adcCalFactS = 0;

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

  ADC1->CR |= ADC_CR_ADEN;//Enable ADC1;

  ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_L)) | (0b0001 << ADC_SQR1_L_Pos);//2 conversions
  ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_SQ1)) | (5 << ADC_SQR1_SQ1_Pos);//1st conversion on IN5
  ADC1->SQR1 = (ADC1->SQR1 & ~(ADC_SQR1_SQ2)) | (18 << ADC_SQR1_SQ2_Pos);//1st conversion on IN18
  
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


  //---------------------CONFIGURE NVIC--------------------------

  NVIC_SetPriorityGrouping(4); //4 bits for pre-emption 0 bit for subpriority
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  //  NVIC_EnableIRQ(TIM2_IRQn);

  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART1_IRQn);



  char buffer[21];


  xTaskCreate(mainTask,"mainTask",800,NULL,1,NULL);
  vTaskStartScheduler();
  
  while (1)
  {

    /*
    printString("\x31""Hola\n");

    for(int i = 1; i < 1000000;i++);
    for(int i = 1; i < 1000000;i++);
    */

    /*
    while(receiveBuffer[g]){
      //      printf("%c",receiveBuffer[g]);
      receiveBuffer[g] = 0;
      g = (g+1) % UART_RX_BUFFER_LEN;
    }
    */

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

    if(receiveBuffer[g]){
      printString(receiveBuffer+g);
    
      while(receiveBuffer[g]){
	receiveBuffer[g] = 0;
	g = (g+1) % UART_RX_BUFFER_LEN;

      }
    }

    */
    
    /*
    if(USART1->ISR & USART_ISR_RXNE){
      g = (USART1->RDR) & 0X00FF;
      printf("%c\n",g);
    };
    */
  }
}

void printString(const char myString[]){
  DMA1_Channel2->CCR &= ~DMA_CCR_EN;//Channel disable
  DMA1_Channel2->CMAR = (uint32_t)myString;//DMA source address
  DMA1_Channel2->CNDTR = strlen(myString);//DMA length
  DMA1_Channel2->CCR |= DMA_CCR_EN;//Channel enable
  while(!(DMA1->ISR & DMA_ISR_TCIF2));//Wait till transfere complete
  DMA1->IFCR |= DMA_IFCR_CTCIF2;//Clear transfere complete  
}

void USART1_IRQHandler(){
  USART1->ICR |= USART_ICR_IDLECF;
}

void TIM2_IRQHandler(){
  TIM2->SR &= ~TIM_SR_CC1IF;
  counter = TIM2->CCR1;
}

void EXTI15_10_IRQHandler(){

  EXTI->PR1 |= EXTI_PR1_PIF12;//Clear flag
  
  if((GPIOB->IDR & GPIO_IDR_ID12)){
    charging = 1;
  }else{
    charging = 0;
  }
  
}



/*

  
  DIFSEL[I] ADC_DIFSEL  //Channel single endded or differential input selection
  ADC_SQR1 |= L
  ADC_SQR1 |= SQ1 //Channel 0?
  //Read ADC_DR

  //Set injected channel sequence legth
  
  //Enable VREFEN  in ADCx_CCR
  

  ADEN = 1//Enable adc
  //Wait till ADRDY
  //Write 1 to ADRDY
  ADSTART
  JADSTART

  ADDIS = 1 //Disable ADC
  //Wait till ADDIS = 0
  
  */
