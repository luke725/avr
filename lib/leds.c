#include <avr/io.h>

#include "leds.h"

int counter = 0;

char leds[4];

void init_led() {
  int i; 
  for (i = 0; i < 4; i++) {
    leds[i] = 0xFF;
  }
}

void set_led(int n, char bits) {
  leds[n] &= 0x80;
  leds[n] |= bits;
}

void set_bit(int n, char bit) {
  if (bit) {
    leds[n] &= 0x7F;
  } else {
    leds[n] |= 0x80;
  }
}

void set_port_c(int i) {
  PORTC &= 0xF0;
  PORTC |= 0x0F & (0x0F - (1 << i));
}

void display_next_led() {
  counter = (counter + 1) & 0x3;
  set_port_c(counter);
  PORTD = leds[counter];
}
