#include "StrategyProcessor.h"
#include "common.h"
#define T StrategyProcessor

typedef struct {
  int order_id;
  Metadata *metadata;
  Strategy*strategy;
} Private;

static void __destructor(T *self);
static Order *__run(T *self, CandleWrapper *candle);
static void __set_strategy(T *self, Strategies strategy);

T *strategyProcessor_constructor(Metadata *metadata) {
  if(metadata == NULL) return NULL;
  T *self = malloc(sizeof(T));
  self->run = __run;
  self->destructor = __destructor;
  self->set_strategy = __set_strategy;
  Private *private = malloc(sizeof(Private));
  private->order_id = 0;
  private->strategy = NULL;
  private->metadata = metadata;
  self->__private = private;
  return self;
}

static void __destructor(T *self) {
  free(self->__private);
  free(self);
}

static Order *__run(T *self,CandleWrapper *candle) {
  Private*private = self->__private;
  Metadata *metadata = private->metadata;
  int order_id = private->order_id;
  Strategy *strategy = private->strategy;

  if (strategy == NULL) {
    exit(RUNTIME_ERROR("Strategy not found", 1));
  }

  metadata->set_equity(metadata, candle->close(candle));
  metadata->set_last_price(metadata, candle->close(candle));

  Order *order = strategy->execute(strategy, candle);
  free(candle);

  if (order->size != 0) {
    int id = order->id(order,READ,NULL) + 1;
    order->id(order,WRITE,id); 
  }
  return order;
}

static void __set_strategy(T *self, Strategies strategy_id) {
  Private *private = self->__private;
  Strategy_constructor *constructor = strategies[strategy_id];
  Strategy *strategy = constructor(private->metadata);
  if (strategy == NULL) {
    exit(RUNTIME_ERROR("Strategy not found", 1));
  }
  private->strategy = strategy;
}

#undef T
