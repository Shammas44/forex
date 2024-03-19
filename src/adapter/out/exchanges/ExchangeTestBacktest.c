#include "ExchangeTestBacktest.h"
#include "Message.h"
#define T ExchangeTestBacktest

#define SUBJECT(exchange) \
({ \
  Private *_private = exchange->__private; \
  Subject *_subject = _private->subject; \
  _subject; \
})

typedef struct {
WsHandler *ws;
  HttpsRequestBuilder *req_builder;
  Parser*response_parser;
  char*token;
  ConfigWrapper*config;
  Subject*subject;
  Metadata*metadata;
} Private;

static int __connect(T*self);
static void __subscribe(T*self,char*path);
static void __attach_observer(T*self,Observer*observer);
static void __dettach_observer(T*self,Observer*observer);
static void __on_row_receive(void*self,void*state);
static void __send_order(T*self,Order*order);

static double _$calculate_broker_commision(Metadata *m, Order *order);
static double _$calculate_slippage(double size);
static double _$get_value_of_one_pipe(double exchange_rate, double size);
static double _$emulate_broker(T*self,Order *order);

T* exchangeTestBacktest_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser,Metadata*metadata){
  T *self = malloc(sizeof(T));
  if (self == NULL) return NULL;
  Private *private = malloc(sizeof(Private));
  if (private == NULL) return NULL;
  HttpsRequestBuilder *req_builder = httpsRequestBuilder_constructor();
  if (req_builder == NULL) return NULL;
  self->subscribe = __subscribe;
  self->connect = __connect;
  self->attach_observer = __attach_observer;
  self->dettach_observer = __dettach_observer;
  self->send_order = __send_order;
  private->ws = ws;
  private->response_parser = parser;
  private->req_builder = req_builder;
  private->config = config;
  private->metadata = metadata;
  private->subject = subject_constructor(NULL);
  Metadata_mode mode = config->mode(config);
  if(mode != Metadata_mode_backtest ){
    RUNTIME_ERROR("ExchangeTestBacktest mode should be set to backtest",1);
    return NULL;
  }    
  self->__private = private;
  return self;
}

static int __connect(T*exchange){
  return 0;
}

static void __attach_observer(T*exchange,Observer*observer){
  Subject *subject = SUBJECT(exchange);
  subject->attach(subject,observer);
}

static void __on_row_receive(void*exchange,void*state){
  T *self = exchange;
  Private *private = self->__private;
  Subject *subject = private->subject;
  Message *message = state;
  Msg_type type = message->code(message,READ,0);
  if(type == Msg_unknown){
    message->code(message,WRITE,Msg_candle);
    Hashmap*map = message->value(message,READ,(Item){}).value;
  }
  Hashmap *map = state;
  subject->set_state(subject,message);
}

static void __dettach_observer(T*exchange,Observer*observer){
  Subject *subject = SUBJECT(exchange);
  subject->detach(subject,observer);
}

static void __notify(T* exchange) {
  Subject *subject = SUBJECT(exchange);
  subject->notify(subject);
}

static void __subscribe(T*exchange,char*backtest_path){
  Private *private = exchange->__private;
  ConfigWrapper *config = private->config;
  Parser *parser = private->response_parser;
  if(backtest_path == NULL) backtest_path = config->backtest_data(config); 
  parser->parse_stream(parser,backtest_path,exchange,__on_row_receive);
}

// Account
// =========================================================================="

// static void __account_informations(T*exchange){
//   HttpsResponse * response = __send(exchange,"account",GET, NULL);
// }

static double _$calculate_broker_commision(Metadata *m, Order *order){
  double size = order->size(order,READ,0) * order->price(order,READ,0);
  double value = size / 1000000 * m->get_broker_commision_dpm(m);
  return value;
}

static double _$calculate_slippage(double size){
  double slippage = 0;
  if(size > 50000){
    slippage = 0.0001;
  }else if(size > 500000){
    slippage = 0.0002;
  }
  return slippage;
}

static double _$get_value_of_one_pipe(double exchange_rate, double size){
  return 0.0001 / exchange_rate * size;
}

static double _$emulate_broker(T*self,Order *order) {
  Private *private = self->__private;
  Metadata *metadata = private->metadata;
  double spread = 0;
  double slippage = 0;
  double commision = 0;
  if(order->side(order,READ,0)== BUY){
  spread = metadata->get_spread(metadata);
  }
  slippage = _$calculate_slippage(order->size(order,READ,0));
  //TODO debug commision
  commision = _$calculate_broker_commision(metadata, order);
  // printf("commision: %f\n",commision);
  return order->price(order,READ,0) + spread + slippage;
}

static void __send_order(T*self,Order*order){
  Private *private = self->__private;
  Metadata *metadata = private->metadata;
  double price = _$emulate_broker(self,order);
  double size = order->size(order,READ,0);
  int number_of_trades = metadata->get_number_of_trades(metadata);
  metadata->set_number_of_trades(metadata, number_of_trades+1);
  metadata->set_last_price(metadata,price);
  int factor = order->side(order,READ,0) == SELL ? -1 : 1;
  metadata->set_market_position(metadata,size*factor);
  order->destructor(order);
}

#undef T
#undef SUBJECT
