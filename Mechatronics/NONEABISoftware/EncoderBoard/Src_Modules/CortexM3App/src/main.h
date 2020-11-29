#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "ring.h"

//Mathematical defs--------------------------------
#define BITS31 2147483648.0f
#define PI 3.14159265f
#define degToRads(x) ((x) * PI)/180.0

//-------------------------CUSTOMIZABLE PART-------------------------
//*******************************************************************

//-----------------------------------------------
#define ENCODERINITIAL_VALUE 32767
#define ENCODERSTEPDISTANCEINMILLS 4084

//Queue sizes------------------------------------
#define LCD_QUEUE_SIZE                20
#define TX_QUEUE_SIZE                 20
#define SEMAPHORE_SIZE                 1
#define COMMUNICATION_QUEUE_SET_SIZE  LCD_QUEUE_SIZE + SEMAPHORE_SIZE

//BUFFER sizes-----------------------------------
#define ENCODER_BUFFER_SIZE 256
#define UART_RX_BUFFER_SIZE 500
#define PARSE_BUFFER_SIZE   70

#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
#else
#warning ENCODER_BUFFER NOT POWER OF 2
#endif

//TIMING ----------------------------------------
#define ENCODER_TASK_DELAY_MS 50
#define BATTERY_FREE_TASK_DELAY_MS 20000


//*******************************************************************

//-------------------------PROTOCOL IMPLEMENTATION-------------------
//*******************************************************************
//Protocol numbers-------------------------------
typedef enum codes_t{
  peripherealCode_EncoderStart = 1,
  peripherealCode_EncoderStop = 2,
  peripherealCode_EncoderBattery = 3,
  centralCode_encoderData = 64,
  centralCode_encoderStart = 65,
  centralCode_encoderBattery = 66,
  centrlCode_encoderStop = 67
}codes_t;

#define PROTOCOL_INITIATOR 0b01111100 // '|'
#define PROTOCOL_TERMINATOR PROTOCOL_INITIATOR

//*******************************************************************

//Queue handles----------------------------------
extern QueueHandle_t lcdQueue;
extern QueueHandle_t uartTXQueue;
extern QueueSetHandle_t communicationQueueSet;

//Binary semaphore handles-----------------------
extern SemaphoreHandle_t communicationSemaphore;

//Queue structures-------------------------------
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

typedef struct lcdData_t{
  messageTypes_t messageType;
  uint32_t displayValue;  
}lcdData_t;

typedef struct uartTXData_t{
  messageTypes_t messageType;
  uint16_t traveledDistanceOrBattery;
  uint16_t meanPropulsiveVelocity;
  uint16_t peakVelocity;
} uartTXData_t;

//Tasks Handles-----------------------------------
extern TaskHandle_t encoderTaskHandle;
extern TaskHandle_t batteryFreeTaskHandle;
extern TaskHandle_t batteryWaitTaskHandle;

//Task structures---------------------------------
typedef struct encoderTaskParamTypes_t{
  uint32_t minDistToTravel;
  uint32_t desiredCounterDirection;
  uint32_t desiredRepDirection;
}encoderTaskParamTypes_t;

//Ring buffer handles-----------------------------
extern ring_t encoder_ring;

//RingBuffer structures---------------------------
typedef struct encoderValues_t{
  uint16_t encoderCounter;
  uint32_t inputCapture;
} encoderValues_t;

//Buffers-----------------------------------------
extern encoderValues_t encoder_buffer[ENCODER_BUFFER_SIZE];
extern char receiveBuffer[UART_RX_BUFFER_SIZE];
extern char parseBuffer[PARSE_BUFFER_SIZE];

//Buffers' positions------------------------------
extern uint32_t receiveBufferPos;

//encoderTask variables---------------------------
extern uint8_t (*goingDesiredCountDir[2])(uint32_t, uint32_t);
extern uint8_t (*hasTraveledMinDist[2])(uint32_t, uint32_t);
extern uint8_t (*hasReturnedToInitial[2])(uint32_t, uint32_t);
extern uint8_t (*newMaxRomDetected[2])(uint32_t,uint32_t);

//ISR variables-----------------------------------
extern volatile encoderValues_t encInterruptValues;
extern volatile uint16_t tim2Counter;
extern volatile uint32_t capturedTime;
extern volatile uint32_t overflowCounter;

//Helper functions--------------------------------
void sendToUARTTXQueue(messageTypes_t messageType,
		       uint16_t traveledDistanceOrBattery,
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
void encodeTwoBytes(char *twoByteBuffer, uint32_t numberToEncode);
uint16_t decodeTwoBytes(uint8_t msb, uint8_t lsb);
void encodeOneByte(char *oneByteBuffer, unsigned int numberToEncode);
uint8_t decodeOneByte(uint8_t byte);

//To implement helper functions
void initializeTimers(void);
void stopTimers(void);
uint32_t readBattery(void);
void printStringUART(const char myString[]);

//Custom helper functions
void cleanAdvanceBuffer(char *buffer, uint32_t *bufferPosition, uint32_t bufferLength);
void getLine(void);

//EncoderHelper functions-------------------------
uint8_t descendente(uint32_t a, uint32_t b);
uint8_t ascendente(uint32_t a, uint32_t b);
uint8_t minDistTraveledDes(uint32_t counter, uint32_t minDistToTravel);
uint8_t minDistTraveledAs(uint32_t counter, uint32_t minDistToTravel);
uint8_t returnedToInitialDes(uint32_t counter, uint32_t minDistToTravel);
uint8_t returnedToInitialAs(uint32_t counter, uint32_t minDistToTravel);
uint8_t maxRomAs(uint32_t counter, uint32_t lastMaxDist);
uint8_t maxRomDes(uint32_t counter, uint32_t lastMaxDist);

//Processes---------------------------------------
void encoderTask(void *args);
void batteryFreeTask(void *args);
void batteryWaitTask(void *args);

//To implement processes
void lcdTask(void *args);
void communicationTask(void *args);

//Interrupt handlers------------------------------
void tim1_cc_isr(void);
void tim1_up_isr(void);
void usart1_isr(void);
void exti15_10_isr(void);

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter()			\
  KIN1_DEMCR |= KIN1_TRCENA_BIT
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
 
#define KIN1_ResetCycleCounter()		\
  KIN1_DWT_CYCCNT = 0
/*!< Reset cycle counter */
 
#define KIN1_EnableCycleCounter()		\
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
/*!< Enable cycle counter */
 
#define KIN1_DisableCycleCounter()		\
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
/*!< Disable cycle counter */
 
#define KIN1_GetCycleCounter()			\
  KIN1_DWT_CYCCNT
/*!< Read cycle counter register */

#endif


