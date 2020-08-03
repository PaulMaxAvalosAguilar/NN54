#include "stm32g431xx.h"

void SystemClock_Config(void);

int main(void)
{
  
  while (1)
  {
    RCC->CFGR |= RCC_CFGR_SW_0;
  }
}


