#ifndef UART_H
#define UART_H

void uart_init(void);
void printString(const char myString[]);
int put_char(int c);
int serialAvailable(void);
char get_char(void);

extern char receiveBuffer[10];
extern int idle;

#endif
