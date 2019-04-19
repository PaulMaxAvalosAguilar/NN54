#ifndef USART
#define USART

#include <stdio.h>

#ifndef F_CPU
	#warning F_CPU is undefined, USART may not work correctly without this
#endif

#define BAUD_CALC(x) ((F_CPU+(x)*8UL) / (16UL*(x))-1UL) // macro calculating precise UBRR value
#define DOUBLE_BAUD_CALC(x) ((F_CPU+(x)*4UL) / (8UL*(x))-1UL) // macro calculating UBRR value for double speed

#define BAUD_TOL 2

#define UBRR_VALUE BAUD_CALC(BAUD)

#if 100 * (F_CPU) > \
  (16 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) + (BAUD) * (BAUD_TOL))
#  ifndef USE_2X
#  define USE_2X
#  endif 
#elif 100 * (F_CPU) < \
  (16 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) - (BAUD) * (BAUD_TOL))
#  ifndef USE_2X
#  define USE_2X
#  endif
#endif

#ifdef USE_2X
#undef UBRR_VALUE
#define UBRR_VALUE DOUBLE_BAUD_CALC(BAUD)

#  if 100 * (F_CPU) > \
  (8 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) + (BAUD) * (BAUD_TOL))
#  warning "Baud rate achieved is higher than allowed with 2X"
#  endif

#  if 100 * (F_CPU) < \
  (8 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) - (BAUD) * (BAUD_TOL))
#  warning "Baud rate achieved is lower than allowed with 2X"
#  endif
#endif /*USE_DOUBLE_SPEED*/

#ifdef UBRR_VALUE
   /* Check for overflow */
#  if UBRR_VALUE >= (1 << 12)
#    warning "UBRR value overflow"
#  endif
#endif

void uart_init(uint16_t ubrr_value);
void printString(const char myString[]);
int put_char(char data, FILE *stream);
int serialAvailable(void);
char get_char(void);

#define uart_autoinit() uart_init(UBRR_VALUE)

#define FDEV_SETUP_STREAM_U(p, g, f, u) \
	{ \
		.put = p, \
		.get = g, \
		.flags = f, \
		.udata = u, \
	}

extern FILE uart_io;

#endif
