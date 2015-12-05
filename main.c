#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/leds.h"
#include "lib/time.h"

char numbers[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
char blinks[4];

char button = 0;
int button_change = 0;

struct button_state_t {
  char value;
  char changes;
};

char register_button(struct button_state_t * button_state, char current_value) {
  if (current_value) current_value = 1;
  
  if (current_value == button_state->value) {
    button_state->changes = 0;
    return 0;
  } else {
    if (button_state->changes == 2) {
      button_state->changes = 0;
      button_state->value = current_value;
      return 1;
    } else {
      button_state->changes++;
      return 0;
    }
  }
}

void init_button_state(struct button_state_t * button_state) {
  button_state->value = 0;
  button_state->changes = 0;
}

struct time_t time;
struct button_state_t button_state1;

ISR(TIMER0_OVF_vect) {
  TCNT0 += 6;
  add_ms(&time, 2);
  char button_change = register_button(&button_state1, bit_is_clear(PINC, 4));
  if (button_change && button_state1.value) {
    blinks[0] = !blinks[0];
  }
  display_next_led();
}

void init_timer() {
  TIMSK = (1<<TOIE0);
  TCCR0 |= (1 << CS01);
  TCNT0 = 0x00;
  sei();
}

void set_led_number(int led, int number, char blink, struct time_t * time) {
  if (blink && (time->msec >= 750 || (time->msec >= 250 && time->msec < 500))) {
    set_led(led, 0xFF);
  } else {
    set_led(led, numbers[number]);
  }
}

void display_time(struct time_t * time) {
  set_led_number(3, time->sec_0, blinks[3], time);
  set_led_number(2, time->sec_1, blinks[2], time);
  set_led_number(1, time->min_1, blinks[1], time);
  set_led_number(0, time->min_0, blinks[0], time);
  set_bit(1, time->msec >= 500);
}

void init_time_display() {
  int i = 0;
  for (i = 0; i < 4; i++) {
    blinks[i] = 0;
  }
}

int main() {
  init_time(&time);
  DDRC &= 0xC0;
  DDRC |= 0x0F;
  PORTC &= 0xC0;
  PORTC |= 0x3F;
  DDRD = 0xFF;
  PORTD = 0xFF;
  init_led();
  init_timer();
  init_button_state(&button_state1);
  while (1) {
    display_time(&time);
    _delay_ms(100);
  }
}
