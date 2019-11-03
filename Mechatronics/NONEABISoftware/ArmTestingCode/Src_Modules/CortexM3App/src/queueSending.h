#ifndef QUEUESENDING_H
#define QUEUESENDING_H


#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t communicationQueue;
extern QueueHandle_t lcdQueue;

//communicationTask -------------------------
typedef enum DataSource_t{
			  adcSender,
			  encoderSender
}DataSource_t;

typedef struct commData_t{
  DataSource_t eDataSource;
  uint16_t uValue;
} commData_t;
//communicatioTask --------------------------

//lcdTask -----------------------------------
typedef enum LCDMessage_t{
			  turnOnMessage,
			  bleConfig,
			  connectedStatus,
			  batteryLevel,
			  chargingStatus
}LCDMessage_t;

typedef struct lcdData_t{
  LCDMessage_t messageType;
  uint32_t displayValue;  
}lcdData_t;
//lcdTask -----------------------------------

void sendToCommunicationQueue(DataSource_t eDataSource, uint16_t uValue);
void sendToLCDQueue(LCDMessage_t messageType,
		    uint32_t displayValue);

#endif
