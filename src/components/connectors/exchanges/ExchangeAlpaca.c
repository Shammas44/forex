#include "ExchangeAlpaca.h"
#define T ExchangeAlpaca

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

typedef struct {
WsHandler *ws;
  HttpsRequestBuilder *req_builder;
  char *key_id;
  char *secret_key;
  Mode mode;
} Private;

static char* host = "https://paper-api.alpaca.markets/v2/";

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method);
static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body);
static void __account_informations(T*exchange);

T *exchangeAlpaca_constructor(WsHandler *handler, Parser *parser){
  T *self = malloc(sizeof(T));
  if (self == NULL) return NULL;
  Private *private = malloc(sizeof(Private));
  if (private == NULL) return NULL;
  HttpsRequestBuilder *req_builder = httpsRequestBuilder_constructor();
  if (req_builder == NULL) return NULL;
  private->ws = handler;
  private->req_builder =
  self->__private = private;


  // self->key_id = key_id;
  // self->secret_key = secret_key;
  // self->paper = paper;
  return self;
}

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method){
  Private *private = exchange->__private;
  HttpsRequestBuilder *req_builder = private->req_builder;
  char *url = malloc(strlen(host) + strlen(route) + 1);
  sprintf(url, "%s%s", host, route);
  req_builder->build(req_builder,url);
  req_builder->add_header(req_builder,"Connection: keep-alive");
  req_builder->add_header(req_builder,"Accept-Encoding: gzip,deflat,br");
  req_builder->add_header(req_builder,"Apca-Api-Key-Id: changeMe");
  req_builder->add_header(req_builder,"Apca-Api-Secret-Key: changeMe");
  HttpsRequest * req = req_builder->get(req_builder);
  req->set_method(req,method);
  return req;
}

static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body){
  Private *private = exchange->__private; 
  WsHandler *ws = private->ws; 
  Https *https = ws->get_https_handler(ws); 
  HttpsRequest *request = __build_query(exchange,route,method);
  return https->get(https,request);
}

// Account
// =========================================================================="

static void __account_informations(T*exchange){
  HttpsResponse * response = __send(exchange,"account",GET, NULL);
}

// Orders
// =========================================================================="

static void __all_orders(T*exchange){
  HttpsResponse * response = __send(exchange,"orders",GET,NULL);
}

static void __new_order(T*exchange,char*order){
  HttpsResponse * response = __send(exchange,"orders",POST,order);
}

static void __order_by_id(T*exchange,char*id){
  char *route = malloc(strlen("orders/") + strlen(id) + 1);
  sprintf(route, "orders/%s", id);
  HttpsResponse * response = __send(exchange,"orders",GET,NULL);
  free(route);
}

static void __update_order(T*exchange,char*id,char*order){
  char *route = malloc(strlen("orders/") + strlen(id) + 1);
  sprintf(route, "orders/%s", id);
  HttpsResponse * response = __send(exchange,"orders",PATCH,order);
  free(route);
}

static void __delete_all_orders(T*exchange){
  HttpsResponse * response = __send(exchange,"orders",DELETE,NULL);
}

static void __delete_order_by_id(T*exchange, char*id){
  char *route = malloc(strlen("orders/") + strlen(id) + 1);
  sprintf(route, "orders/%s", id);
  HttpsResponse * response = __send(exchange,route,DELETE,NULL);
  free(route);
}
#undef T
#undef GET_REQUEST
