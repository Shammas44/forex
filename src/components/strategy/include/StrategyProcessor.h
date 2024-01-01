#ifndef STRATEGYPROCESSOR_H
#define STRATEGYPROCESSOR_H
#include "Order.h"
#include "Metadata.h"
#include "Strategies.h"

#define T StrategyProcessor

typedef struct T T;

typedef void (StrategyProcessor_destructor)(T*self);
typedef void (StrategyProcessor_set_strategy)(T*self,Strategies strategy);
typedef Order* (StrategyProcessor_run)(T*self,CandleWrapper*candle);


typedef struct T {
  StrategyProcessor_destructor *destructor;
  StrategyProcessor_set_strategy *set_strategy;
  StrategyProcessor_run *run;
  void *__private;
} T;

T* strategyProcessor_constructor(Metadata *metadata);

#undef T
#endif
