#ifndef CANDLEWRAPPER_H
#define CANDLEWRAPPER_H
#include "hashmap.h"
#include "wrapper.h"

#define T CandleWrapper
#define F(name) T##_##name

typedef struct T T;

typedef void(F(destructor))(T *self);
typedef char* (F(get))(T *self,char*key);

typedef char* (F(date))(T *self);
typedef char* (F(time))(T *self);
typedef double (F(open))(T *self);
typedef double (F(high))(T *self);
typedef double (F(low))(T *self);
typedef double (F(close))(T *self);
typedef double (F(up_volume))(T *self);
typedef double (F(down_volume))(T *self);
typedef double (F(total_volume))(T *self);
typedef double (F(up_ticks))(T *self);
typedef double (F(down_ticks))(T *self);
typedef int (F(total_ticks))(T *self);

typedef struct T {
  F(destructor) *destructor;
  F(get) *get;
  F(date) *date;
  F(time) *time;
  F(open) *open;
  F(high) *high;
  F(low) *low;
  F(close) *close;
  F(up_volume) *up_volume;
  F(down_volume) *down_volume;
  F(total_volume) *total_volume;
  F(up_ticks) *up_ticks;
  F(down_ticks) *down_ticks;
  F(total_ticks) *total_ticks;
  void *__private;
} T;

T *candleWrapper_constructor(Hashmap *map);

#undef T
#undef F
#endif
