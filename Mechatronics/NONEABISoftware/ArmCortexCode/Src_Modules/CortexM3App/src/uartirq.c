#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "uart.h"
#include "printf.h"

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

void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...){
  
  //Send Command
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  //Send Command
}

static void uart_task(void *pvParameters __attribute__((unused))){

  char *ptr = "Hello world!";
  char *np = 0;
  int i = 5;
  unsigned int bs = sizeof(int)*8;
  int mi;
  char buf[80];

  mi = (1 << (bs-1)) + 1;
  printf("%s\n", ptr);
  printf("printf test\n");
  printf("%s is null pointer\n", np);
  printf("%d = 5\n", i);
  printf("%d = - max int\n", mi);
  printf("char %c = 'a'\n", 'a');
  printf("hex %x = ff\n", 0xff);
  printf("hex %02x = 00\n", 0);
  printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
  printf("%d %s(s)%", 0, "message");
  printf("\n");
  printf("%d %s(s) with %%\n", 0, "message");
  sprintf(buf, "justif: \"%-10s\"\n", "left"); printf("%s", buf);
  sprintf(buf, "justif: \"%10s\"\n", "right"); printf("%s", buf);
  sprintf(buf, " 3: %04d zero padded\n", 3); printf("%s", buf);
  sprintf(buf, " 3: %-4d left justif.\n", 3); printf("%s", buf);
  sprintf(buf, " 3: %4d right justif.\n", 3); printf("%s", buf);
  sprintf(buf, "-3: %04d zero padded\n", -3); printf("%s", buf);
  sprintf(buf, "-3: %-4d left justif.\n", -3); printf("%s", buf);
  sprintf(buf, "-3: %4d right justif.\n", -3); printf("%s", buf);
  
  for(;;){
    runLockingCOMMAND(NULL,"SN,%s\n", "BVTBLE");
    printf("printf test\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    printf("hex %04X\n", 45);
    printf("SR,300,%04X\n", 100);
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
  
  while (1);

  return 0;
}
