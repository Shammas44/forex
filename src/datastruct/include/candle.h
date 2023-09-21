#ifndef CANDLE_H
#define CANDLE_H
#include "tick.h"
#include "date.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef enum Candle_status {
  CANDLE_FILLED,
  CANDLE_STOP,
} Candle_status;

typedef struct Candle {
  Candle_status status;
  float open;
  float close;
  float high;
  float low;
  float volume;
  time_t timestamp;
} Candle;

typedef enum Candle_state {
  CANDLE_IN_PROGRESS,
  CANDLE_COMPLETED,
  CURRENT_TICK_SKIPPED
} Candle_state;

void candle_print(Candle *candle);

Candle_state candle_create(Candle *candle, Tick *tick, int duration);

char* candle_to_json(Candle *candle);

Candle* candle_copy(Candle candle);

#endif 

