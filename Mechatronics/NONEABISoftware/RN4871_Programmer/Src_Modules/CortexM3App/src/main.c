#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <string.h>

extern void initialise_monitor_handles(void);

static void configurePeriphereals(void){

  rcc_periph_clock_enable(RCC_AFIO);    // I2C UART ADC EXTI
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM2 UART
  rcc_periph_clock_enable(RCC_USART1);  // UART

  //UART
  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);

  /* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_PULL_UPDOWN , GPIO7);
  gpio_clear(GPIOB, GPIO7);

  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  //  usart_set_flow_control(USART1,USART_FLOWCONTROL_RTS_CTS);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  //UART

  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
		     AFIO_MAPR_I2C1_REMAP |  //I2C PB8 + PB9
		     AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1 |
		     AFIO_MAPR_USART1_REMAP  ); //USART PB6 + PB7

    usart_enable(USART1);//Enable USART

}

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  configurePeriphereals();
    initialise_monitor_handles();
  for(;;){

    for(unsigned int i = 0; i < 100000; i++);
    usart_send_blocking(USART1,'h');
    printf("HOLA\n");


  }

  return 0;
}

