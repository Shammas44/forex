#ifndef CANDLEWRAPPER_H
#define CANDLEWRAPPER_H
#include "hashmap.h"
#include "wrapper.h"
#include <time.h>

#define T CandleWrapper

typedef struct T T;

typedef void(CandleWrapper_destructor)(T *self);
typedef char* (CandleWrapper_get)(T *self,char*key);

typedef char* (CandleWrapper_date)(T *self);
typedef char* (CandleWrapper_time)(T *self);
typedef double (CandleWrapper_open)(T *self);
typedef double (CandleWrapper_high)(T *self);
typedef double (CandleWrapper_low)(T *self);
typedef double (CandleWrapper_close)(T *self);
typedef double (CandleWrapper_up_volume)(T *self);
typedef double (CandleWrapper_down_volume)(T *self);
typedef double (CandleWrapper_total_volume)(T *self);
typedef double (CandleWrapper_up_ticks)(T *self);
typedef double (CandleWrapper_down_ticks)(T *self);
typedef int (CandleWrapper_total_ticks)(T *self);
typedef time_t (CandleWrapper_timestamp)(T *self);
typedef double (CandleWrapper_volume)(T *self);

typedef struct T {
  CandleWrapper_destructor *destructor;
  CandleWrapper_get *get;
  CandleWrapper_date *date;
  CandleWrapper_time *time;
  CandleWrapper_open *open;
  CandleWrapper_high *high;
  CandleWrapper_low *low;
  CandleWrapper_close *close;
  CandleWrapper_up_volume *up_volume;
  CandleWrapper_down_volume *down_volume;
  CandleWrapper_total_volume *total_volume;
  CandleWrapper_up_ticks *up_ticks;
  CandleWrapper_down_ticks *down_ticks;
  CandleWrapper_total_ticks *total_ticks;
  CandleWrapper_timestamp *timestamp;
  CandleWrapper_volume *volume;
  void *__private;
} T;

T *candleWrapper_constructor(Hashmap *map);

#undef T
#undef F
#endif
