#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "uart.h"
#include "RN4020definitions.h"


//PRIVATE HANDLES
typedef struct{
  uint8_t handleFound;
  uint16_t handle;
  uint8_t notificationEnabled;
  uint8_t isNotifying;
} characteristicStatus_t;

extern characteristicStatus_t characteristicStatus;

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


//-------------- USART PARSING FUNCTIONS----------
void genericLineParsing(charLineBuffer_t *);


#endif
