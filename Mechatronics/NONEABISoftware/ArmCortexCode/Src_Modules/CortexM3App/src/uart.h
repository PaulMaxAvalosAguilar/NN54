#ifndef UART
#define UART

#include <stdint.h>
#include <stdio.h>

void uart_init(void);
void printString(const char myString[]);
int serialAvailable(void);
char get_char(void);

#endif
