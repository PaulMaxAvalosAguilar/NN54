#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include "FreeRTOS.h"
#include "printf.h"
#include "task.h"
#include "lcd.h"

void configurePeriphereals(void);

void configurePeriphereals(void){
  rcc_periph_clock_enable(RCC_AFIO);    // I2C
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM4
  rcc_periph_clock_enable(RCC_I2C1);	// I2C
  rcc_periph_clock_enable(RCC_TIM4);    // TIM4

  //I2C
  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO8|GPIO9);			
  gpio_set(GPIOB,GPIO8|GPIO9);
  //I2C

  //TIM4
  gpio_set_mode(GPIOB,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO6|GPIO7);
  TIM4_ARR = 0xFFFF;
  TIM4_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI2 | TIM_CCMR1_CC1S_IN_TI1);
  TIM4_CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM4_SMCR |= TIM_SMCR_SMS_EM3;
  TIM4_CNT = 32767;
  TIM4_CR1 = TIM_CR1_CEN;
  //TIM4  

  gpio_primary_remap(0,AFIO_MAPR_I2C1_REMAP);  //I2C PB8 + PB9
}

static void
task1(void *args __attribute__((unused))) {

  lcd_init(LCD_DISP_ON);
  lcd_puts("Hello");
  lcd_gotoxy(0,1);
  lcd_puts("Good");

  char buffer[20];
  
  for (;;) {
    sprintf(buffer,"%d", TIM4_CNT - 32767);
    lcd_gotoxy(0,2);
    lcd_puts(buffer);    
  }
}

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  configurePeriphereals();
  xTaskCreate(task1,"task1",800,NULL,1,NULL);
  vTaskStartScheduler();
  for(;;);
  return 0;
}
