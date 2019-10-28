#ifndef COMMUNICACTION
#define COMMUNICATION

#include <stdint.h>
#include "uart.h"
#include "RN4020definitions.h"

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored

void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...);
void advertise(void);
void unBond(void);
void unBondAdvertise(void);
void sendLS(void);
void cleanPrivateService(void);
void setFactoryReset(uint8_t arg);
void setFeatures(uint32_t arg);
void setSupportedServices(uint32_t arg);
void setPrivateService(const char *arg);
void setPrivateCharacteristic(const char *service,
			      uint8_t properties,
			      uint8_t dataSize,
			      uint8_t securityFlag);
void setName(char *string);//No >  6 bytes when using private service
void writeOneOneByteCharacteristic(uint8_t value0);
void writeTenTwoBytesCharacteristic(uint16_t value0,
				    uint16_t value1, uint16_t value2,
				    uint16_t value3, uint16_t value4,
				    uint16_t value5, uint16_t value6,
				    uint16_t value7, uint16_t value8,
				    uint16_t value9);
void turnOffSubscription(void);

//-------------- USART PARSING FUNCTIONS----------
int parseWCLine(const char* line);
int parseUUIDLine(const char* line);
void unlockWaitingLineParsing(charLineBuffer_t *,
			      int* waitingState, int *errorDetection);
void genericLineParsing(charLineBuffer_t *);

//RN4020 configuration
void bluetoothConfig(int configuration);

#endif
