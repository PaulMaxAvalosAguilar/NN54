#include "Encoder.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ring.h"

//encoder Variables
volatile uint16_t encoderCounter = 32768;

#define ENCODER_BUFFER_LEN 32
ring_t encoder_ring;
uint16_t encoder_buffer[ENCODER_BUFFER_LEN];

#if ((ENCODER_BUFFER_LEN - 1) & ENCODER_BUFFER_LEN) == 0
# if ENCODER_BUFFER_LEN > 256
# warning ENCODER_BUFFER_LEN> 256
# endif
#else
	#warning ENCODER_BUFFER_LEN NOT POWER OF 2
#endif

void initEncoder(void){
  ext_interruptConfig();
  timer1Config();
  ring_buffer_init(&encoder_ring, encoder_buffer, sizeof(encoder_buffer[0]),
		   sizeof(encoder_buffer));
  
  DDRD &= ~(1 << PD2);//INT0 as input
  DDRD &= ~(1 << PD3);
}

void ext_interruptConfig(void){
	
  //CHANGE MODE
  EICRA |= (1 << ISC00);
	
  //INT0 INTERRUPT ENABLE
  EIMSK |= (1 << INT0);

}

void timer1Config(void){

  //CTC OCR1A MODE
  TCCR1B |= (1 << WGM12);
	
  //CLOCK PREESCALER TO 8
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS10);
	
  //INITIAL AND COMPARE VALUE
  TCNT1 = 0;
  //Counter top = (T x Fcpu / N) - 1
  //Counter top = (.005"*20000000/8)-1
  OCR1A = 12499;//Traduced to 200 samples per second
	
  //COMPARE INTERRUPT ENABLE
  TIMSK1 |= (1 << OCIE1A);
}

int8_t getPositionValues(uint16_t *integer){

 return ring_buffer_get(&encoder_ring, integer);
}

ISR(INT0_vect){
	
  //2X ENCODING	
  if(PIND & (1 << PD2)){  //RISING STATE OF INT0
	
    if( (PIND & (1 << PD3)) == 0){ //if pulse states are different
      encoderCounter++;
    }else{			       //if pulse states are the same
      encoderCounter--;
    }
	
  }else{			//FALLING STATE OF INT0
	
    if(PIND & (1 << PD3)){	       //if pulse states are different
      encoderCounter++;
    }else{			       //if pulse states are the same
      encoderCounter--;
    }
	
  }	
}

ISR(TIMER1_COMPA_vect){

  uint16_t detectedPosition = encoderCounter;
  
  ring_buffer_put(&encoder_ring, &detectedPosition);
  
}


