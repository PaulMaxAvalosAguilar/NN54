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
  
  
  while (1)
  {
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
    FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

    
    //Turn HSE and wait till is ready
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & (RCC_CR_HSERDY)));

    //Turn PLL on, configure it and wait till ready
    RCC->PLLCFGR = (RCC_PLLCFGR_PLLM_0 | (20 << RCC_PLLCFGR_PLLN_Pos)); //M = 2 , N = 20
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;    
    
    RCC->CR |= RCC_CR_PLLON;
    while(!RCC_CR_PLLRDY);

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    
    //Select PLL as system clocksource
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    
  }
}


