#include "hola.h"

void go() {
  initEncoder();//Should go first
  initCommunication();//enables interruptions automatically

  while (1) {

    readAllSerial();

    //After CMD is read and handles found
    if (rn4020.allUUIDHandlesMatchesFound) {
      uint16_t detectedPosition = 0;
      int recolectedEncoderValues = 0;
      uint16_t valuesBuff[10];

      //initiate Loop
      while (1) {

        readAllSerial();

        //Writting a biger than characteristic size
        //value will make rn4020 urresponsive so take care
        //with disconnecting encoder related wires which make
        //microcontroller read big Values

        if (getPositionValues(&detectedPosition) != -1) {
          valuesBuff[recolectedEncoderValues] = detectedPosition;
          recolectedEncoderValues++;
          if (recolectedEncoderValues == 10) {
              printf("SHW,001B,"
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
                    valuesBuff[0],
                    valuesBuff[1],
                    valuesBuff[2],
                    valuesBuff[3],
                    valuesBuff[4],
                    valuesBuff[5],
                    valuesBuff[6],
                    valuesBuff[7],
                    valuesBuff[8],
                    valuesBuff[9]
                   );
            
            /*
            writeTenTwoBytesCharacteristic(ENCODER_VALUE,
                                           valuesBuff[0],
                                           valuesBuff[1],
                                           valuesBuff[2],
                                           valuesBuff[3],
                                           valuesBuff[4],
                                           valuesBuff[5],
                                           valuesBuff[6],
                                           valuesBuff[7],
                                           valuesBuff[8],
                                           valuesBuff[9]);
                                           */
            recolectedEncoderValues = 0;          
          }
        }
      }
    }
  }
}


int ring_buffer_init(ring_t *ring, void *buf, size_t buf_elements_size, size_t arrayByteSize) {

  //Check passed pointers against null
  if ((ring != NULL) && (buf != NULL)) {
    //Check passed values
    if ((buf_elements_size > 0) && (arrayByteSize > 0)) {

      uint16_t n_elements = arrayByteSize / buf_elements_size;

      //Check nelements is a power of 2
      if (((n_elements - 1) & n_elements) == 0) {
        ring->s_elem = buf_elements_size;
        ring->n_elem = n_elements;
        ring->buf = buf;
        ring->head = 0;
        ring->tail = 0;
        return 0;
      }
    }
  }

  return -1;
}

int ring_buffer_full(ring_t *ring) {

  return ((ring->head - ring->tail) == ring->n_elem) ? 1 : 0;

}


int ring_buffer_empty(ring_t *ring) {
  return ((ring->head - ring->tail) == 0) ? 1 : 0;
}

inline void memcopy(void *destref, const void *srcref, size_t count) {
  uint8_t *dest = (uint8_t*)destref;
  uint8_t *src = (uint8_t*)srcref;

  while (count--) {
    *dest++ = *src++;
  }
}

int ring_buffer_put(ring_t *ring, const void *data) {
  int status = -1;

  //Chek if can insert
  if (!ring_buffer_full(ring)) {
    //Determine insertion position
    const size_t position = (ring->head & (ring->n_elem - 1)) * ring->s_elem;
    //insert data at right position
    memcopy((uint8_t*)ring->buf + position, data, ring->s_elem);
    ring->head++;

    status = 0;
  }
  return status;
}


int ring_buffer_get(ring_t *ring, void *data) {
  int status = -1;

  //Chek if can insert
  if (!ring_buffer_empty(ring)) {
    //Determine insertion position
    const size_t position = (ring->tail & (ring->n_elem - 1)) * ring->s_elem;
    //insert data at right position
    memcopy(data, (uint8_t *)ring->buf + position, ring->s_elem);
    ring->tail++;

    status = 0;

  }
  return status;
}

FILE uart_io = FDEV_SETUP_STREAM_U(put_char, NULL, _FDEV_SETUP_WRITE, (void*)0);

#define USART0_SEND_BUFFER_LEN 256
#define USART0_RECEIVE_BUFFER_LEN   256

ring_t usart_send_ring;
ring_t usart_receive_ring;

char usart_send_buffer[USART0_SEND_BUFFER_LEN];
char usart_receive_buffer[USART0_RECEIVE_BUFFER_LEN];

#if ((USART0_SEND_BUFFER_LEN - 1) & USART0_SEND_BUFFER_LEN) == 0
# if USART0_SEND_BUFFER_LEN > 256
# warning USART0_SEND_BUFFER > 256
# endif
#else
#warning USART0_SEND_BUFFER NOT POWER OF 2
#endif

#if ((USART0_RECEIVE_BUFFER_LEN- 1) & USART0_RECEIVE_BUFFER_LEN) == 0
# if USART0_RECEIVE_BUFFER_LEN > 256
# warning USART0_RECEIVE_BUFFER > 256
# endif
#else
#warning USART0_RECEIVE_BUFFER NOT POWER OF 2
#endif

void uart_init(uint16_t ubrr_value) {

  UBRR0L  = (uint8_t) ubrr_value;
  UBRR0H  = (ubrr_value >> 8);

#ifdef USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif

  ring_buffer_init(&usart_receive_ring, usart_receive_buffer, sizeof(usart_send_buffer[0]), sizeof(usart_send_buffer));

  ring_buffer_init(&usart_send_ring, usart_send_buffer, sizeof(usart_send_buffer[0]), sizeof(usart_send_buffer));

  //Atach uart stream to stdout
  stdout = &uart_io;

  UCSR0B |= (1 << TXEN0) | (1 << RXEN0); /* Enable USART transmitter/receiver */

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */

  UCSR0B |= (1 << RXCIE0); //Enable USART receive interrupt
}

void printString(const char myString[]) {
  uint8_t i = 0;
  while (myString[i]) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = myString[i];
    i++;
  }
}

int put_char(char data, FILE * stream) {

  //loop_until_bit_is_set(UCSR0A, UDRE0);/* Wait for empty transmit buffer */
  //UDR0 = data;
  while (ring_buffer_full(&usart_send_ring)); //wait till there's space
  ring_buffer_put(&usart_send_ring, &data);
  UCSR0B |= (1 <<  UDRIE0);
  return 0;
}

int serialAvailable(void) {
  return  (!ring_buffer_empty(&usart_receive_ring)) ? 1 : 0;
}

char get_char(void) {
  char c = 0;

  ring_buffer_get(&usart_receive_ring, &c);

  return c;
}

ISR(USART_RX_vect) {

  const char c = UDR0;

  ring_buffer_put(&usart_receive_ring, &c);

}

ISR(USART_UDRE_vect) {

  if (ring_buffer_empty(&usart_send_ring))
    UCSR0B &= ~(1 <<  UDRIE0);

  char c;
  if (ring_buffer_get(&usart_send_ring, &c) != -1) {
    UDR0 = c;
  }
}

//encoder Variables
volatile uint16_t encoderCounter = 32768;

#define ENCODER_BUFFER_LEN 16
ring_t encoder_ring;
uint16_t encoder_buffer[ENCODER_BUFFER_LEN];

#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
# if ENCODER_BUFFER_LEN > 256
# warning ENCODER_BUFFER_LEN> 256
# endif
#else
#warning ENCODER_BUFFER_LEN NOT POWER OF 2
#endif

void initEncoder(void) {
  ext_interruptConfig();
  timer1Config();
  ring_buffer_init(&encoder_ring, encoder_buffer, sizeof(encoder_buffer[0]),
                   sizeof(encoder_buffer));

  DDRD &= ~(1 << PD2);//INT0 as input
  DDRD &= ~(1 << PD3);
}

void ext_interruptConfig(void) {

  //CHANGE MODE
  EICRA |= (1 << ISC00);

  //INT0 INTERRUPT ENABLE
  EIMSK |= (1 << INT0);

}

void timer1Config(void) {

  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  //CTC OCR1A MODE
  TCCR1B |= (1 << WGM12);

  //CLOCK PREESCALER TO 8
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS10);

  //INITIAL AND COMPARE VALUE
  TCNT1 = 0;
  //Counter top = (T x Fcpu / N) - 1
  //Counter top = (.005"*16000000/8)-1
  OCR1A = 9999;//Traduced to 200 samples per second

  //COMPARE INTERRUPT ENABLE
  TIMSK1 |= (1 << OCIE1A);
}

int8_t getPositionValues(uint16_t *integer) {

  return ring_buffer_get(&encoder_ring, integer);
}

ISR(INT0_vect) {

  //2X ENCODING
  if (PIND & (1 << PD2)) { //RISING STATE OF INT0

    if ( (PIND & (1 << PD3)) == 0) { //if pulse states are different
      encoderCounter++;
    } else {            //if pulse states are the same
      encoderCounter--;
    }

  } else {     //FALLING STATE OF INT0

    if (PIND & (1 << PD3)) {       //if pulse states are different
      encoderCounter++;
    } else {            //if pulse states are the same
      encoderCounter--;
    }

  }
}



ISR(TIMER1_COMPA_vect) {  
  cli();
  uint16_t detectedPosition = encoderCounter;
sei();

  ring_buffer_put(&encoder_ring, &detectedPosition);

}

//PRIVATE SERVICE CHARACTERISTIC UUID
const char private_service_UUID[] PROGMEM = "11223344556677889900AABBCCDDEEFF";

PGM_P const services_table[SERVICES_NUMBER] PROGMEM = {
  private_service_UUID
};

//128 AND 16 BIT USED CHARACTERISTICS UUIDS
const char characteristic_UUID_0[] PROGMEM = "010203040506070809000A0B0C0D1FFF";//HIGHEST VALUE FOR CORRUPTION REMOVAL NOT NOTIFYABLE OR INDICABLE
const char characteristic_UUID_1[] PROGMEM = "010203040506070809000A0B0C0D0E0F";
const char characteristic_UUID_2[] PROGMEM = "111213141516171819101A1B1C1D1E1F";
const char characteristic_UUID_3[] PROGMEM = "2A19";

PGM_P const characteristics_table[CHARACTERISTICS_NUMBER] PROGMEM = {
  characteristic_UUID_0,
  characteristic_UUID_1,
  characteristic_UUID_2,
  characteristic_UUID_3
};

void initCommunication() {
  uart_autoinit();
  sei();
  printString("InitializingBluetooth\n");
  DDRD |= (1 << PD4);
  PORTD |= (1 << PD4);
}

void bluetoothConfig(int configuration) {
  if (configuration == 1) {

    char stringsBuffer[33];

    setFactoryReset(1);//Should go first
    cleanPrivateService();//Should go second to services to show
    setFeatures(RN4020_FEATURE_PERIFERAL | RN4020_FEATURE_DO_NOT_SAVE_BONDING
                | RN4020_FEATURE_SERVER_ONLY); //SERVER ONLY REDUCES OVERHEAD
    setSupportedServices(RN4020_SERVICE_BATTERY | RN4020_SERVICE_USER_DEFINED );

    //SET PRIVATE SERVICES (JUST ONE IN RN4020)
    //Set Private Services (Just one in RN4020)
    for (int i = 0; i < SERVICES_NUMBER; i++) {
      strcpy_P(stringsBuffer, (PGM_P)pgm_read_word(&(services_table[i])));
      setPrivateService(stringsBuffer);
    }

    //CONFIGURE CHARACTERISTICS

    //CONFIGURE CORRUPTION
    //THIS SHOULD BE NO NOTIFY TO GET AWAY FROM CORRUPTION
    strcpy_P(stringsBuffer,
             (PGM_P)pgm_read_word(&(characteristics_table[CORRUPTION])));
    setPrivateCharacteristic(stringsBuffer,
                             RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ,
                             20,
                             RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                            );

    //CONFIGURE ENCODER
    strcpy_P(stringsBuffer,
             (PGM_P)pgm_read_word(&(characteristics_table[ENCODER_VALUE])));
    setPrivateCharacteristic(stringsBuffer,
                             RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_NOTIFY,
                             20,
                             RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                            );


    //CONFIGURE BATTERYCHARGING
    strcpy_P(stringsBuffer,
             (PGM_P)pgm_read_word(&(characteristics_table[BATTERYCHARGING_VALUE])));
    setPrivateCharacteristic(stringsBuffer,
                             RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ,
                             20,
                             RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                            );

    setName("VBT");//No more than 6 bytes when using private service


  } else if (configuration == 2) {
    setFactoryReset(2);
  }
}

void runLockingCOMMAND(uint8_t* notifyChecking, const char * format, ...) {

  int notifyCheckingIsNull = 1;
  uint8_t *notifyActive = notifyChecking;
  int previousNotifyActive;

  if (notifyChecking != NULL) {
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
  readAllSerial();

  //Send Command
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  //Send Command

  int waitingMax = 2;
  int errorDetection = 0;
  //waiting loop
  for (int i = 0; i < waitingMax; i ++) {
    int waitingState = 1;

    while (waitingState) {
      if (serialAvailable()) {
        charLineBuffer_t *buffer = tryReadCharLineUSART();
        genericLineParsing(buffer);
        unlockWaitingLineParsing(buffer, &waitingState, &errorDetection);

      }
      //second wait 1->0 lock detection section
      if ( (i == 1) && (!(*notifyActive))) {
        waitingState = 0;
      }

    }

    if (notifyCheckingIsNull) {
      i = waitingMax;
    } else if (!(previousNotifyActive)) {
      i = waitingMax;
    } else if (errorDetection) {
      i = waitingMax;
    }

  }

}

void advertise() {
  runLockingCOMMAND(NULL, "A\n");
}

void unBond() {
  runLockingCOMMAND(NULL, "U\n");
}

void sendLS() {
  privateHandles.length = 0;
  runLockingCOMMAND(NULL, "LS\n");
}

void cleanPrivateService() {
  runLockingCOMMAND(NULL, "PZ\n");
}

void setFactoryReset(uint8_t arg) {
  runLockingCOMMAND(NULL, "SF,%d\n", arg);
}

void setFeatures(uint32_t arg) {
  runLockingCOMMAND(NULL, "SR,%08lX\n", arg);
}

void setSupportedServices(uint32_t arg) {
  runLockingCOMMAND(NULL, "SS,%08lX\n", arg);
}

void setPrivateService(char *arg) {
  runLockingCOMMAND(NULL, "PS,%s\n", arg);
}

void setPrivateCharacteristic(char *service,
                              uint8_t properties,
                              uint8_t dataSize,
                              uint8_t securityFlag) {
  if (securityFlag != 0) {

    runLockingCOMMAND(NULL, "PC,%s,%02X,%02X,%02X\n",
                      service,
                      properties,
                      dataSize,
                      securityFlag);

  } else {

    runLockingCOMMAND(NULL, "PC,%s,%02X,%02X\n",
                      service,
                      properties,
                      dataSize);
  }
}

void setName( char *string) {
  runLockingCOMMAND(NULL, "SN,%s\n", string);
}

void writeOneOneByteCharacteristic(unsigned int handle, uint8_t value0) {

  runLockingCOMMAND(&privateHandles.
                    privateHandleNotifyActive[handle]
                    , "SHW,%04X,"
                    "%04X\n",
                    privateHandles.privateHandle[handle],
                    value0);
}

void writeTenTwoBytesCharacteristic(unsigned int handle, uint16_t value0,
                                    uint16_t value1, uint16_t value2,
                                    uint16_t value3, uint16_t value4,
                                    uint16_t value5, uint16_t value6,
                                    uint16_t value7, uint16_t value8,
                                    uint16_t value9) {

  runLockingCOMMAND(&privateHandles.privateHandleNotifyActive[handle],
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
                    privateHandles.privateHandle[handle],
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

void turnOffSubscription(int handle) {
  uint16_t subscriptionHandle =
    privateHandles.privateHandle[handle] + 1;
  runLockingCOMMAND(NULL
                    , "SHW,%04X,0000\n",
                    subscriptionHandle);
}

int RN4020_parseHandleWCLine(const char* line) {

  if (strncmp(line, "WC,", 3) != 0) {
    return 0;
  }

  //Convert handle to String
  char handleStr[5];
  strncpy(handleStr, line + 3, 4);
  handleStr[4] = '\0';
  uint16_t handle = strtol(handleStr, NULL, 16);

  //Compare received handle to our handles to make
  //sure received handle is a valid handle
  for (int i = 0; i < privateHandles.length; i++) {
    if ((privateHandles.privateHandle[i] + 1)
        == handle) {
      const char* dataStringPtr = line + 8;

      if ((strncmp(dataStringPtr, "0100", 4) == 0) ||
          (strncmp(dataStringPtr, "0200", 4) == 0)
         ) {
        privateHandles.privateHandleNotifyActive[i] = 1;
      } else {
        privateHandles.privateHandleNotifyActive[i] = 0;

      }
    }
  }

  return 1;
}

void UUIDHandleMatchFound(uint8_t UUIDHandlematchFound, uint16_t handle) {

  //If handle is not set, set match
  if ( (rn4020.UUIDHandlematchesFound & (1 << UUIDHandlematchFound)) == 0 ) {
    rn4020.UUIDHandlematchesFound |= (1 << UUIDHandlematchFound);
    privateHandles.privateHandle[UUIDHandlematchFound] = handle;
    privateHandles.length++;
  } else {
    //If handles is set, notification or indications handles is found
    //indicate subscription is available for this characteristic
    privateHandles.subscriptionEnabled |= (1 << UUIDHandlematchFound);
  }

  //Check allMatchesFound stated
  for (int i = 0; i < CHARACTERISTICS_NUMBER; i++) {
    if (rn4020.UUIDHandlematchesFound & (1 << i)) {
      rn4020.allUUIDHandlesMatchesFound = 1;
    } else {
      rn4020.allUUIDHandlesMatchesFound = 0;
      break;
    }
  }

}

int RN4020_parseHandleUUIDLine(const char* line) {

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

  //PARSE CHARACTERISTICS UUID MATCHES
  char buffer[33];
  for (int i = 0; i < CHARACTERISTICS_NUMBER; i++) {
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(characteristics_table[i])));
    if (strncmp(buffer, UUIDline, strlen(buffer)) == 0) {
      UUIDHandleMatchFound(i, handle);
    }
  }

  return 1;
}

void unlockWaitingLineParsing(charLineBuffer_t *clb,
                              int* waitingState, int *errorDetection) {
  char *buffer = clb->buf;
  int *terminator = &clb->terminatorcharposition;
  //Change to standard null terminator
  buffer[*terminator] = '\0';

  char* stdLine = buffer;

  //INTERPRET
  if ((strstr(stdLine, "AOK") != NULL) ||
      (strstr(stdLine, "END") != NULL)
     ) {
    *waitingState = 0;

  } else if (strstr(stdLine, "ERR") != NULL) {
    //If an ERR is received no second aok will come therefore we skip
    //otherwise communication is gonna lock waiting for no coming aok
    *errorDetection = 1;
    *waitingState = 0;
  }

  //Return to line as null terminator
  buffer[*terminator] = '\n';
}

void genericLineParsing(charLineBuffer_t *clb) {
  char *buffer = clb->buf;
  int *terminator = &clb->terminatorcharposition;
  //Change to standard null terminator
  buffer[*terminator] = '\0';

  char* stdLine = buffer;

  //INTERPRET
  if (strstr(stdLine, "Connection End") != NULL) {
    OLED_puts("Connection End", 3);
    for (int i = 0; i < privateHandles.length; i++) {
      privateHandles.privateHandleNotifyActive[i] = 0;

      if (privateHandles.subscriptionEnabled & (1 << i)) {
        turnOffSubscription(i);
      }
    }

    unBond();
    advertise();

  } else if (strstr(stdLine, "Connected") != NULL) {
    OLED_puts("Connected", 3);
  } else if (strstr(stdLine, "Bonded") != NULL) {
    OLED_puts("Bonded", 3);

  } else if (RN4020_parseHandleUUIDLine(stdLine)) {

  } else if (RN4020_parseHandleWCLine(stdLine)) {

  } else if (strstr(stdLine, "CMD") != NULL) {
    bluetoothConfig(1);
    sendLS();
    OLED_puts("SDT ENCODER VBT", 1);
    advertise();

  }


  //Return to line as null terminator
  buffer[*terminator] = '\n';
}

void readAllSerial() {
  while (serialAvailable()) {
    genericLineParsing(tryReadCharLineUSART());
  }
}

charLineBuffer_t *tryReadCharLineUSART() {
  int i = 0;
  char c;
  char *buffer = charLineBuffer.buf;
  while ((c = get_char()) != '\n') {
    //if characters were found
    if (c != 0) {
      //Add characters to buffer;
      buffer[i] = c;
      i++;
    }
  }

  buffer[i] = c;//Write string termination chacter (\n)
  charLineBuffer.terminatorcharposition = i;

  return &charLineBuffer;
}

void OLED_puts(const char *g, int ypos) {
  int i = 0;
  char text[22];
  int blankchars;

  while (*(g + i)) {
    if (i > 20) break;
    text[i] = *(g + i);
    i++;
  }

  blankchars = 21 - i;
  while (blankchars) {
    text[i] = ' ';
    blankchars--;
    i++;
  }
  text[i] = '\0';
}
