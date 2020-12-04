#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "ring.h"

/*
To implement means functions, variables and magnitude values which are present
on the protocol but function definitions and values need to be provided
in the way most convenient and functional for the specific hardware 
characteristics in which the implementation is meant to work.

Implementation dependent means functions, variables and magnitude values which
are not present on the protocol but which 
functionality is contemplated by the protocol but which implementation can
not be encapsulated in universal fixed declarations and namings, thus
specific functions, variables and magnitude values should be created freely
in order to comply with the protocol specified functionality.

Custom means functions, variables and magnitude values which are not present
in the protocol and which functionality is not expected by the protocol,
however a particular implementation may find convinient or necessary to
implement.

Implementation dependent handles
A handle for talking to inform commandRXInterfaceProcess there's work
to do.

Implementation dependent processes:
commandRXTX communication process

Implementation dependent interrupt handlers:
encoder pulse capture (time and encoder counter)
power connection status change
 */

/*
displayInterface
commandTXInterface
commandRXInterface
 */

//Mathematical defs--------------------------------
#define BITS31 2147483648.0f
#define PI 3.14159265f
#define degToRads(x) ((x) * PI)/180.0

//-------------------------PROTOCOL--------------------------
//*******************************************************************

//To implement ENCODER hardware specs-------------------------
#define ENCODERINITIAL_VALUE 32767
#define ENCODERSTEPDISTANCEINMILLS 4084

//To implement Queue sizes------------------------------------
#define LCD_QUEUE_SIZE                20//CHANGE
#define TX_QUEUE_SIZE                 20//CHANGE

//To implement BUFFER sizes-----------------------------------
#define ENCODER_BUFFER_SIZE 256

#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
#else
#warning ENCODER_BUFFER NOT POWER OF 2
#endif

//To implement TIMING ----------------------------------------
#define ENCODER_TASK_DELAY_MS 50
#define BATTERY_FREE_TASK_DELAY_MS 20000

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

//Queue handles----------------------------------
extern QueueHandle_t lcdQueue;//CHANGE
extern QueueHandle_t uartTXQueue;//CHANGE

//Queue structures----------------------------------------
typedef enum messageTypes_t{
			    turnOnMessage,
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
}lcdData_t;//CHANGE

typedef struct uartTXData_t{
  messageTypes_t messageType;
  uint16_t traveledDistanceOrBattery;
  uint16_t meanPropulsiveVelocity;
  uint16_t peakVelocity;
} uartTXData_t;//CHANGE

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

//Ring buffer structures--------------------------
typedef struct encoderValues_t{
  uint16_t encoderCounter;
  uint32_t inputCapture;
} encoderValues_t;

//Buffers-----------------------------------------
extern encoderValues_t encoder_buffer[ENCODER_BUFFER_SIZE];

//encoderTask variables---------------------------
extern uint8_t (*goingDesiredCountDir[2])(uint32_t, uint32_t);
extern uint8_t (*hasTraveledMinDist[2])(uint32_t, uint32_t);
extern uint8_t (*hasReturnedToInitial[2])(uint32_t, uint32_t);
extern uint8_t (*newMaxRomDetected[2])(uint32_t,uint32_t);

//ISR variables-----------------------------------
extern volatile encoderValues_t encInterruptValues;
extern volatile uint16_t encoderTimerCounter;
extern volatile uint32_t capturedTime;
extern volatile uint32_t overflowCounter;

//Helper functions--------------------------------
void sendToUARTTXQueue(messageTypes_t messageType,
		       uint16_t traveledDistanceOrBattery,
		       uint16_t meanPropulsiveVelocity,
		       uint16_t peakVelocity);//CHANGE
void sendToLCDQueue(messageTypes_t messageType,
		    uint32_t displayValue);//CHANGE
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
void turnOnEncoderSensors(void);
void turnOffEncoderSensors(void);
uint32_t readBattery(void);

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
void lcdTask(void *args);//CHANGE

//*******************************************************************

//-----------------PROTOCOL IMPLEMENTATION DEPENDENT-----------------
//*******************************************************************

//TX/RX protocol delimiters----------------------
#define PROTOCOL_INITIATOR 0b01111100 // '|'
#define PROTOCOL_TERMINATOR PROTOCOL_INITIATOR

//Command RXInterfaceProcess handles-------------
extern QueueSetHandle_t communicationQueueSet;//CHANGE
extern SemaphoreHandle_t communicationSemaphore;//CHANGE

//Implementation dependent proceses--------------
void communicationTask(void *args);//CHANGE

//Interrupt handlers-----------------------------
void tim1_cc_isr(void);
void tim1_up_isr(void);
void usart1_isr(void);
void exti15_10_isr(void);

//*******************************************************************

//---------------------------CUSTOM----------------------------------
//*******************************************************************
#define SEMAPHORE_SIZE                 1
#define COMMUNICATION_QUEUE_SET_SIZE  LCD_QUEUE_SIZE + SEMAPHORE_SIZE

#define UART_RX_BUFFER_SIZE 500//CHANGE
#define PARSE_BUFFER_SIZE   70//CHANGE

//Custom Buffers---------------------------------
extern char receiveBuffer[UART_RX_BUFFER_SIZE];//CHANGE
extern char parseBuffer[PARSE_BUFFER_SIZE];//CHANGE

//Custom Buffers positions-----------------------
extern uint32_t receiveBufferPos;//CHANGE

//Custom helper functions------------------------
void printStringUART(const char myString[]);

void cleanAdvanceBuffer(char *buffer, uint32_t *bufferPosition, uint32_t bufferLength);
void getLine(void);
//*******************************************************************


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


