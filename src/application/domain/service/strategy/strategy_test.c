#include "Strategy_test.h"
#include "common.h"
#include "EquityReport.h"
#define T Strategy_test
static char* report_name = "equity.csv";

typedef struct {
  Metadata*metadata;
  Report*report;
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
  private->report = equityReport_constructor(metadata,report_name);
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
  Report *report = private->report;
  if (private->first_run) {
      private->first_run = false;
      report->erase(report);
      report->add_header(report);
  }
  double close = candle->close(candle);
  double open = candle->open(candle);
  int market_position = metadata->get_market_position(metadata);
  Order * order = order_constructor(-1);
  order->price(order,WRITE,close);
  order->status(order,WRITE,ORDER_CANCELLED);
  order->type(order,WRITE,strdup("Market"));

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
  Report_entry entry = {.timestamp=candle->timestamp(candle)};
  report->add_entry(report,&entry);
  return order;
}

#undef T
#undef EQUITY_FILE
