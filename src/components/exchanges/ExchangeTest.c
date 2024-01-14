// "tests/hashmap.test.c"
#include "ExchangeTest.h"
#include "common.h"
#include "hashmap.h"
#include "CandleWrapper.h"
#include "json.h"
#define T ExchangeTest
#define NUMBER_OF_OBSERVERS 10

#define GET_REQUEST(exchange,route) \
({ \
  Private *_private = exchange->__private; \
  WsHandler *_ws = _private->ws; \
  Https *_https = _ws->get_https_handler(_ws); \
  HttpsRequest *_request = __build_query(exchange,route); \
  _request; \
})

typedef enum Mode {
Live,Paper
}Mode; 

typedef enum Candle_state {
  CANDLE_IN_PROGRESS,
  CANDLE_COMPLETED,
  CURRENT_TICK_SKIPPED
} Candle_state;

typedef struct {
  WsHandler *ws;
  HttpsRequestBuilder *req_builder;
  Parser*parser;
  char*token;
  ConfigWrapper*config;
  Subject*subject;
  SSL*ssl;
} Private;

static char* host = "https://127.0.0.1:443/api/";

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method);
static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body);
static void __account_informations(T*exchange);
static int __connect(T*exchange);
static void __subscribe(T*exchange,char*path);
static void __attach_observer(T*exchange,Observer*observer);
static void __dettach_observer(T*exchange,Observer*observer);
static void __notify(T* exchange);
static Candle_state __candle_create(CandleWrapper *candle, CandleWrapper *tick, int duration);

T *exchangeTest_constructor(WsHandler *handler,ConfigWrapper*config, Parser *parser){
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
  private->ws = handler;
  private->parser = parser;
  private->req_builder = req_builder;
  private->config = config;
  private->subject = subject_constructor(NULL);
  char*mode = config->mode(config);
  if(strcmp(mode,"BACKTEST") == 0) return NULL;

  // bool paper = config->paper(config);
  // if(paper == true) private->mode = Paper;
  // else if(paper == false) private->mode = Live;
  // else private->mode = Paper;

  self->__private = private;
  return self;
}

static int __connect(T*exchange){
  Private *private = exchange->__private;
  ConfigWrapper *config = private->config;
  char*email = config->get(config,"email");
  char*password = config->get(config,"password");

  char body_tmp[] = "{"
                    "\"email\":\"%s\","
                    "\"password\":\"%s\""
                    "}";

  char *body = malloc(strlen(body_tmp) + strlen(email) + strlen(password) + 1);
  sprintf(body, body_tmp, email, password);
  HttpsResponse * response = __send(exchange,"auth",POST, body);
  if(strcmp(response->status(response),"200") ==0 ){
    Hashmap*map = NULL;
    json_to_map(response->body(response),&map,NULL,NULL);
    private->token = map->get(map,"token").value;
    return 0;
  }
  return 1;
}

static void __attach_observer(T*exchange,Observer*observer){
  Private *private = exchange->__private;
  Subject *subject = private->subject;
  subject->attach(subject,observer);
}

static void __on_frame_receive(void*exchange,void*state){
  T *self = exchange;
  Private *private = self->__private;
  Subject *subject = private->subject;
  Parser *parser = private->parser;
  Hashmap*map = parser->parse(parser,state);
  WsHandler *ws = private->ws;
  SSL*ssl = private->ssl;
  ws->close(ws,ssl);

  // ws->send(ws,ssl,"{\"action\":\"auth\",\"data\":{\"key\":\"changeMe\"}}");
  
  int capacity = 10;
  CandleWrapper *candle = NULL;
  int current_sec = -1;
  bool isFirst = true;
  // while (1) {
    // void *data = mtqueue_dequeue(queues->ticks);
    // Tick *tick = (Tick *)data;
    // if (candle == NULL)
    //   candle = malloc(sizeof(CandleWrapper));
    // if (__candle_create(candle, tick, 5) == CANDLE_COMPLETED) {
      // mtqueue_enqueue(queues->bars, &candle);
    //   candle = NULL;
    // };
    // free(tick);
  // }




  subject->set_state(subject,map);
}


// static Candle_state __candle_create(CandleWrapper *candle, CandleWrapper *tick, int duration) {
//   static time_t current_interval_start = 0;
//   static float high = 0;
//   static float low = 0;
//   static float open = 0;
//   static float volume = 0;
//   static time_t timestamp = 0;
//   static CandleWrapper *candle = NULL;

//   // if (!tick_is_valid(*tick)) {
//   //   return CURRENT_TICK_SKIPPED;
//   // };

//   // Initialization when starting
//   if (current_interval_start == 0) {
//     candle = tick;
//     return CANDLE_IN_PROGRESS;
//   }

//   // Check if this tick is still within the current interval
//   if (tick->timestamp < current_interval_start + duration) {
//     if (tick->last_price > high)
//       high = tick->last_price;
//     if (tick->last_price < low)
//       low = tick->last_price;
//     volume += tick->last_quantity;
//     return CANDLE_IN_PROGRESS;
//   }

//   // If we've reached here, the current tick is outside of the current interval,
//   // meaning we need to finalize the current candle and start a new one

//   // Set the old candle values
//   candle->high = high;
//   candle->low = low;
//   candle->open = open;
//   candle->volume = volume;
//   candle->timestamp = timestamp;
//   candle->close =
//       tick->last_price; // This wasn't in your original code for this part

//   // Reset for the new candle interval
//   current_interval_start = tick->timestamp;
//   high = tick->last_price;
//   low = tick->last_price;
//   open = tick->last_price;
//   volume = tick->last_quantity;
//   timestamp = tick->timestamp;

//   return CANDLE_COMPLETED; // We've finalized a candle and returned it
// }

static void __dettach_observer(T*exchange,Observer*observer){
  Private *private = exchange->__private;
  Subject *subject = private->subject;
  subject->detach(subject,observer);
}

static void __notify(T* exchange) {
  Private *private = exchange->__private;
  Subject *subject = private->subject;
  subject->notify(subject);
}

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method){
  Private *private = exchange->__private;
  HttpsRequestBuilder *req_builder = private->req_builder;
  char *url = malloc(strlen(host) + strlen(route) + 1);
  sprintf(url, "%s%s", host, route);
  req_builder->build(req_builder,url);
  req_builder->add_header(req_builder,"Connection: keep-alive");
  req_builder->add_header(req_builder,"Accept-Encoding: gzip,deflat,br");
  req_builder->add_header(req_builder,"Content-Type: application/json");
  req_builder->add_header(req_builder,"Apca-Api-Key-Id: changeMe");
  req_builder->add_header(req_builder,"Apca-Api-Secret-Key: changeMe");
  req_builder->set_method(req_builder,method);
  HttpsRequest * req = req_builder->get(req_builder);
  return req;
}

static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body){
  Private *private = exchange->__private; 
  WsHandler *ws = private->ws; 
  Https *https = ws->get_https_handler(ws); 
  HttpsRequest *request = __build_query(exchange,route,method);
  request->set_body(request,body);
  return https->fetch(https,request);
}

// Account
// =========================================================================="

static void __account_informations(T*exchange){
  HttpsResponse * response = __send(exchange,"account",GET, NULL);
}

static void __subscribe(T*exchange,char*path){
  Private *private = exchange->__private;
  Subject*subject = private->subject;
  WsHandler *ws = private->ws;
  HttpsRequestBuilder *req_builder = private->req_builder;
  req_builder->build(req_builder,host);

  char*authorization = malloc(strlen(private->token) + strlen("Authorization: Bearer ") + 1);
  sprintf(authorization,"Authorization: Bearer %s",private->token);
  req_builder->add_header(req_builder,authorization);
  req_builder->add_header(req_builder,"Connection: Upgrade");
  req_builder->add_header(req_builder,"Upgrade: websocket");
  req_builder->add_header(req_builder,"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==");
  req_builder->add_header(req_builder,"Sec-WebSocket-Version: 13");
  HttpsRequest * req = req_builder->get(req_builder);
  SSL* ssl = ws->handshake(ws,req);
  private->ssl = ssl;
  if(ssl ==NULL ) return;
  ws->listen(ws,ssl,exchange,__on_frame_receive);
}

#undef T
#undef NUMBER_OF_OBSERVERS
#undef GET_REQUEST
