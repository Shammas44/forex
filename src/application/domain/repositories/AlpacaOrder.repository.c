#include "AlpacaOrder.repository.h"
#include "Parser.h"
#include "common.h"
#include "https.h"
#define T AlpacaOrderRepository

static void __destructor(T *self);
static Order *__create(T *self, Order *order);
static Order *__delete(T *self, OrderId id);
static Order *__update(T *self, OrderUpdateArgs args);
static Order *__replace(T *self, OrderUpdateArgs args);
static HttpsRequest *_$build_query(T *self, char *route, HttpsRequest_method method, Order *body);
static HttpsResponse *_$send(T *self, char *route, HttpsRequest_method method, Order *body);
static void *_$handle_response(T *self, HttpsResponse *response);

typedef struct {
  HttpsRequestBuilder *req_builder;
  Https *https;
  char *host;
  Parser *response_body_parser;
} Private;


T *alpacaOrderRepository_constructor(OrderRepositoryArgs args) {
  T *self = (T *)malloc(sizeof(T));
  Private *private = malloc(sizeof(Private));
  self->__private = private;
  self->__destructor = (IsDestroyable){.destructor = __destructor};
  return self;
}

static void __destructor(T *self) {}

static Order *__create(T *self, Order *order) {
  Private *private = self->__private;
  Parser *parser = private->response_body_parser;
  HttpsResponse *response = _$send(self, "order", POST, NULL);
  return _$handle_response(self, response);
}

static Order *__delete(T *self, OrderId id);
static Order *__update(T *self, OrderUpdateArgs args);
static Order *__replace(T *self, OrderUpdateArgs args);

static HttpsRequest *_$build_query(T *self, char *route, HttpsRequest_method method, Order *body) {
  Private *private = self->__private;
  HttpsRequestBuilder *req_builder = private->req_builder;
  char *host = private->host;
  char *url = malloc(strlen(host) + strlen(route) + 1);
  sprintf(url, "%s%s", host, route);
  req_builder->build(req_builder, url);
  req_builder->add_header(req_builder, "Connection: keep-alive");
  req_builder->add_header(req_builder, "Accept-Encoding: gzip,deflat,br");
  req_builder->add_header(req_builder, "Apca-Api-Key-Id: changeMe");
  req_builder->add_header(req_builder, "Apca-Api-Secret-Key: changeMe");
  // if(body){
  //   char*json = order_to_json(body);
  //   req_builder->set_body(req_builder, json);
  // }
  HttpsRequest *req = req_builder->get(req_builder);
  req->set_method(req, method);
  return req;
}

static void *_$handle_response(T *self, HttpsResponse *response) {
  Private *private = self->__private;
  Parser *parser = private->response_body_parser;
  char *status = response->status(response);
  char *body = response->body(response);
  if (strcmp(status, "200") == 0) {
    Hashmap *map = parser->parse(parser, body);
    return map;
  } else {
    char msg[] = "Order creation error: https status code";
    char *error = malloc(strlen(status) + strlen(msg) + 2);
    sprintf(error, "%s %s", msg, status);
    RUNTIME_ERROR(error, 1);
    free(error);
    return NULL;
  }
}

static HttpsResponse *_$send(T *self, char *route, HttpsRequest_method method, Order *body) {
  Private *private = self->__private;
  Https *https = private->https;
  HttpsRequest *request = _$build_query(self, route, method, body);
  return https->get(https, request);
}

#undef T
