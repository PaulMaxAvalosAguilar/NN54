#ifndef COMMUNICACTION
#define COMMUNICATION

#include <avr/pgmspace.h>


//------------APPLICATION DEPENDENT DEFINED MACROS-------------------
//This values along with bluetoothConfig should be modified when
//adding or removing characteristics or services
//Characteristics should also be configured in bluetoothConfig
//function for this to work properly
//Up to 8 private characteristics, subscription
//occupy 2 slots

//PRIVATE SERVICE CHARACTERISTIC UUID (no more than 1 in rn4020)
extern const char private_service_UUID[] PROGMEM;

#define SERVICES_NUMBER 1
extern PGM_P const services_table[] PROGMEM;

//128 AND 16 BIT USED CHARACTERISTICS UUIDS (strings should be
//added to characteristics_table)
extern const char characteristic_UUID_0[] PROGMEM;
extern const char characteristic_UUID_1[] PROGMEM;
extern const char characteristic_UUID_2[] PROGMEM;
extern const char characteristic_UUID_3[] PROGMEM;

#define CHARACTERISTICS_NUMBER 4
extern PGM_P const characteristics_table[] PROGMEM;

//UUIDHANDLEmatchesFound BITS (should be in ascending order
//beginning from 0)
#define CHARACTERISTIC_0MATCH 0
#define CHARACTERISTIC_1MATCH 1
#define CHARACTERISTIC_2MATCH 2
#define CHARACTERISTIC_3MATCH 3

//PRIVATE CHARACTERISTICS ALIAS
#define CORRUPTION CHARACTERISTIC_0MATCH
#define ENCODER_VALUE CHARACTERISTIC_1MATCH
#define BATTERYCHARGING_VALUE CHARACTERISTIC_2MATCH
#define BATTERYLEVEL_VALUE CHARACTERISTIC_3MATCH
//-----------APPLICATION DEPENDENT DEFINED MACROS-------------------

#if CHARACTERISTICS_NUMBER <= 8
typedef uint8_t characNum;
#else
typedef uint16_t characNum;
#endif

typedef struct{
  characNum UUIDHandlematchesFound;//each bit represents a characteristic
  uint8_t allUUIDHandlesMatchesFound;
}rn4020_state;

//PRIVATE HANDLES
typedef struct{
  uint16_t privateHandle[CHARACTERISTICS_NUMBER];
  uint8_t privateHandleNotifyActive[CHARACTERISTICS_NUMBER];
  characNum subscriptionEnabled;//each bit represents a characteristic
  int length;//Modified in sendLS and UUIDHandleMatchFound
} handles;

typedef enum{
  CMD,
  CONNECTED,
  CONNECTION_END,
  BONDED,
  ENDLS,
  AOKORERR
}actions;

typedef struct{
  char buf[128];
  int terminatorcharposition;
}charLineBuffer_t;

handles privateHandles;
rn4020_state rn4020;
charLineBuffer_t charLineBuffer;

//-------------- IOMANIP INITIALIZATION----------
void initCommunication();

//-------------- BLUETOOTH COMMANDS---------------
//Only pair characters can be send (one byte in hex) with
//writing characteristics functions other will be ignored
void bluetoothConfig(int configuration);
void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...);
void advertise();
void unBond();
void sendLS();
void cleanPrivateService();
void setFactoryReset(uint8_t arg);
void setFeatures(uint32_t arg);
void setSupportedServices(uint32_t arg);
void setPrivateService(char *arg);
void setPrivateCharacteristic(char *service,
			      uint8_t properties,
			      uint8_t dataSize,
			      uint8_t securityFlag);
void setName(char *string);//No >  6 bytes when using private service
void writeOneOneByteCharacteristic(unsigned int handle, uint8_t value0);
void writeTenTwoBytesCharacteristic(unsigned int handle, uint16_t value0,
				    uint16_t value1, uint16_t value2,
				    uint16_t value3, uint16_t value4,
				    uint16_t value5, uint16_t value6,
				    uint16_t value7, uint16_t value8,
				    uint16_t value9);
void turnOffSubscription(int handle);

//-------------- USART PARSING FUNCTIONS----------
int RN4020_parseHandleWCLine(const char* line);
void UUIDHandleMatchFound(uint8_t UUIDHandlematchFound, uint16_t handle);
int RN4020_parseHandleUUIDLine(const char* line);
void unlockWaitingLineParsing(charLineBuffer_t *,
			      int* waitingState, int *errorDetection);
void genericLineParsing(charLineBuffer_t *);

//-------------- SPI FUNCTIONS--------------------
void OLED_puts(const char *g, int ypos);

#endif
