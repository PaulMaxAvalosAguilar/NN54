#include "stm32f1xx.h"


void SystemClock_Config(void);

int main(void)
{
  //  RCC->CR |= RCC_CR_HSION;
  
  //Wait till HSI ready
  //  while( !(RCC->CR & (RCC_CR_HSIRDY)) );
  
  //Select HSI as system clock 01
  //  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (RCC_CFGR_SW_HSI);
  //  n200797009
  

  //Turn on HSE
  RCC->CR |= RCC_CR_HSEON;
  
  //Wait till HSE ready
  while( !(RCC->CR & (RCC_CR_HSERDY)) );
  
  //Select HSE as system clock 01
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (RCC_CFGR_SW_HSE);



  //Set HPRE DIV1
  RCC->CFGR &= ~(RCC_CFGR_HPRE_3);
  //Set ADCPRE
  RCC->CFGR |= (RCC_CFGR_ADCPRE_DIV8);
  //Set PPRE1
  RCC->CFGR |= (RCC_CFGR_PPRE1_DIV2);
  //Set PPRE2
  RCC->CFGR |= (RCC_CFGR_PPRE2_DIV1);

  //Set Flash wait state
  FLASH->ACR |= FLASH_ACR_LATENCY_2;

  //Set PLL Multiplier
  RCC->CFGR |= RCC_CFGR_PLLMULL9;

  //HSE OSC as PLL Source
  RCC->CFGR |= RCC_CFGR_PLLSRC;

  //HSE clock PLL Entry not divided
  RCC->CFGR &= ~(RCC_CFGR_PLLXTPRE);

  //Turn on PLL
  RCC->CR |= RCC_CR_PLLON;
  
  //Wait till PLL ready
  while(!(RCC->CR & RCC_CR_PLLRDY));

  //Select PLL as system clock source
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (RCC_CFGR_SW_PLL);


  
  //Enable port C
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  //Output mode, max speed 2 Mhz
  GPIOC->CRH &= ~(GPIO_CRH_MODE13_0);
  GPIOC->CRH |= GPIO_CRH_MODE13_1;

  //General purpose output Open-Drain
  GPIOC->CRH |= GPIO_CRH_CNF13_0;
  GPIOC->CRH &= ~GPIO_CRH_CNF13_1;

  GPIOC->BSRR = 1<<(13);
  
  while (1)
  {

    if((RCC->CFGR & RCC_CFGR_SWS_PLL)){
      for(int i = 0;i < 1000000; i++);
      GPIOC->BSRR = 1<<13;
      for(int i = 0;i < 1000000; i++);
      GPIOC->BSRR = 1<<(13+16);
    }
  }
}


