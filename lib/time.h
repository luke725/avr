#ifndef TIME_H
#define TIME_H

struct time_t {
  int msec;
  int sec_0, sec_1;
  int min_0, min_1;
};

void init_time(struct time_t * time);

void add_ms(struct time_t * time, int ms);


#endif
