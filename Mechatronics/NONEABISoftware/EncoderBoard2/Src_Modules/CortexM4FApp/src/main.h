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
extern SemaphoreHandle_t uartRXSemaphore;
extern QueueHandle_t TXQueue;
extern QueueHandle_t lcdQueue;

//Queue and Semaphore sizes------------------
#define LCD_QUEUE_SIZE                20
#define TX_QUEUE_SIZE                 20

//uartTXTask -------------------------
typedef enum DataSource_t{
			  adcSender,
			  encoderSender
}DataSource_t;

typedef struct commData_t{
  DataSource_t eDataSource;
  uint16_t traveledDistanceOrADC;
  uint16_t meanPropulsiveVelocity;
  uint16_t peakVelocity;
} commData_t;

//uartRXTask --------------------------

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

//connection State machine
extern uint32_t bluetoothConnected;
extern uint32_t minDistToTravel;
extern uint32_t desiredCounterDirection;
extern uint32_t desiredRepDir;

void sendToLCDQueue(LCDMessage_t messageType,
		    uint32_t displayValue);
void printString(const char myString[]);
void setBLEConnected(uint8_t boolean);
uint8_t getBLEConnected(void);

#endif
