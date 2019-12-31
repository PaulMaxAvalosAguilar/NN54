#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "uart.h"
#include "RN4020definitions.h"


extern uint8_t bluetoothConnected;

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
void writeOneOneByteCharacteristic(uint8_t value0);
void writeTenTwoBytesCharacteristic(uint16_t value0,
				    uint16_t value1, uint16_t value2,
				    uint16_t value3, uint16_t value4,
				    uint16_t value5, uint16_t value6,
				    uint16_t value7, uint16_t value8,
				    uint16_t value9);
void setBLEConnected(uint8_t boolean);
uint8_t getBLEConnected(void);


//-------------- USART PARSING FUNCTIONS----------
void genericLineParsing(charLineBuffer_t *);


#endif
