#include "Strategy_test.h"
#include "common.h"
#define EQUITY_FILE "equity.csv"
#define T Strategy_test

typedef struct {
  Metadata*metadata;
  bool first_run;
} Private;

static void __destructor(T*self);
static Order* __execute(T*self, CandleWrapper*candle);

T* strategy_test_constructor(Metadata*metadata){
  if(metadata==NULL){
    RUNTIME_ERROR("metadata is NULL",1);
    return NULL;
  }
  T*self=malloc(sizeof(T));
  Private * private = malloc(sizeof(Private));
  private->first_run = true;
  private->metadata = metadata;
  self->__private = private;
  self->destructor = __destructor;
  self->execute = __execute;
  return self;
}

static void __destructor(T*self){
  if(self==NULL){
    RUNTIME_ERROR("self is NULL",1);
    return;
  }
  free(self->__private);
  free(self);
}

static Order* __execute(T*self, CandleWrapper*candle){
  Private * private = self->__private;
  Metadata *metadata = private->metadata;
  if (private->first_run) {
      private->first_run = false;
//     csv_erase_content(EQUITY_FILE);
//     csv_add_new_line(EQUITY_FILE, "equity,timestamp");
  }
  double close = candle->close(candle);
  double open = candle->open(candle);
  int market_position = metadata->get_market_position(metadata);
  Order * order = order_constructor(-1);
  order->price(order,WRITE,close);
  order->status(order,WRITE,ORDER_CANCELLED);
  order->type(order,WRITE,"Market");

  if (close > open && market_position >= 0) {
    order->side(order,WRITE,SELL);
    order->status(order,WRITE,ORDER_PENDING);
    if (market_position == 0) {
    order->size(order,WRITE,10000);
    } else {
    order->size(order,WRITE,20000);
    }
  }

  if (close < open && market_position <= 0) {
    order->side(order,WRITE,BUY); 
    order->status(order,WRITE,ORDER_PENDING);
    if (market_position == 0) {
      order->size(order,WRITE, 10000);
    } else {
      order->size(order,WRITE, 20000);
    }
  }
//   strategy_test_report(metadata, candle.timestamp);
  return order;
}

#undef T
#undef EQUITY_FILE

// void strategy_test_report(Tsmetadata *m, time_t timestamp) {
//   double equity = tsmetadata_get_equity(m);
//   char line[50];
//   sprintf(line, "%f,%ld", equity, timestamp);
//   csv_add_new_line(EQUITY_FILE, line);
// }
