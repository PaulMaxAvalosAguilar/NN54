#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "uart.h"
#include "RN4020definitions.h"

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void writeEncoderValues(uint16_t value0,
			 uint16_t value1,
			 uint16_t value2);
void setBLEConnected(uint8_t boolean);
uint8_t getBLEConnected(void);
void setENCODERStarted(uint8_t boolean);
uint8_t getENCODERStarted(void);

void setMinDistToTravel(uint16_t value);
uint16_t getMinDistToTravel(void);
void setDesiredCountDir(uint8_t value);
uint8_t getDesiredCountDir(void);
void setDesiredRepDir(uint8_t value);
uint8_t getDesiredRepDir(void);

//-------------- USART PARSING FUNCTIONS----------
void genericLineParsing(charLineBuffer_t *);

#endif
