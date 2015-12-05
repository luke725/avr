#ifndef BUTTONS_H
#define BUTTONS_H

struct button_state_t {
  char value;
  char changes;
};

void init_button_state(struct button_state_t * button_state);
char register_button(struct button_state_t * button_state, char current_value);
char button_value(struct button_state_t * button_state);

#endif
