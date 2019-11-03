
#ifndef UART_H
#define UART_H

typedef struct charLineBuffer_t{
  char buf[100];
  uint16_t terminatorcharposition;
}charLineBuffer_t;

void uart_configure(void);
void printString(const char myString[]);
int serialAvailable(void);
char get_char(void);
void readDMA(void);
charLineBuffer_t *forceReadCharLineUsart(void);
void uartRxTask(void *args);


#endif
