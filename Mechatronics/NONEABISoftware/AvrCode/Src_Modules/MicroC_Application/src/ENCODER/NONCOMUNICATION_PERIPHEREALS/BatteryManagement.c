#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "BatteryManagement.h"
#include "communication.h"
#include "ring.h"

#define ANALOG_DATA_BUFFER_LEN 2
ring_t analog_data_ring;
uint16_t analog_data_buffer[ANALOG_DATA_BUFFER_LEN];

#if ((ANALOG_DATA_BUFFER_LEN - 1) & ANALOG_DATA_BUFFER_LEN) == 0
# if ANALOG_DATA_BUFFER_LEN > 256
# warning ANALOG_DATA_BUFFER_LEN> 256
# endif
#else
	#warning ANALOG_DATA_BUFFER_LEN NOT POWER OF 2
#endif

volatile uint16_t analogcounter = 0;
volatile uint16_t adcValue = 0;

volatile uint16_t analogIncorrectVoltage = 0;
volatile uint16_t supplyVoltage = 0;
volatile uint16_t analogCorrectedVoltage = 0;

uint8_t lastPercentage = 0;
uint8_t currPercentage = 0;

void initBatteryManagement(){
  configureADC();
  SETPC0CHANNEL;
  currentWaitingCount = fiftyMillis;
  ring_buffer_init(&analog_data_ring, &analog_data_buffer, sizeof(analog_data_buffer[0]), sizeof(analog_data_buffer));
  timer0Config();

  //Charging detection
  DDRB &= ~(1 << PB1);
  
}

void configureADC(){

  //AVcc REFERENCE
  ADMUX &= ~(1 << REFS1);
  ADMUX |= (1 << REFS0);
  
  //PRESCALER VALUE OF 128 FOR 20MHZ
  ADCSRA |= (1 << ADPS2);
  ADCSRA |= (1 << ADPS1);
  ADCSRA |= (1 << ADPS0);
	
  //ADC COMPLETE INTERRUPT ENABLE
  ADCSRA |= (1 << ADIE);
	
  //ENABLE ADC
  ADCSRA |= (1<<ADEN);
}

void timer0Config(){
  
  //CTC OCR1A MODE
  TCCR0A |= (1 << WGM01);

  //CLOCK PREESCALER TO 256
  START_ANALOGTIMER;

  //INITIAL AND COMPARE VALUE
  TCNT0 = 0;
  
  //Counter top = (T x Fcpu / N) - 1
  //Counter top = (.0032"*20000000/256)-1
  OCR0A = 249;

  //COMPARE INTERRUPT ENABLE
  TIMSK0 |= (1 << OCIE1A);
}

uint8_t analogBufferAvailable(){
  return (!ring_buffer_empty(&analog_data_ring))? 1:0;
}

uint16_t getAnalogVoltage(){
  uint16_t voltage = 0;
  ring_buffer_get(&analog_data_ring, &voltage);

  return voltage;
}

uint8_t batteryIsCharging(){
  if(PINB & (1 << PB1)){
    return 1;
  }else{
    return 0;
  }
}

uint8_t getBatteryPercentage(uint16_t analogVoltage){

  static uint8_t firstFunctionCall = 1;
  uint8_t calculatedPercentage;
  uint8_t returnedValue;
  
  //Written as if statements to allow future changes
  //dependent of each battery
  if(analogVoltage <= 3000){
    calculatedPercentage = 0;
  }else if(analogVoltage <= 3150){
    calculatedPercentage = 10;
  }else if(analogVoltage <= 3300){
    calculatedPercentage = 20;
  }else if(analogVoltage <= 3400){
    calculatedPercentage = 30;
  }else if(analogVoltage <= 3500){
    calculatedPercentage = 40;
  }else if(analogVoltage <= 3600){
    calculatedPercentage = 50;
  }else if(analogVoltage <= 3700){
    calculatedPercentage = 60;
  }else if(analogVoltage <= 3800){
    calculatedPercentage = 70;
  }else if(analogVoltage <= 3900){
    calculatedPercentage = 80;
  }else if(analogVoltage <= 4000){
    calculatedPercentage = 90;
  }else{
    calculatedPercentage = 100;
  }

  if(firstFunctionCall == 1){
    lastPercentage = calculatedPercentage;
    firstFunctionCall = 0;
  }

  if(batteryIsCharging()){
    if(calculatedPercentage <= lastPercentage){
      returnedValue = lastPercentage;
    }else{
      lastPercentage = calculatedPercentage;
      returnedValue = calculatedPercentage;
    }
  }else{
    if(calculatedPercentage >= lastPercentage){
      returnedValue = lastPercentage;
    }else{
      lastPercentage = calculatedPercentage;
      returnedValue = calculatedPercentage;
    }
  }

  return returnedValue;
}

ISR(TIMER0_COMPA_vect){
  analogcounter++;
  //1.0016 seconds
  if(analogcounter == currentWaitingCount){
    analogcounter=0;
    //INIT ADC CONVERTION
    ADCSRA |= (1 << ADSC);
  }		
}

ISR(ADC_vect){

  //Prevent other interrupts occurring 'cause timer
  //initializing ADC convertion
  STOP_ANALOGTIMER;
  
  static uint8_t correctVoltageMode = 0;

  //Determine pertinent voltage determination mode
  if(!correctVoltageMode){
    
    //Determine Voltage
    analogIncorrectVoltage = 5 * ADC/1023 * 1000UL;
    
    //Set voltage correction conversion to occur as soon as possible 
    currentWaitingCount = fiftyMillis;
    //Set next voltage determination to correctVoltageMode
    correctVoltageMode = 1;
    //Set next voltage determination channel
    SET1VCHANNEL;
    
  }else{

    uint16_t finalVoltage;
    //Determine Voltage
    supplyVoltage = 1126400L/ADC;// Calculate Vcc 1126400 = 1.1*1024*1000
    analogCorrectedVoltage =
      (supplyVoltage*1000UL/5000UL *analogIncorrectVoltage)/1000;
    
    //Insert Results    
    finalVoltage = analogCorrectedVoltage;
    ring_buffer_put(&analog_data_ring, &finalVoltage);
    
    //Once voltage is corrected set adc conversions to happen each 4s 
    currentWaitingCount = twoSeconds;    
    //Set next voltage determination to no correctVoltageMode
    correctVoltageMode = 0;    
    //Set next voltage determination channel
    SETPC0CHANNEL;
      
  }

  //RESTART ANALOG TIMER
  START_ANALOGTIMER;
}

// 5523096070 
