#ifndef ALPACAORDER_REPOSITORY_H
#define ALPACAORDER_REPOSITORY_H
#include "Order.repository.h"
#include "https.h"
#define T AlpacaOrderRepository

typedef struct OrderRepository T;

typedef struct {
  char *host;
  HttpsRequestBuilder *req_builder;
  Https *https;
  Parser *response_body_parser;
} OrderRepositoryArgs;

T *alpacaOrderRepository_constructor(OrderRepositoryArgs args);

#undef T
#endif
