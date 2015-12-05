#include "buttons.h"

void init_button_state(struct button_state_t * button_state) {
  button_state->value = 0;
  button_state->changes = 0;
}

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

char button_value(struct button_state_t * button_state) {
  return button_state->value;
}

