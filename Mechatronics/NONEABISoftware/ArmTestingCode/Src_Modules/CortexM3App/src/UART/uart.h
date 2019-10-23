#ifndef UART_H
#define UART_H

void uart_configure(void);
void printString(const char myString[]);
int serialAvailable(void);
char get_char(void);
void uartRxTask(void *args);

#endif
