#include <libopencm3/stm32/gpio.h>
#include <string.h>
#include <stdlib.h>
#include "bluetooth.h"
#include "uart.h"
#include "printf.h"
#include "queueSending.h"

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

characteristicStatus_t  characteristicStatus;
charLineBuffer_t *charLineBufferPtr;
char printBuffer[100];

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...);
void advertise(void);
void unBond(void);
void unBondAdvertise(void);
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


//-------------- USART PARSING FUNCTIONS----------
int parseWCLine(const char* line);
int parseUUIDLine(const char* line);
void unlockWaitingLineParsing(charLineBuffer_t *,
			      int* waitingState, int *errorDetection);

//-------------- RN4020 configuration
void bluetoothConfig(int configuration);

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

void unBondAdvertise(void){
  runLockingCOMMAND(NULL,"U\nA\n");
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

void writeOneOneByteCharacteristic(uint8_t value0){

  runLockingCOMMAND(&characteristicStatus.isNotifying
		    ,"SHW,%04X,"
		    "%04X\n",
		    characteristicStatus.handle,
		    value0);
}

void writeTenTwoBytesCharacteristic(uint16_t value0,
				    uint16_t value1, uint16_t value2,
				    uint16_t value3, uint16_t value4,
				    uint16_t value5, uint16_t value6,
				    uint16_t value7, uint16_t value8,
				    uint16_t value9){
  
  runLockingCOMMAND(&characteristicStatus.isNotifying,
		    "SHW,%04X,"
		    "%04x"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X"
		    "%04X\n",
		    characteristicStatus.handle,
		    value0,
		    value1,
		    value2,
		    value3,
		    value4,
		    value5,
		    value6,
		    value7,
		    value8,
		    value9
		    );
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
    sendToLCDQueue(connectedStatus,1,0);
    characteristicStatus.isNotifying = 0;

    if(characteristicStatus.notificationEnabled){
      turnOffSubscription();
    }
    
    unBondAdvertise();
  
  }else if(strstr(stdLine,"Connected") != NULL){
    sendToLCDQueue(connectedStatus,1,1);
  }else if(strstr(stdLine, "Bonded") != NULL){
      
  }else if(parseUUIDLine(stdLine)){
    
  }else if(parseWCLine(stdLine)){
    
  }else if(strstr(stdLine, "CMD")!= NULL){
    bluetoothConfig(1);
    sendLS();
    //    sendToLCDQueue(turnOnMessage,0,0);
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
		| RN4020_FEATURE_SERVER_ONLY); //SERVER ONLY REDUCES OVERHEAD
    setSupportedServices(RN4020_SERVICE_USER_DEFINED );

    //SET PRIVATE SERVICE (JUST ONE IN RN4020)
    setPrivateService(private_service_UUID);

    //CONFIGURE ENCODER
    setPrivateCharacteristic(characteristic_UUID,
			     RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_NOTIFY,
			     20,
			     RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE 
			     );
    
    setName("VBT");//No more than 6 bytes when using private service

     
  }else if(configuration == 2){
    setFactoryReset(2);
  }
}
