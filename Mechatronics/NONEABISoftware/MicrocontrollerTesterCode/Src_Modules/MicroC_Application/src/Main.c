#include <avr/io.h>
#include <avr/interrupt.h>

void configure_timer0();

volatile int timer_repetitions = 0;

int main(){

  DDRB |= (1 << PB3);
  DDRB |= (1 << PB4);

  PORTB &= ~(1 << PB3);
  PORTB |= (1 << PB4);
  
  configure_timer0();
  sei();
  
  while(1){

  }
  
  return 0;
}

void configure_timer0(){

  //CTC OCR0A MODE
  TCCR0A &= ~(1 << WGM00);
  TCCR0A |= (1 << WGM01);
  TCCR0B &= ~(1 << WGM02);

  //CLOCK PREESCALER TO 8
  TCCR0B &= ~(1 << CS00);
  TCCR0B |= (1 << CS01);
  TCCR0B &= ~(1 << CS02);

  TCNT0 = 0;
  //Counter top = (T x Fcpu / N) - 1
  //Counter top = (.001"*1000000/8)-1
  OCR0A = 124;//Traduced to 100 samples per second

  //COMPARE INTERRUPT ENABLE
  TIMSK |= (1 << OCIE0A);

}

ISR(TIMER0_COMPA_vect){

  timer_repetitions++;
  if(timer_repetitions == 1){
    timer_repetitions = 0;
    PORTB ^= (1 << PB3);
    PORTB ^= (1 << PB4);
  }
  
}
