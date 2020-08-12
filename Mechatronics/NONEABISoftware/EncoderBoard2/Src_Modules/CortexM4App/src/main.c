#include "stm32g431xx.h"

void SystemClock_Config(void);

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
  FLASH->ACR = (FLASH->ACR & (~FLASH_ACR_LATENCY)) | FLASH_ACR_LATENCY_2WS; //2 wait states
    
  //Turn HSE and wait till is ready
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR & (RCC_CR_HSERDY)));

  //Turn PLL on, configure it and wait till ready
  RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN))
    | (0b10 << RCC_PLLCFGR_PLLM_Pos) | (24 << RCC_PLLCFGR_PLLN_Pos); //M = 3, N = 24
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;    
    
  RCC->CR |= RCC_CR_PLLON;
  while(!RCC_CR_PLLRDY);

  RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    
  //Select PLL as system clocksource and wait till selected
  RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_SW_PLL)) | (RCC_CFGR_SW_PLL);
  while( ((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


  //---------------------ENABLE GPIO CLOCK---------------------
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE8)) | (0b01 << GPIO_MODER_MODE8_Pos); //General Purpose Output mode
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED8)) | (0b00 << GPIO_OSPEEDR_OSPEED8_Pos); //Low speed
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT8; //Push pull
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD8)) | (0b00 << GPIO_PUPDR_PUPD8_Pos); //No Pull up down
  //  GPIOA->BSRR |= GPIO_BSRR_BS_8;


  //---------------------CONFIGURE I2C-------------------------

  /*
  //I2C2 GPIO CONFIGUREATION
  //PA8 I2C2_SDA
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE8)) | (0b10 << GPIO_MODER_MODE8_Pos); //Alternate function mode
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED8)) | (0b00 << GPIO_OSPEEDR_OSPEED8_Pos); //Low speed
  GPIOA->OTYPER |= GPIO_OTYPER_OT8; //Open drain
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD8)) | (0b01 << GPIO_PUPDR_PUPD8_Pos); //Pull up
  GPIOA->AFR[0] = (GPIOA->AFR[0] & (~GPIO_AFRH_AFSEL8)) | (4 << GPIO_AFRH_AFSEL8_Pos); //Alternate function 4

  //PA9 I2C2_SCL
  GPIOA->MODER = (GPIOA->MODER & (~GPIO_MODER_MODE9)) | (0b10 << GPIO_MODER_MODE9_Pos); //Alternate function mode
  GPIOA->OSPEEDR = (GPIOA->OSPEEDR & (~GPIO_OSPEEDR_OSPEED9)) | (0b00 << GPIO_OSPEEDR_OSPEED9_Pos); //Low speed
  GPIOA->OTYPER |= GPIO_OTYPER_OT9; //Open drain
  GPIOA->PUPDR = (GPIOA->PUPDR & (~GPIO_PUPDR_PUPD9)) | (0b01 << GPIO_PUPDR_PUPD9_Pos); //Pull up
  GPIOA->AFR[0] = (GPIOA->AFR[0] & (~GPIO_AFRH_AFSEL9)) | (4 << GPIO_AFRH_AFSEL9_Pos); //Alternate function 4

  //I2C2 CONFIGURATION
    
  //RCC_CCIPR I2C2 PCLK selected as clock
  //Analog noise filter ON
  //Digital filter diSabled
  //Clock stretching enabled    
  RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN; //Enable I2C clock
  I2C2->TIMINGR = 0x00702991 & 0xF0FFFFFFU; //Set timings
  I2C2->CR1 |= I2C_CR1_PE; //Enable periphereal
  */

  while( ((RCC->PLLCFGR) & RCC_PLLCFGR_PLLN) == (24 << RCC_PLLCFGR_PLLN_Pos)){
        GPIOA->BSRR |= GPIO_BSRR_BS8;
  }


  while (1)
  {

      GPIOA->BSRR |= GPIO_BSRR_BS8;
      for(int i = 1; i < 1000000;i++);
      GPIOA->BSRR |= GPIO_BSRR_BR8;
      for(int i = 1; i < 1000000;i++);

  }
}


