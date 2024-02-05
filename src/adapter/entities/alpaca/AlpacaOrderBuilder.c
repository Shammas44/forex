#include "AlpacaOrderBuilder.h"
#include <stdio.h>
#define T AlpacaOrderBuilder

#define PRIV(self) \
({ \
  Private *_private = self->__private; \
  _private; \
})
typedef struct {
  AlpacaOrderEntity *order;
  AlpacaOrderArgs args;
} Private;

static void __destructor(T *self);
static AlpacaOrderEntity *__get(T *self);
static void __build(T *self, AlpacaOrderBuilderBuildArgs args);
static void __qty(T *self, double value);
static void __limit_price(T *self, double value);
static void __stop_price(T *self, double value);
static void __notional(T *self, char* value);
static void __trail_percent(T *self, char* value);
static void __trail_price(T *self, double value);
static void __extended_hours(T *self, bool value);
static void __client_order_id(T *self, char* value);
static void __order_class(T *self, AlpacaOrder_Class value);
static void __take_profit(T *self, Hashmap* value);
static void __stop_loss(T *self, Hashmap* value);

T *alpacaOrderBuilder_constructor() {
  T *self = (T *)malloc(sizeof(T));
  Private *private = calloc(0,sizeof(Private));
  self->__private = private;
  self->destructor = __destructor;
  self->build = __build;
  self->get = __get;
  self->qty = __qty;
  self->notional = __notional;
  self->limit_price = __limit_price;
  self->stop_price = __stop_price;
  self->trail_price = __trail_price;
  self->trail_percent = __trail_percent;
  self->extended_hours = __extended_hours;
  self->client_order_id = __client_order_id;
  self->order_class = __order_class;
  self->take_profit = __take_profit;
  self->stop_loss = __stop_loss;
  return self;
}

static void __destructor(T *self) {
  Private *private = self->__private;
  free(private);
  free(self);
}

static void __build(T *self, AlpacaOrderBuilderBuildArgs args) {
  Private *private = self->__private;
  private->args.symbol = args.symbol;
  private->args.type = args.type;
  private->args.side = args.side;
  private->args.time_in_force = args.time_in_force;
}

static AlpacaOrderEntity *__get(T *self) {
  Private *private = self->__private;
  AlpacaOrderArgs args = private->args;
  AlpacaOrderEntity *entity = alpacaOrderEntity_constructor(args);
  return entity;
}

static void __qty(T *self, double value){
  PRIV(self)->args.qty = value;
}

static void __notional(T *self, char* value){
  PRIV(self)->args.notional = value;
}

static void __limit_price(T *self, double value){
  PRIV(self)->args.limit_price = value;
}

static void __stop_price(T *self, double value){
  PRIV(self)->args.stop_price = value;
}

static void __trail_price(T *self, double value){
  PRIV(self)->args.trail_price = value;
}

static void __trail_percent(T *self, char* value){
  PRIV(self)->args.trail_percent = value;
}

static void __extended_hours(T *self, bool value){
  PRIV(self)->args.extended_hours = value;
}

static void __client_order_id(T *self, char* value){
  PRIV(self)->args.client_order_id = value;
}

static void __order_class(T *self, AlpacaOrder_Class value){
  PRIV(self)->args.order_class = value;
}

static void __take_profit(T *self, Hashmap* value){
  PRIV(self)->args.take_profit = value;
}

static void __stop_loss(T *self, Hashmap* value){
  PRIV(self)->args.stop_loss = value;
}

#undef PRIV
#undef T
