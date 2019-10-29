#include "queueSending.h"

QueueHandle_t communicationQueue;
QueueHandle_t lcdQueue;


void sendToCommunicationQueue(DataSource_t eDataSource, uint16_t uValue){
  commData_t dataStruct;
  dataStruct.eDataSource = eDataSource;
  dataStruct.uValue = uValue;

  xQueueSendToBack(communicationQueue, &dataStruct, 0);
}

void sendToLCDQueue(LCDMessage_t messageType, uint8_t position,
		      uint32_t displayValue){
  lcdData_t dataToSend;
  dataToSend.messageType = messageType;
  dataToSend.position = position;
  dataToSend.displayValue = displayValue;

  xQueueSendToBack(lcdQueue,&dataToSend,0);
}
