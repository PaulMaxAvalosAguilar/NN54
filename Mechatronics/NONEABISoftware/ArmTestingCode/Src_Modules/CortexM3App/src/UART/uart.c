#include "FreeRTOS.h"
#include "semphr.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include "uart.h"
#include "ring.h"
#include <string.h>
#include "task.h"

#define UART_RX_BUFFER_LEN 256
ring_t usart_rx_ring;
char usart_rx_buffer[UART_RX_BUFFER_LEN];
#if ((UART_RX_BUFFER_LEN - 1) & UART_RX_BUFFER_LEN) == 0
#else
	#warning UART_RX_BUFFER NOT POWER OF 2
#endif

char receiveBuffer[UART_RX_BUFFER_LEN] = {0}; //elements inialized to 0
SemaphoreHandle_t idleSmphr;

void uart_configure(){			

  //Configure ring buffer
  ring_buffer_init(&usart_rx_ring, usart_rx_buffer, sizeof(usart_rx_buffer[0]),
		   sizeof(usart_rx_buffer));
  
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

  dma_set_number_of_data(DMA1, DMA_CHANNEL5, UART_RX_BUFFER_LEN);
  
  dma_enable_channel(DMA1, DMA_CHANNEL5);
  //Receive DMA


  idleSmphr = xSemaphoreCreateBinary();
  USART_CR1(USART1) |= USART_CR1_IDLEIE;  //Enable USART1 Receive interrupt.
  usart_enable(USART1);//Enable USART 
}



void printString(const char myString[]) {

  dma_disable_channel(DMA1, DMA_CHANNEL4);
  dma_set_number_of_data(DMA1, DMA_CHANNEL4, strlen(myString));
  dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)myString);
  dma_enable_channel(DMA1, DMA_CHANNEL4);

  while(!dma_get_interrupt_flag(DMA1, DMA_CHANNEL4, DMA_TCIF));
  dma_clear_interrupt_flags(DMA1, DMA_CHANNEL4, DMA_TCIF); 

}

int serialAvailable(void){
  return  (!ring_buffer_empty(&usart_rx_ring))? 1:0;
}

char get_char(void){
  char c = 0;
  
  ring_buffer_get(&usart_rx_ring, &c);
  
  return c;
}

void forceReadCharLineUsart(charLineBuffer_t *charLineBuffer){

  int i = 0;
  char c;
  char *buffer = charLineBuffer->buf;

  while((c = get_char()) != '\n'){

    //if characters were found
    if(c != 0){
      //Add characters to buffer;
      buffer[i] = c;
      i++;
    }
  }

  buffer[i] = c;//Write string termination chacter (\n)
  charLineBuffer->terminatorcharposition = i;
}

void uartRxTask(void *args __attribute__((unused))){
  
  int i = 0;

  for(;;){
    xSemaphoreTake(idleSmphr,portMAX_DELAY);

    while(receiveBuffer[i]){
      ring_buffer_put(&usart_rx_ring,&receiveBuffer[i]);
      receiveBuffer[i] = 0;
      i = (i+1) % 256;
    }
  }
}

void usart1_isr(void)
{
  if (((USART_CR1(USART1) & USART_CR1_IDLEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_IDLE) != 0)) {
    usart_recv(USART1);//Interrupt clears at reading rx register

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(idleSmphr,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
  }
}
