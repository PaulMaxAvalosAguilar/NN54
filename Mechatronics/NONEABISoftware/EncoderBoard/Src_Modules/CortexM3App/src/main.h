#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "ring.h"

#define ENCODERINITIAL_VALUE 32767
#define ENCODERSTEPDISTANCEINMILLS 4084

//Mathematical defs-----------------------------
#define BITS31 2147483648.0f
#define PI 3.14159265f
#define degToRads(x) ((x) * PI)/180.0

//BUFFER LENGTHS-----------------------------------
#define UART_RX_BUFFER_LEN 256
extern char receiveBuffer[UART_RX_BUFFER_LEN];

#define ENCODER_BUFFER_LEN 256
//put extern char encoderBuffer[encoder_BUFFER_LEN];

#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
#else
#warning ENCODER_BUFFER NOT POWER OF 2
#endif

//Queue sizes-------------------------------------
#define LCD_QUEUE_SIZE                20
#define TX_QUEUE_SIZE                 20
#define SEMAPHORE_SIZE                 1
#define COMMUNICATION_QUEUE_SET_SIZE  LCD_QUEUE_SIZE + SEMAPHORE_SIZE

//Ring buffer handles---------------------------------
typedef struct charLineBuffer_t{
  char buf[100];
  uint16_t terminatorcharposition;
}charLineBuffer_t;//DELETE!!!
extern ring_t encoder_ring;

//Queue handles---------------------------------
extern QueueHandle_t lcdQueue;
extern QueueHandle_t uartTXQueue;
extern QueueSetHandle_t communicationQueueSet;

//Queue structures------------------------------
typedef enum messageTypes_t{
			    turnOnMessage,
			    bleConfig,
			    encoderData,
			    encoderStart,
			    encoderStop,
			    connectedStatus,
			    batteryLevel,
			    chargingStatus
}messageTypes_t;

typedef struct uartTXData_t{
  messageTypes_t messageType;
  uint16_t traveledDistanceOrADC;
  uint16_t meanPropulsiveVelocity;
  uint16_t peakVelocity;
} uartTXData_t;

typedef struct lcdData_t{
  messageTypes_t messageType;
  uint32_t displayValue;  
}lcdData_t;

//Tasks Handles-----------------------------------
extern TaskHandle_t encoderTaskHandle;
extern TaskHandle_t adcFreeTaskHandle;
extern TaskHandle_t adcWaitTaskHandle;

//Task structures---------------------------------
typedef struct encoderTaskParamTypes_t{
  uint32_t minDistToTravel;
  uint16_t desiredCounterDirection;
  uint16_t desiredRepDirection;
}encoderTaskParamTypes_t;

//RingBuffer structures------------------------------
typedef struct encoderValues_t{
  uint16_t encoderCounter;
  uint32_t inputCapture;
} encoderValues_t;

//Encoder variables--------------------------
extern uint8_t (*goingDesiredCountDir[2])(uint32_t, uint32_t);
extern uint8_t (*hasTraveledMinDist[2])(uint32_t, uint32_t);
extern uint8_t (*hasReturnedToInitial[2])(uint32_t, uint32_t);
extern uint8_t (*newMaxRomDetected[2])(uint32_t,uint32_t);

//Helper functions---------------------------
void sendToUARTTXQueue(messageTypes_t messageType,
		       uint16_t traveledDistanceOrADC,
		       uint16_t meanPropulsiveVelocity,
		       uint16_t peakVelocity);
void sendToLCDQueue(messageTypes_t messageType,
		    uint32_t displayValue);
void createTask(TaskFunction_t pvTaskCode,
		const char *const pcName,
		configSTACK_DEPTH_TYPE usStackDepth,
		void *pvParameters,
		UBaseType_t uxPriority,
		TaskHandle_t *taskHandle);
void deleteTask(TaskHandle_t *pxTask);
void initializeTimers(void);
void stopTimers(void);
uint32_t readADC(void);
void printStringUART(const char myString[]);
int serialAvailable(void);//DELETE!!!
char get_char(void);//DELETE!!!
charLineBuffer_t *forceReadCharLineUsart(void);//DELETE!!!

//EncoderHelper functions-------------------------
uint8_t descendente(uint32_t a, uint32_t b);
uint8_t ascendente(uint32_t a, uint32_t b);
uint8_t minDistTraveledDes(uint32_t counter, uint32_t minDistToTravel);
uint8_t minDistTraveledAs(uint32_t counter, uint32_t minDistToTravel);
uint8_t returnedToInitialDes(uint32_t counter, uint32_t minDistToTravel);
uint8_t returnedToInitialAs(uint32_t counter, uint32_t minDistToTravel);
uint8_t maxRomAs(uint32_t counter, uint32_t lastMaxDist);
uint8_t maxRomDes(uint32_t counter, uint32_t lastMaxDist);

#endif


