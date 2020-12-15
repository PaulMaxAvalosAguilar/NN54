#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "main.h"
#include "RN4020definitions.h"

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void writeEncoderData(uint8_t centralCode,
			char traveledDistance[3],
			char meanPropulsiveVelocity[3],
			char peakVelocity[3]);
void writeEncoderStartValue(uint8_t centralCode);
void writeBatteryLevel(uint8_t centralCode,
		       char level[3]);
void writeEncoderStop(uint8_t centralCode);
void writeEncoderChargingStatus(uint8_t centralCode,
				char chargingStatus[3]);

//-------------- USART PARSING FUNCTIONS----------
void genericParsing(char *line);

#endif
