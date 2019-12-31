#ifndef QUEUESENDING_H
#define QUEUESENDING_H


#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define LCD_QUEUE_SIZE                20
#define BINARY_SEMAPHORE_SIZE         1
#define COMMUNICATION_QUEUE_SIZE      20
#define COMMUNICATION_QUEUE_SET_SIZE  LCD_QUEUE_SIZE + BINARY_SEMAPHORE_SIZE

extern SemaphoreHandle_t adcSemaphore;

extern QueueHandle_t communicationQueue;
extern QueueHandle_t lcdQueue;
extern QueueSetHandle_t communicationQueueSet;

//communicationTask -------------------------
typedef enum DataSource_t{
			  adcSender,
			  encoderSender
}DataSource_t;

typedef struct commData_t{
  DataSource_t eDataSource;
  int16_t uValue;
} commData_t;
//communicatioTask --------------------------

//lcdTask -----------------------------------
typedef enum LCDMessage_t{
			  turnOnMessage,
			  bleConfig,
			  connectedStatus,
			  batteryLevel,
			  chargingStatus,
			  encoder
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
