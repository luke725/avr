#ifndef LEDS_H
#define LEDS_H

void init_led();
void set_led(int n, char bits);
void set_bit(int n, char bit);

void display_next_led();

#endif
