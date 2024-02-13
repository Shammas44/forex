#ifndef ORDER_H
#define ORDER_H
#include "common.h"
#include "jsmn.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>

#define T Order

typedef struct T T;

typedef enum OrderStatus {
  // ORDER_OPEN,
  // ORDER_FILLED,
  ORDER_CANCELLED,
  ORDER_REJECTED,
  // ORDER_EXPIRED,
  ORDER_PENDING,
  ORDER_EXECUTED,
  ORDER_STOP,
} OrderStatus;

typedef enum Side {
  BUY,
  SELL,
} Side;

typedef struct T {
  void (*destructor)(T *self);
  int (*id)(T *self, Access_mode mode, int new);
  char *(*type)(T *self, Access_mode mode, char *new);
  OrderStatus (*status)(T *self, Access_mode mode, OrderStatus new);
  double (*limit)(T *self, Access_mode mode, double new);
  double (*price)(T *self, Access_mode mode, double new);
  Side (*side)(T *self, Access_mode mode, Side new);
  double (*size)(T *self, Access_mode mode, double new);
  double (*executed_price)(T *self, Access_mode mode, double new);
  int (*ttl)(T *self, Access_mode mode, int new);
  char *(*to_json)(T *self);
  void *__private;
} T;

T *order_constructor(int id);

// #define ORDER_PROPS_NUM 7

// void order_print(Order order);

// char *order_to_json(Order *order);

// Order order_create(char *json, jsmntok_t **tokens, int token_num);

// char *order_get_status(Order order);

#undef T
#endif
