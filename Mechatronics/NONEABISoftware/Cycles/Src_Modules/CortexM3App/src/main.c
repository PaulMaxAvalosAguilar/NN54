
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#include <math.h>
#include "fixmath.h"

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

int32_t catan2(int32_t xvalue, int32_t yvalue);

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"

  KIN1_InitCycleCounter(); /* enable DWT hardware */
  KIN1_ResetCycleCounter(); /* reset cycle counter */
  KIN1_EnableCycleCounter();

  int y = 800;
  int x = 42;
  int angle = 0;

  fix16_t yFix = fix16_from_int(y);
  fix16_t xFix = fix16_from_int(x);
  fix16_t angleFix = 0;

  double angleDouble = 0;
  double angleFloat = 0;

  for(;;){
    angle = catan2(y,x);

    angleFix = fix16_atan2(y,x);

    angleDouble = atan2((double)y, (double) x);

    angleFloat = atan2f((float)y, (float) x);

    y++;
    x++;
  }

  return 0;
}

int32_t catan2(int32_t xvalue, int32_t yvalue){

  int32_t y = xvalue * 1000;
  int32_t x = yvalue * 1000;

  int32_t iterations = 0;

  int32_t xRotationResult = x;
  int32_t yRotationResult = y;
    
  int32_t sum = 0;

  int32_t angles[16] = {
			 45000, 26565, 14036, 7125, 3576, 1790, 895, 448, 224, 112, 56, 28, 14, 7, 3
  };


  while( iterations < 16){

    if(yRotationResult > 0){
      
      xRotationResult = x + (y >>  iterations);
      yRotationResult = y - (x >>  iterations);

      x = xRotationResult;
      y = yRotationResult;


      sum += angles[iterations]; 

    }else{

      xRotationResult = x - (y >> iterations);
      yRotationResult = y + (x >> iterations);
      x = xRotationResult;
      y = yRotationResult;
      
      sum -= angles[iterations]; 
    }

    iterations++;
  }

  return sum;
}

