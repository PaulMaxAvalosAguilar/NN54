#include "FreeRTOS.h"
#include "semphr.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <string.h>
#include <stdlib.h>
#include "bluetooth.h"
#include "uart.h"
#include "CUSTOM/PRINTF/printf.h"
#include "CUSTOM/MATH/abs.h"
#include "main.h"
#include "ring.h"
#include "lcd.h"

//PRIVATE SERVICE CHARACTERISTIC UUID (no more than 1 in rn4020)
const char private_service_UUID[] = "11223344556677889900AABBCCDDEEFF";

//128 CHARACTERISTIC UUID
const char characteristic_UUID[] = "010203040506070809000A0B0C0D0FFF";

//UUIDHANDLEmatchesFound BITS (should be in ascending order
//beginning from 0)
#define CHARACTERISTIC_0MATCH 0


//PRIVATE CHARACTERISTICS ALIAS
#define CORRUPTION CHARACTERISTIC_0MATCH

typedef enum{
  CMD,
  CONNECTED,
  CONNECTION_END,
  BONDED,
  ENDLS,
  AOKORERR
}actions;

uint8_t bluetoothConnected;
uint8_t encoderStarted;
typedef struct{
  uint8_t handleFound;
  uint16_t handle;
  uint8_t notificationEnabled;
  uint8_t isNotifying;
} characteristicStatus_t;
characteristicStatus_t  characteristicStatus;

charLineBuffer_t *charLineBufferPtr;
char printBuffer[100];


//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...);
void advertise(void);
void unBond(void);
void stopAdvertising(void);
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
void setName(char *string);//No >  6 bytes when using private serviceo
void turnOffSubscription(void);
void writeEncoderStartValue(void);

//-------------- USART PARSING FUNCTIONS----------
int parseWVLine(const char* line);
int parseWCLine(const char* line);
int parseUUIDLine(const char* line);
void unlockWaitingLineParsing(charLineBuffer_t *,
			      int* waitingState, int *errorDetection);

//-------------- RN4020 configuration------------
void bluetoothConfig(int configuration);

//-------------- rtos task---------------------
static void adcFreeTask(void *args __attribute__((unused)));
static void adcWaitTask(void *args __attribute__((unused)));
static void encoderTask(void *args __attribute__((unused)));


void writeEncoderValues(uint16_t value0,
			 uint16_t value1,
			 uint16_t value2){

  runLockingCOMMAND(&characteristicStatus.isNotifying
		    ,"SHW,%04X,"
		    "%02X"
		    "%04X"
		    "%04X"
		    "%04X\n",
		    characteristicStatus.handle,
		    255,
		    value0,
		    value1,
		    value2);
}

void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...){

  int notifyCheckingIsNull= 1;
  uint8_t *notifyActive = notifyChecking;
  int previousNotifyActive;
  
  if(notifyChecking != NULL){
    notifyCheckingIsNull = 0;
    previousNotifyActive = *notifyActive;
  }

  //NOTES FOR NOTIFYCHECKING CHANGES DUE TO
  //READING STATE AND ACTUAL STATE OF BLUETOOTH MODULE

  //From 0 -> 1 there won't be a second AOK waiting,
  //if second AOKs come remaining AOKS will be subsecuently cleaned in main loop or
  //in this function
  //if no second AOKs come there is no problem

  //From 1 -> 0 there will be a second AOK waiting,
  //If second AOKs come, if interpreted by unlockWaitingLineParsing waitingState
  //will be broken
  
  //If no second AOKs come, if in second wait and realNotifications is not Active
  //or changes by genericLineParsing while looping waiting state will be broken
  //by second wait 1->0 lock detection section
  
  //Clean Serial
  while(serialAvailable()){
    charLineBufferPtr = forceReadCharLineUsart();
    genericLineParsing(charLineBufferPtr);
  }



  //Send Command
  va_list args;
  va_start(args, format);
  vsnprintf(printBuffer,100,format,args);
  va_end(args);
  printString(printBuffer);
  //Send Command

  int waitingMax = 2;
  int errorDetection = 0;
  //waiting loop
  for(int i = 0; i < waitingMax; i ++){
    int waitingState = 1;
    
    while(waitingState){
      if(serialAvailable()){
	charLineBufferPtr = forceReadCharLineUsart();
	genericLineParsing(charLineBufferPtr);
	unlockWaitingLineParsing(charLineBufferPtr, &waitingState, &errorDetection);

      }
      //second wait 1->0 lock detection section
      if( (i == 1) && (!(*notifyActive))){
	waitingState =0; 
      }
      
    }

    if(notifyCheckingIsNull){
      i = waitingMax;
    }else if(!(previousNotifyActive)){
      i = waitingMax;
    }else if(errorDetection){
      i = waitingMax;
    }

  }
  
}

void advertise(){
  runLockingCOMMAND(NULL,"A\n");
}

void unBond(){
  runLockingCOMMAND(NULL,"U\n");
}

void stopAdvertising(){
 runLockingCOMMAND(NULL,"Y\n");
}

void sendLS(){
  characteristicStatus.handleFound = 0;
  runLockingCOMMAND(NULL,"LS\n");
}

void cleanPrivateService(){
  runLockingCOMMAND(NULL,"PZ\n");
}

void setFactoryReset(uint8_t arg){
  runLockingCOMMAND(NULL,"SF,%d\n",arg);
}

void setFeatures(uint32_t arg){
  runLockingCOMMAND(NULL,"SR,%08lX\n",arg);
}

void setSupportedServices(uint32_t arg){
  runLockingCOMMAND(NULL,"SS,%08lX\n",arg);
}

void setPrivateService(const char *arg){
  runLockingCOMMAND(NULL,"PS,%s\n",arg);
}

void setPrivateCharacteristic(const char *service,
			      uint8_t properties,
			      uint8_t dataSize,
			      uint8_t securityFlag){
  if(securityFlag != 0){

    runLockingCOMMAND(NULL,"PC,%s,%02X,%02X,%02X\n",
		      service,
		      properties,
		      dataSize,
		      securityFlag);
  
  }else{

    runLockingCOMMAND(NULL,"PC,%s,%02X,%02X\n",
		      service,
		      properties,
		      dataSize);   
  }
}

void setName( char *string){
  runLockingCOMMAND(NULL,"SN,%s\n", string);
}



void turnOffSubscription(void){
  uint16_t subscriptionHandle =
    characteristicStatus.handle + 1;
  runLockingCOMMAND(NULL
		    ,"SHW,%04X,0000\n",
		    subscriptionHandle);
}

void writeEncoderStartValue(){
  runLockingCOMMAND(&characteristicStatus.isNotifying
		    ,"SHW,%04X,"
		    "%02X\n",
		    characteristicStatus.handle,
		    200);
}

int parseWVLine(const char* line){
  if(strncmp(line,"WV,",3) != 0){
    return 0;
  }

  char cbyteBuffer[3];

  //Begin at line 10 since 00 is expected from sender:
  //WV,FFFF,00FFBBBBCCDD
  strncpy(cbyteBuffer, line+10,2);
  cbyteBuffer[2] = '\n';
  uint8_t messageType = strtol(cbyteBuffer, NULL, 16);

  if(messageType == 1){

    encoderTaskParamTypes_t dataToSend;

    //Get minDistToTravel
    strncpy(cbyteBuffer, line+12,2);
    cbyteBuffer[2] = '\n';
    dataToSend.minDistToTravel = strtol(cbyteBuffer,NULL,16);

    //Get desiredCountDir
    strncpy(cbyteBuffer, line+14,2);
    cbyteBuffer[2] = '\n';
    dataToSend.desiredCounterDirection = strtol(cbyteBuffer,NULL,16);

    //Get desiredRepDir
    strncpy(cbyteBuffer, line+16,2);
    cbyteBuffer[2] = '\n';
    dataToSend.desiredRepDirection = strtol(cbyteBuffer,NULL,16);

    //Should go before creating encoderTask------------------------------
    (adcWaitTaskHandle != NULL)? vTaskSuspend(adcWaitTaskHandle): "";

    gpio_clear(GPIOA, GPIO4);//Activate sensors, should go before initializeTimers()
    initializeTimers();//Clock gating, should go before clearing ring buffer
    ring_buffer_clear(&encoder_ring);//Clear ring buffer
    writeEncoderStartValue();//sendToUARTTXQueue(encoderStart,0,0,0) equivalent

    //Should go before creating encoderTask------------------------------
    createTask(encoderTask, "encoderTask",200,&dataToSend, 4,
	       &encoderTaskHandle);
    
  }else if(messageType ==2){

    deleteTask(&encoderTaskHandle);
    //Should go after deleting encoderTask-------------------------------
    //    sendToUARTTXQueue(encoderStop,0,0,0);
    stopTimers();
    gpio_set(GPIOA, GPIO4);
    vTaskResume(adcWaitTaskHandle);
    //Should go after deleting encoderTask-------------------------------
    
  }else if(messageType == 3){
    (adcWaitTaskHandle != NULL)? xTaskNotifyGive(adcWaitTaskHandle) : 0;
  }

  return 1;
}

int parseWCLine(const char* line){

  //Check if received line contains a WC message
  if(strncmp(line, "WC,",3) != 0){
    return 0;
  }

  //Copy handle string part to a buffer
  char handleStr[5];
  strncpy(handleStr, line + 3, 4);
  handleStr[4] = '\0';

  //Convert string to a number
  uint16_t handle = strtol(handleStr, NULL, 16);

  //Compare received handle to stored handle

  //If received handle is equal to stored handle
  if((characteristicStatus.handle + 1)
     == handle){

    //Read value in received line
    const char* dataStringPtr = line + 8;
    if((strncmp(dataStringPtr,"0100",4) == 0) ||
       (strncmp(dataStringPtr,"0200",4) == 0)
       ){
      characteristicStatus.isNotifying=1;
    }else{
      characteristicStatus.isNotifying=0;
    }
  }    


  return 1;
}

int parseUUIDLine(const char* line) {

  //Check if line is a UUID Line
  if (strncmp(line, "  ", 2) != 0) {
    return 0;
    
  }

  //Identify UUID
  const char* startOfUUIDPtr = line + 2;
  const char* firstCommaPtr = strchr(startOfUUIDPtr, ',');
  int UUIDlength = firstCommaPtr - startOfUUIDPtr;
  char UUIDline[50];
  
  strncpy(UUIDline, startOfUUIDPtr, UUIDlength);
  UUIDline[UUIDlength] = '\0';
  
  //Identify HANDLE
  char handleStr[5];
  
  strncpy(handleStr, firstCommaPtr + 1, 4);
  handleStr[4] = 0;
  uint16_t handle = strtol(handleStr, NULL, 16);

  //If handle is not set, set match
  if(characteristicStatus.handleFound == 0 ){
    characteristicStatus.handleFound = 1;
    characteristicStatus.handle = handle;
  }else{
    //Notification or indication handle was received
    //indicate subscription is available for the characteristic
    characteristicStatus.notificationEnabled = 1;
  }
  
  return 1;
}


void unlockWaitingLineParsing(charLineBuffer_t *clb,
				     int* waitingState, int *errorDetection){
  char *buffer = clb->buf;
  uint16_t *terminator = &clb->terminatorcharposition;
  //Change to standard null terminator
  buffer[*terminator] = '\0';

  char* stdLine = buffer;
  
  //INTERPRET
  if((strstr(stdLine, "AOK") != NULL) ||
     (strstr(stdLine, "END") != NULL)      
     ){
    *waitingState = 0;
      
  }else if (strstr(stdLine,"ERR") != NULL){
    //If an ERR is received no second aok will come therefore we skip
    //otherwise communication is gonna lock waiting for no coming aok
    *errorDetection = 1;
    *waitingState = 0;
  }
  
  //Return to line as null terminator
  buffer[*terminator] = '\n';
}


void genericLineParsing(charLineBuffer_t *clb){
  char *buffer = clb->buf;
  uint16_t *terminator = &clb->terminatorcharposition;
  //Change to standard null terminator
  buffer[*terminator] = '\0';

  char* stdLine = buffer;
  
  //INTERPRET
  if(strstr(stdLine, "Connection End") != NULL){

    //In case no stop was ever received-----------------------------------
    deleteTask(&encoderTaskHandle);
    stopTimers();//Should go after deleting encoder task
    gpio_set(GPIOA, GPIO4);//Should go after delting encoder task
    //In case no stop was ever received-----------------------------------
    sendToLCDQueue(connectedStatus, 0);
    deleteTask(&adcWaitTaskHandle);
    createTask(adcFreeTask,"adcFreeTask",100,NULL,1,&adcFreeTaskHandle);


    characteristicStatus.isNotifying = 0;

    if(characteristicStatus.notificationEnabled){
      turnOffSubscription();
    }

    unBond();
    advertise();
  
  }else if(strstr(stdLine,"Connected") != NULL){
    sendToLCDQueue(connectedStatus,1);
    deleteTask(&adcFreeTaskHandle);
    createTask(adcWaitTask,"adcWaitTask",100,NULL,1,&adcWaitTaskHandle);

  }else if(strstr(stdLine, "Bonded") != NULL){
      
  }else if(parseWVLine(stdLine)){

  }else if(parseUUIDLine(stdLine)){
    
  }else if(parseWCLine(stdLine)){
    
  }else if(strstr(stdLine, "CMD")!= NULL){
    sendToLCDQueue(bleConfig,0);
    bluetoothConfig(0);
    sendLS();
    stopAdvertising();
    advertise();	  
  }

  //Return to line as null terminator
  buffer[*terminator] = '\n';
}

void bluetoothConfig(int configuration){

  if(configuration == 1){

    setFactoryReset(1);//Should go first
    cleanPrivateService();//Should go after Factory reset for services to show
    setFeatures(RN4020_FEATURE_PERIFERAL | RN4020_FEATURE_DO_NOT_SAVE_BONDING
		| RN4020_FEATURE_NO_DIRECT_ADVERTISE
		| RN4020_FEATURE_SERVER_ONLY); //SERVER ONLY REDUCES OVERHEAD
    setSupportedServices(RN4020_SERVICE_USER_DEFINED );

    //SET PRIVATE SERVICE (JUST ONE IN RN4020)
    setPrivateService(private_service_UUID);

    //CONFIGURE ENCODER
    setPrivateCharacteristic(characteristic_UUID,
			     RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_NOTIFY |
			     RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_WRITE,
			     20,
			     RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE 
			     );
    
    setName("VBT");//No more than 6 bytes when using private service

     
  }else if(configuration == 2){
    setFactoryReset(2);
  }
}

static void adcFreeTask(void *args __attribute__((unused))) {

  uint32_t adcData = 0;
  
  for(;;){
    
    adcData = readADC();

    sendToLCDQueue(batteryLevel,adcData);

    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

static void adcWaitTask(void *args __attribute__((unused))){
  uint32_t adcData = 0;
  
  for(;;){

    ulTaskNotifyTake(pdTRUE,portMAX_DELAY);//Should go first
    
    adcData = readADC();
    sendToLCDQueue(batteryLevel, adcData);
    //    sendToUARTTXQueue(batteryLevel, (uint16_t)adcData, 0,0);
  }  
}

static __attribute__((optimize("O0"))) void encoderTask(void *args __attribute__((unused))){

  encoderValues_t receiveEncValues;  
  encoderTaskParamTypes_t taskParam;
  taskParam = *(encoderTaskParamTypes_t *)args;

  //Direction variables
  uint32_t desiredDirFollowed = 0;

  //Distance calculation variables
  int32_t lastPosition = ENCODERINITIAL_VALUE;
  uint32_t elapsedPosition = 0;
  uint32_t elapsedDistance = 0;

  //Time calculation variables
  uint32_t lastTime = 0;
  uint32_t elapsedTime = 0;

  //Velocity Calulcation Variables
  uint32_t currentVelocity = 0;
  uint32_t lastVelocity = 0;
  
  uint32_t meanPropVelCount = 0;
  uint32_t meanPropVel = 0;
  uint32_t peakVel = 0;

  //ROM variables
  uint16_t maxROM = ENCODERINITIAL_VALUE;

  //Repetition algorithm variables
  uint32_t canCountRep = 0;
  uint32_t  minDistTraveled= 0;

  for(;;){

    while(ring_buffer_get(&encoder_ring, &receiveEncValues) != -1){

      //VELOCITY ALGORITHM-------------------------------------------------------------------
      if( (*goingDesiredCountDir[taskParam.desiredCounterDirection])(receiveEncValues.encoderCounter,
					   lastPosition) ){
	desiredDirFollowed = 1;
	elapsedPosition = custom_abs((int32_t)receiveEncValues.encoderCounter - lastPosition);	
      }else{
	desiredDirFollowed = 0;
      }

      lastPosition = receiveEncValues.encoderCounter;
      elapsedTime = receiveEncValues.inputCapture - lastTime;
      lastTime = receiveEncValues.inputCapture;
      
      if(desiredDirFollowed){
	elapsedDistance = ENCODERSTEPDISTANCEINMILLS * elapsedPosition;
	currentVelocity = (elapsedDistance * 100) / elapsedTime;

	//Mean Propulsive Velocity Calculation
	if(currentVelocity > lastVelocity){
	  meanPropVelCount++;
	  meanPropVel += currentVelocity;
	}
	lastVelocity = currentVelocity;
	
	//Peak Velocity Calculation
	if(currentVelocity > peakVel) peakVel = currentVelocity;
      }
      //VELOCITY ALGORITHM-----------------------------------------------------------------

      //REPETITION ALGORITHM---------------------------------------------------------------

      if( (*newMaxRomDetected[taskParam.desiredRepDirection])(receiveEncValues.encoderCounter,
								 maxROM) ){
	maxROM = receiveEncValues.encoderCounter;
      }


      if((*hasTraveledMinDist[taskParam.desiredRepDirection])
	(receiveEncValues.encoderCounter, taskParam.minDistToTravel)){
	minDistTraveled = 1;
      }


      if(minDistTraveled && (*hasReturnedToInitial[taskParam.desiredRepDirection])
	 (receiveEncValues.encoderCounter, taskParam.minDistToTravel)){
	canCountRep = 1;
      }
      
      if(canCountRep){

	sendToUARTTXQueue(encoderData,
			  (custom_abs(maxROM-ENCODERINITIAL_VALUE)* ENCODERSTEPDISTANCEINMILLS)/1000,
			  meanPropVel/meanPropVelCount,
			  peakVel);
	
	//Velocity Calulcation Variables reset
	lastVelocity = 0;
  
	meanPropVelCount = 0;
	meanPropVel = 0;
	peakVel = 0;

	//ROM variables reset
	maxROM = ENCODERINITIAL_VALUE;

	//Repetition algorithm variables
	canCountRep = 0;
	minDistTraveled= 0;
      }
      //REPETITION ALGORITHM---------------------------------------------------------------
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));    
  }
}

