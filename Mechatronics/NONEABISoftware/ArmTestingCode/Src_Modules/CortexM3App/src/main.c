#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

static void
task1(void *args __attribute__((unused))) {

  lcd_init(LCD_DISP_ON);
  lcd_puts("Hello");
  lcd_gotoxy(0,1);
  lcd_puts("Goodbye");
  for (;;) {
    
  }
}

int main(void)
{

  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C
  rcc_periph_clock_enable(RCC_I2C1);	// I2C

  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO6|GPIO7);			// I2C
  gpio_set(GPIOB,GPIO6|GPIO7);		// Idle high

  // AFIO_MAPR_I2C1_REMAP=0, PB6+PB7
  gpio_primary_remap(0,0); 

  xTaskCreate(task1,"task1",800,NULL,1,NULL);
  vTaskStartScheduler();
  for(;;);
  return 0;
}
