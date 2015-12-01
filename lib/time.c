#include "time.h"

void init_time(struct time_t * time) {
  time->msec = 0;
  time->sec_0 = 0;
  time->sec_1 = 0;
  time->min_0 = 0;
  time->min_1 = 0;
}

void add_ms(struct time_t * time, int ms) {
  time->msec += ms;
  if (time->msec >= 1000) {
    time->msec -= 1000;
    time->sec_0 += 1;
  }
  if (time->sec_0 >= 10) {
    time->sec_0 -= 10;
    time->sec_1 += 1;
  }
  if (time->sec_1 >= 6) {
    time->sec_1 -= 6;
    time->min_0 += 1;
  }
  if (time->min_0 >= 10) {
    time->min_0 -= 10;
    time->min_1 += 1;
  }
  if (time->min_1 >= 10) {
    time->min_1 -= 10;
  }
}
