#include "queueSending.h"

SemaphoreHandle_t adcSemaphore;
SemaphoreHandle_t encoderSemaphore;

QueueHandle_t communicationQueue;
QueueHandle_t lcdQueue;
QueueSetHandle_t communicationQueueSet;

void sendToCommunicationQueue(DataSource_t eDataSource,
			      uint16_t traveledDistanceOrADC,
			      uint16_t meanPropulsiveVelocity,
			      uint16_t peakVelocity){
  commData_t dataStruct;
  dataStruct.eDataSource = eDataSource;
  dataStruct.traveledDistanceOrADC = traveledDistanceOrADC;
  dataStruct.meanPropulsiveVelocity = meanPropulsiveVelocity;
  dataStruct.peakVelocity = peakVelocity;

  xQueueSendToBack(communicationQueue, &dataStruct, 0);
}

void sendToLCDQueue(LCDMessage_t messageType,
		      uint32_t displayValue){
  lcdData_t dataToSend;
  dataToSend.messageType = messageType;
  dataToSend.displayValue = displayValue;

  xQueueSendToBack(lcdQueue,&dataToSend,0);
}
