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
int32_t arccos_cordic ( int32_t t);
int32_t fixMultiply(int32_t a, int32_t b);

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

  double val = .9;

  for(;;){

    /*
    val += .1;
    angleDouble = sqrt(val);

    angleDouble = sqrtf(val);
    */

    val += .0001;
    angleDouble = acosf(val);

    /*
    yFix = fix16_from_float(.5);
    angleFix = fix16_sqrt(yFix);
    float d = fix16_to_float(angleFix);
    */

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

int32_t arccos_cordic ( int32_t cosine){
  int32_t angle;
  
  int32_t i;
  int32_t j;
  int32_t poweroftwo;
  int32_t sigma;
  int32_t sign_z2;
  int32_t theta;
  int32_t x1;
  int32_t x2;
  int32_t y1;
  int32_t y2;

  /*
  double angles[27]= {  45,       26.565051, 14.036243, 7.125016, 3.576334,
		      1.789910, 0.895173,  0.447585,  0.223796, 0.111898,
		      0.055977, 0.027960,  0.013980,  0.006990, 0.003495,
		      0.001776, 0.000859,  0.000458,  0.000229, 0.000114,
		      0.000057, 0.000027,  0.000014,  0.000007, 0.00003,
		      0.000002, 0.000001};
*/

  /*
  double angles[27] = {
		       44.999895 ,26.564989 ,14.036211 ,7.125000 ,3.576326 ,1.789906 ,0.895172 ,0.447613 ,0.223810 ,0.111905 ,0.055953 ,0.027976 ,0.013988 ,0.006994 ,0.003497 ,0.001749 ,0.000874 ,0.000437 ,0.000219 ,0.000109 ,0.000055 ,0.000027 ,0.000014 ,0.000007 ,0.000003 ,0.000002 ,0.000001 };
  */

  int32_t angles[27] = {
		       44999895 ,26564989 ,14036211 ,7125000 ,3576326 ,1789906 ,895172 ,447613 ,223810 ,111905 ,55953 ,27976 ,13988 ,6994 ,3497 , 1749 ,874 ,437 ,219 ,109 ,55 ,27 ,14 ,7 ,3 ,2 ,1 };


  theta = 0;
  x1 = 1000000;
  y1 = 0;
  poweroftwo = 1000000;

  for ( j = 0; j < 16; j++ )
  {
    if ( y1 < 0 )
    {
      sign_z2 = -1000000;
    }
    else
    {
      sign_z2 = +1000000;
    }

    if ( cosine <= x1 )
    {
      sigma = + sign_z2;
    }
    else
    {
      sigma = - sign_z2;
    }

    angle = angles[j];

    for ( i = 0; i < 2; i++ )
    {
      x2 = x1 - fixMultiply( sigma, fixMultiply( poweroftwo, y1));
      y2 = fixMultiply( sigma, fixMultiply( poweroftwo, x1)) + y1;

      x1 = x2;
      y1 = y2;
    }

    theta = theta + fixMultiply(2000000, fixMultiply(sigma, angle));
    cosine = cosine + fixMultiply(cosine, fixMultiply(poweroftwo, poweroftwo));
    
    poweroftwo = poweroftwo / 2;
  }

  return theta;
}

int32_t fixMultiply(int32_t a, int32_t b){
  int64_t mul = (int64_t)a * (int64_t)b;
  int32_t result = mul/1000000;
  return result;
}

