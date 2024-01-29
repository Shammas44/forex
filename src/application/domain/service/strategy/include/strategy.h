#ifndef STRATEGY_H
#define STRATEGY_H
#include "Metadata.h"
#include "CandleWrapper.h"
#include "Order.h"

#define T Strategy

typedef struct T T;

typedef T* (Strategy_constructor)(Metadata *metadata);
typedef void (Strategy_destructor)(T *self);
typedef Order * (Strategy_execute)(T *self,CandleWrapper* candle);
typedef void (Strategy_cleanup)(T *self);

typedef struct T {
  Strategy_destructor* destructor;
  Strategy_execute* execute;
  Strategy_cleanup* cleanup;
  void * __private;
} T;

#undef T
#endif 
