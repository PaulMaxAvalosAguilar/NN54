#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include "uart.h"
#include "ring.h"
#include <string.h>

#define USART0_SEND_BUFFER_LEN 256
#define USART0_RECEIVE_BUFFER_LEN   256

ring_t usart_send_ring;
ring_t usart_receive_ring;

char usart_send_buffer[USART0_SEND_BUFFER_LEN];
char usart_receive_buffer[USART0_RECEIVE_BUFFER_LEN];

#if ((USART0_SEND_BUFFER_LEN - 1) & USART0_SEND_BUFFER_LEN) == 0
# if USART0_SEND_BUFFER_LEN > 256
# warning USART0_SEND_BUFFER > 256
# endif
#else
	#warning USART0_SEND_BUFFER NOT POWER OF 2
#endif

#if ((USART0_RECEIVE_BUFFER_LEN- 1) & USART0_RECEIVE_BUFFER_LEN) == 0
# if USART0_RECEIVE_BUFFER_LEN > 256
# warning USART0_RECEIVE_BUFFER > 256
# endif
#else
	#warning USART0_RECEIVE_BUFFER NOT POWER OF 2
#endif

char receiveBuffer[10]; //elements inialized to 0
int idle = 0;

void uart_init(){			
  ring_buffer_init(&usart_receive_ring, usart_receive_buffer, sizeof(usart_send_buffer[0]), sizeof(usart_send_buffer));

  ring_buffer_init(&usart_send_ring, usart_send_buffer, sizeof(usart_send_buffer[0]),sizeof(usart_send_buffer));
  

  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);

  /* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT, GPIO7);

  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX_RX);

  /* Enable USART1 Receive interrupt. */
  //  USART_CR1(USART1) |= USART_CR1_RXNEIE;
  //USART_CR1(USART1) |= USART_CR1_IDLEIE;
  
  /* Finally enable the USART. */
  usart_enable(USART1);

  //Write DMA
  usart_enable_tx_dma(USART1);
  dma_set_priority(DMA1, DMA_CHANNEL4, DMA_CCR_PL_LOW );
  dma_set_read_from_memory(DMA1, DMA_CHANNEL4);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL4);
  
  dma_set_memory_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);

  dma_set_peripheral_address(DMA1, DMA_CHANNEL4, (uint32_t)&USART1_DR);
  //Write DMA

  //Receive DMA
  usart_enable_rx_dma(USART1);
  dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_VERY_HIGH);
  dma_set_read_from_peripheral(DMA1, DMA_CHANNEL5);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);
  dma_enable_circular_mode(DMA1, DMA_CHANNEL5);
  
  dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);

  dma_set_memory_address(DMA1, DMA_CHANNEL5, (uint32_t)receiveBuffer);
  dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t)&USART1_DR);

  dma_set_number_of_data(DMA1, DMA_CHANNEL5, 10);
  
  dma_enable_channel(DMA1, DMA_CHANNEL5);
  //Receive DMA
  
 
}

void printString(const char myString[]) {

  dma_disable_channel(DMA1, DMA_CHANNEL4);
  dma_set_number_of_data(DMA1, DMA_CHANNEL4, strlen(myString));
  dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)myString);
  dma_enable_channel(DMA1, DMA_CHANNEL4);

  while(!dma_get_interrupt_flag(DMA1, DMA_CHANNEL4, DMA_TCIF));
  dma_clear_interrupt_flags(DMA1, DMA_CHANNEL4, DMA_TCIF); 
}

void _putchar(char character){
  while(ring_buffer_full(&usart_send_ring));//wait till there's space
  ring_buffer_put(&usart_send_ring, &character);
  USART_CR1(USART1) |= USART_CR1_TXEIE;
}

int serialAvailable(void){
  return  (!ring_buffer_empty(&usart_receive_ring))? 1:0;
}

char get_char(void){
  char c = 0;
 
  ring_buffer_get(&usart_receive_ring, &c);
 
  return c;
}

void usart1_isr(void)
{
  if (((USART_CR1(USART1) & USART_CR1_IDLEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_IDLE) != 0)) {
    usart_recv(USART1);
    idle = 1;
  }
}
