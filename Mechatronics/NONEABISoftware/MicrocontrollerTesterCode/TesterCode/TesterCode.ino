#include "avr/io.h"
#include "avr/interrupt.h"

void configure_timer0();

void setup() {

  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  DDRB |= (1 << PB3);
  DDRB |= (1 << PB4);

  PORTB &= ~(1 << PB3);
  PORTB |= (1 << PB4);

  configure_timer0();
  sei();

}

void loop() {
  delay(500);

}

void configure_timer0() {

  //CTC OCR0A MODE
  TCCR1A &= ~(1 << WGM10);
  TCCR1A &= ~(1 << WGM11);
  TCCR1B |= (1 << WGM12);

  //CLOCK PREESCALER TO 8
  TCCR1B &= ~(1 << CS10);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS12);

  TCNT1 = 0;
  //Counter top = (T x Fcpu / N) - 1
  //Counter top = (.001"*16000000/8)-1
  OCR1A = 1999;//Traduced to 100 samples per second

  //COMPARE INTERRUPT ENABLE
  TIMSK1 |= (1 << OCIE1A);

}

ISR(TIMER1_COMPA_vect) {
  static int i = 0;
  if (i == 0) {
    PORTB ^= (1 << PB3);
    i = 1;

  } else if (i == 1) {
    PORTB ^= (1 << PB4);
    i= 0;
  }

}
