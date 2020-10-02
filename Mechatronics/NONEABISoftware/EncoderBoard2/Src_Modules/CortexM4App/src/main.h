#ifndef MAIN_H
#define MAIN_H

//Mathematical defs--------------------------
#define BITS31 2147483648.0f
#define PI 3.14159265f
#define degToRads(x) ((x) * PI)/180.0

//UART RX------------------------------------
#define UART_RX_BUFFER_LEN 256
extern char receiveBuffer[UART_RX_BUFFER_LEN];


//Queue and Semaphore handles----------------
extern SemaphoreHandle_t adcSemaphore;
extern SemaphoreHandle_t encoderSemaphore;

extern QueueHandle_t communicationQueue;
extern QueueHandle_t lcdQueue;
extern QueueSetHandle_t communicationQueueSet;


//Queue and Semaphore sizes------------------
#define LCD_QUEUE_SIZE                20

//lcdTask -----------------------------------
typedef enum LCDMessage_t{
			  connectedStatus,
			  batteryLevel,
			  chargingStatus
}LCDMessage_t;

typedef struct lcdData_t{
  LCDMessage_t messageType;
  uint32_t displayValue;  
}lcdData_t;
//lcdTask -----------------------------------

void sendToLCDQueue(LCDMessage_t messageType,
		    uint32_t displayValue);


#endif
