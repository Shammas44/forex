#ifndef CANDLEWRAPPER_H
#define CANDLEWRAPPER_H
#include "Hashmap.h"
#include <time.h>

#define T CandleWrapper

typedef struct T T;

typedef struct T {
  void (*destructor)(T *self);
  void *(*get)(T *self, char *key);
  char *(*date)(T *self);
  char *(*time)(T *self);
  double (*open)(T *self);
  double (*high)(T *self);
  double (*low)(T *self);
  double (*close)(T *self);
  double (*up_volume)(T *self);
  double (*down_volume)(T *self);
  double (*total_volume)(T *self);
  double (*up_ticks)(T *self);
  double (*down_ticks)(T *self);
  double (*total_ticks)(T *self);
  time_t (*timestamp)(T *self);
  double (*volume)(T *self);
  void *__private;
} T;

T *candleWrapper_constructor(Hashmap *map);

#undef T
#undef F
#endif
