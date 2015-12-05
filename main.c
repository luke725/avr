#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/leds.h"
#include "lib/time.h"
#include "lib/buttons.h"

char numbers[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

struct set_time_state_t {
  char position;
  char on;
};

struct set_time_state_t set_time_state;

void init_set_time_state(struct set_time_state_t * set_time_state) {
  set_time_state->on = 0;
  set_time_state->position = 0;
}

void next_set_time_state(struct set_time_state_t * set_time_state) {
  if (set_time_state->on) {
    if (set_time_state->position == 3) {
      set_time_state->on = 0;
    } else {
      set_time_state->position++;
    }
  } else {
    set_time_state->on = 1;
    set_time_state->position = 0;
  }
}

void change_time(struct set_time_state_t * set_time_state, struct time_t * time) {
  if (!set_time_state->on) {
    return;
  }
  switch (set_time_state->position) {
    case 0:
      time->min_1++;
      if (time->min_1 == 6) time->min_1 = 0;
      break;
    case 1:
      time->min_0++;
      if (time->min_0 == 10) time->min_0 = 0;
      break;
    case 2:
      time->sec_1++;
      if (time->sec_1 == 6) time->sec_1 = 0;
      break;
    case 3:
      time->sec_0++;
      if (time->sec_0 == 10) time->sec_0 = 0;
      break;
  }
}

struct time_t time;
struct button_state_t button_state1;
struct button_state_t button_state2;

ISR(TIMER0_OVF_vect) {
  TCNT0 += 6;
  add_ms(&time, 2);
  if (register_button(&button_state1, bit_is_clear(PINC, 4)) && button_value(&button_state1)) {
    next_set_time_state(&set_time_state);
  }
  if (register_button(&button_state2, bit_is_clear(PINC, 5)) && button_value(&button_state2)) {
    change_time(&set_time_state, &time);
  }
  display_next_led();
}

void init_timer() {
  TIMSK = (1<<TOIE0);
  TCCR0 |= (1 << CS01);
  TCNT0 = 0x00;
  sei();
}

char should_blink(struct set_time_state_t * set_time_state, int led) {
  return (set_time_state->on && set_time_state->position == led);
}

void set_led_number(struct set_time_state_t * set_time_state, struct time_t * time, int led, int number) {
  if (should_blink(set_time_state, led) && (time->msec >= 750 || (time->msec >= 250 && time->msec < 500))) {
    set_led(led, 0xFF);
  } else {
    set_led(led, numbers[number]);
  }
}

void display_time(struct set_time_state_t * set_time_state, struct time_t * time) {
  set_led_number(set_time_state, time, 3, time->sec_0);
  set_led_number(set_time_state, time, 2, time->sec_1);
  set_led_number(set_time_state, time, 1, time->min_0);
  set_led_number(set_time_state, time, 0, time->min_1);
  set_bit(1, time->msec >= 500);
}

int main() {
  init_time(&time);
  init_set_time_state(&set_time_state);
  DDRC &= 0xC0;
  DDRC |= 0x0F;
  PORTC &= 0xC0;
  PORTC |= 0x3F;
  DDRD = 0xFF;
  PORTD = 0xFF;
  init_led();
  init_timer();
  init_button_state(&button_state1);
  init_button_state(&button_state2);
  while (1) {
    display_time(&set_time_state, &time);
    _delay_ms(100);
  }
}
