#ifndef HOLA_H
#define HOLA_H

#define BAUD 115200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h>

void go();

typedef struct{
    size_t s_elem;
    uint16_t n_elem;
    void *buf;
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_t;

int ring_buffer_init(ring_t *ring, void *buf, size_t buf_elements_size, size_t arrayByteSize);

int ring_buffer_full(ring_t *ring);
int ring_buffer_empty(ring_t *ring);

void memcopy(void *destref, const void *srcref, size_t count);

int ring_buffer_put(ring_t *ring, const void *data);
int ring_buffer_get(ring_t *ring, void *data);

#ifndef F_CPU
  #warning F_CPU is undefined, USART may not work correctly without this
#endif

#define BAUD_CALC(x) ((F_CPU+(x)*8UL) / (16UL*(x))-1UL) // macro calculating precise UBRR value
#define DOUBLE_BAUD_CALC(x) ((F_CPU+(x)*4UL) / (8UL*(x))-1UL) // macro calculating UBRR value for double speed

#define BAUD_TOL 2

#define UBRR_VALUE BAUD_CALC(BAUD)

#if 100 * (F_CPU) > \
  (16 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) + (BAUD) * (BAUD_TOL))
#  ifndef USE_2X
#  define USE_2X
#  endif 
#elif 100 * (F_CPU) < \
  (16 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) - (BAUD) * (BAUD_TOL))
#  ifndef USE_2X
#  define USE_2X
#  endif
#endif

#ifdef USE_2X
#undef UBRR_VALUE
#define UBRR_VALUE DOUBLE_BAUD_CALC(BAUD)

#  if 100 * (F_CPU) > \
  (8 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) + (BAUD) * (BAUD_TOL))
#  warning "Baud rate achieved is higher than allowed with 2X"
#  endif

#  if 100 * (F_CPU) < \
  (8 * ((UBRR_VALUE) + 1)) * (100 * (BAUD) - (BAUD) * (BAUD_TOL))
#  warning "Baud rate achieved is lower than allowed with 2X"
#  endif
#endif /*USE_DOUBLE_SPEED*/

#ifdef UBRR_VALUE
   /* Check for overflow */
#  if UBRR_VALUE >= (1 << 12)
#    warning "UBRR value overflow"
#  endif
#endif

void uart_init(uint16_t ubrr_value);
void printString(const char myString[]);
int put_char(char data, FILE *stream);
int serialAvailable(void);
char get_char(void);

#define uart_autoinit() uart_init(UBRR_VALUE)

#define FDEV_SETUP_STREAM_U(p, g, f, u) \
  { \
    .put = p, \
    .get = g, \
    .flags = f, \
    .udata = u, \
  }

void initEncoder(void);
void ext_interruptConfig(void);
void timer1Config(void);
int8_t getPositionValues(uint16_t *target);

#define RN4020_MAX_UUID_LEN_BYTES (128 / 8)

//SUPPORTED FEATURES
#define RN4020_FEATURE_PERIFERAL           0x00000000
#define RN4020_FEATURE_CENTRAL             0x80000000
#define RN4020_FEATURE_REAL_TIME_READ      0x40000000
#define RN4020_FEATURE_AUTO_ADVERTISE      0x20000000
#define RN4020_FEATURE_ENABLE_MLDP         0x10000000
#define RN4020_FEATURE_AUTO_MLDP_DISABLE   0x08000000
#define RN4020_FEATURE_NO_DIRECT_ADVERTISE 0x04000000
#define RN4020_FEATURE_UART_FLOW_CONTROL   0x02000000
#define RN4020_FEATURE_RUN_SCRIPT_PWR_ON   0x01000000
#define RN4020_FEATURE_ENABLE_AUTH         0x00400000
#define RN4020_FEATURE_ENABLE_REMOTE_CMD   0x00200000
#define RN4020_FEATURE_DO_NOT_SAVE_BONDING 0x00100000
#define RN4020_FEATURE_IO_CAP              0x00E00000
#define RN4020_FEATURE_BLOCK_SET_CMD       0x00010000
#define RN4020_FEATURE_ENABLE_OTA          0x00008000
#define RN4020_FEATURE_IOS_MODE            0x00004000
#define RN4020_FEATURE_SERVER_ONLY         0x00002000
#define RN4020_FEATURE_ENABLE_UART_SCRIPT  0x00001000
#define RN4020_FEATURE_AUTO_MLDP           0x00000800
#define RN4020_FEATURE_MLDP_WITHOUT_STATUS 0x00000400

//SUPPORTED SERVICES
#define RN4020_SERVICE_DEVICE_INFORMATION    0x80000000
#define RN4020_SERVICE_BATTERY               0x40000000
#define RN4020_SERVICE_HEART_RATE            0x20000000
#define RN4020_SERVICE_HEALTH_THERMOMETER    0x10000000
#define RN4020_SERVICE_GLUCOSE               0x08000000
#define RN4020_SERVICE_BLOOD_PRESSURE        0x04000000
#define RN4020_SERVICE_RUNNING_SPEED_CADENCE 0x02000000
#define RN4020_SERVICE_CYCLING_SPEED_CADENCE 0x01000000
#define RN4020_SERVICE_CURRENT_TIME          0x00800000
#define RN4020_SERVICE_NEXT_DST_CHANGE       0x00400000
#define RN4020_SERVICE_REFERENCE_TIME_UPDATE 0x00200000
#define RN4020_SERVICE_LINK_LOSS             0x00100000
#define RN4020_SERVICE_IMMEDIATE_ALERT       0x00080000
#define RN4020_SERVICE_TX_POWER              0x00040000
#define RN4020_SERVICE_ALERT_NOTIFICATION    0x00020000
#define RN4020_SERVICE_PHONE_ALERT_STATUS    0x00010000
#define RN4020_SERVICE_SCAN_PARAMETERS       0x00004000
#define RN4020_SERVICE_USER_DEFINED          0x00000001

//CHARACTERISTIC PROPERTIES
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_EXTENDED               0b10000000
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_AUTH_WRITE             0b01000000
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_INDICATE               0b00100000
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_NOTIFY                 0b00010000
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_WRITE                  0b00001000
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_WRITE_WITHOUT_RESPONSE 0b00000100
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ                   0b00000010
#define RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_BROADCAST              0b00000001

//SECURITY FLAGS OF CHARACTERISTIC
#define RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE     0b00000000
#define RN4020_PRIVATE_CHARACTERISTIC_SECURITY_ENCR_R   0b00000001
#define RN4020_PRIVATE_CHARACTERISTIC_SECURITY_AUTH_R   0b00000010
#define RN4020_PRIVATE_CHARACTERISTIC_SECURITY_ENCR_W   0b00010000
#define RN4020_PRIVATE_CHARACTERISTIC_SECURITY_AUTH_W   0b00100000

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

//-------------- USART FUNCTIONS------------------
void readAllSerial();
charLineBuffer_t *tryReadCharLineUSART();

//-------------- SPI FUNCTIONS--------------------
void OLED_puts(const char *g, int ypos);

#endif
