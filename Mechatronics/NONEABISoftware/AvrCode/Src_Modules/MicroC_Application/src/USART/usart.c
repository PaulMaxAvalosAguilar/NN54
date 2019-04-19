#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "ring.h"

FILE uart_io = FDEV_SETUP_STREAM_U(put_char, NULL, _FDEV_SETUP_WRITE, (void*)0);

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

void uart_init(uint16_t ubrr_value){
			
  UBRR0L	= (uint8_t) ubrr_value;
  UBRR0H	= (ubrr_value>>8);
		
#ifdef USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif

  ring_buffer_init(&usart_receive_ring, usart_receive_buffer, sizeof(usart_send_buffer[0]), sizeof(usart_send_buffer));

  ring_buffer_init(&usart_send_ring, usart_send_buffer, sizeof(usart_send_buffer[0]),sizeof(usart_send_buffer));

  //Atach uart stream to stdout
  stdout = &uart_io;

  UCSR0B |= (1 << TXEN0) | (1 << RXEN0); /* Enable USART transmitter/receiver */

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */
  
  UCSR0B |= (1<< RXCIE0);//Enable USART receive interrupt
}

void printString(const char myString[]) {
  uint8_t i = 0;
  while (myString[i]) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = myString[i];
    i++;
  }
}

int put_char(char data, FILE *stream){
                                     
  //loop_until_bit_is_set(UCSR0A, UDRE0);/* Wait for empty transmit buffer */
  //UDR0 = data;
  while(ring_buffer_full(&usart_send_ring));//wait till there's space
  ring_buffer_put(&usart_send_ring, &data);
  UCSR0B |= (1 <<  UDRIE0);
  return 0;
}

int serialAvailable(void){
  return  (!ring_buffer_empty(&usart_receive_ring))? 1:0;
}

char get_char(void){
  char c = 0;
 
  ring_buffer_get(&usart_receive_ring, &c);
 
  return c;
}

ISR(USART_RX_vect){

  const char c = UDR0; 
  
  ring_buffer_put(&usart_receive_ring, &c);

}

ISR(USART_UDRE_vect){

  if(ring_buffer_empty(&usart_send_ring))
    UCSR0B &= ~(1 <<  UDRIE0);
    
  char c;
  if(ring_buffer_get(&usart_send_ring,&c) != -1){
      UDR0 = c;
  }
}
