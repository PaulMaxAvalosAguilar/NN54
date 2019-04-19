#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "uart.h"

static void clock_setup(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  /* Enable GPIOA clock (for LED GPIOs). */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_USART1);
}


static void gpio_setup(void)
{
  gpio_set(GPIOC, GPIO13);

  /* Setup GPIO6 and 7 (in GPIO port A) for LED use. */
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}



static void uart_task(void *pvParameters __attribute__((unused))){
  for(;;){
    printString("Paul\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    printString("Max\n");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

int main(void)
{
  clock_setup();
  gpio_setup();
  uart_init();

  xTaskCreate(uart_task,"uart_task",100,NULL, configMAX_PRIORITIES-1,NULL);

  vTaskStartScheduler();

  while (1)
    __asm__("nop");

  return 0;
}
