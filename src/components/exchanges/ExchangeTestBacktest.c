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
} Private;

static int __connect(T*exchange);
static void __subscribe(T*exchange,char*path);
static void __attach_observer(T*exchange,Observer*observer);
static void __dettach_observer(T*exchange,Observer*observer);
static void __on_row_receive(void*exchange,void*state);

T* exchangeTestBacktest_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser){
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
  private->ws = ws;
  private->response_parser = parser;
  private->req_builder = req_builder;
  private->config = config;
  private->subject = subject_constructor(NULL);
  char*mode = config->mode(config);
  if(strcmp(mode,"BACKTEST") != 0) return NULL;
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

#undef T
#undef SUBJECT
