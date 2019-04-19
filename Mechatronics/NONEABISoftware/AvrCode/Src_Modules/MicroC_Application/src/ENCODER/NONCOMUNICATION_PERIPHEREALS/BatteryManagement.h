#ifndef BATTERYMANAGEMENT_H
#define BATTERYMANAGEMENT_H

#define START_ANALOGTIMER TCCR0B |=		\
    (1 << CS02);				\
  TCCR0B &= ~(1 << CS01) |			\
    ~(1 << CS00)
#define STOP_ANALOGTIMER TCCR0B &=		\
    ~(1 << CS02) | ~(1 << CS01) |		\
    ~(1 << CS00)

#define SETPC0CHANNEL				\
  ADMUX &= ~(1 << MUX3);			\
  ADMUX &= ~(1 << MUX2);			\
  ADMUX &= ~(1 << MUX1);			\
  ADMUX &= ~(1 << MUX0)

#define SET1VCHANNEL				\
  ADMUX |= (1 << MUX3);				\
  ADMUX |= (1 << MUX2);				\
  ADMUX |= (1 << MUX1);				\
  ADMUX &= ~(1 << MUX0)

//TIME DEFINED FOR 128 PREESCALER AT 20MHZ
typedef enum{
  fiftyMillis = 16,
  twoSeconds = 625
}timeInTicks;

volatile timeInTicks currentWaitingCount;
extern volatile uint16_t analogcounter;
extern volatile uint16_t adcValue;

extern volatile uint16_t analogIncorrectVoltage;
extern volatile uint16_t supplyVoltage;
extern volatile uint16_t analogCorrectedVoltage;

extern uint8_t lastPercentage;
extern uint8_t currPercentage;

void initBatteryManagement();
void configureADC();
void timer0Config();
uint8_t analogBufferAvailable();
uint16_t getAnalogVoltage();
uint8_t batteryIsCharging();//Pulldown resistor logic
uint8_t getBatteryPercentage(uint16_t analogVoltage);

#endif

