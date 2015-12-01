#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/leds.h"
#include "lib/time.h"

char numbers [] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

struct time_t time;

ISR(TIMER0_OVF_vect) {
  TCNT0 += 6;
  add_ms(&time, 2);
  display_next_led();
}

void init_timer() {
  TIMSK = (1<<TOIE0);
  TCCR0 |= (1 << CS01);
  TCNT0 = 0x00;
  sei();
}

void display_time(struct time_t * time) {
    set_led(3, numbers[time->sec_0]);
    set_led(2, numbers[time->sec_1]);
    set_led(1, numbers[time->min_0]);
    set_led(0, numbers[time->min_1]);
	set_bit(1, time->msec >= 500);
}

int main() {
  init_time(&time);
  DDRC |= 0x0F;
  DDRD = 0xFF;
  init_led();
  init_timer();
  while (1) {
    display_time(&time);
    _delay_ms(100);
  }
}
