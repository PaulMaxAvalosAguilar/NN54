#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "main.h"
#include "RN4020definitions.h"

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void writeEncoderValues(uint16_t value0,
			 uint16_t value1,
			 uint16_t value2);
//-------------- USART PARSING FUNCTIONS----------
void genericLineParsing(charLineBuffer_t *);

#endif
