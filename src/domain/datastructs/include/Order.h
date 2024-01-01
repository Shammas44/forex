#ifndef ORDER_H
#define ORDER_H
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


typedef enum {
  READ,
  WRITE,
} Acces_mode;

typedef void(Order_destructor)(T *self);
typedef int(Order_id)(T *self, Acces_mode mode, int new);
typedef char* (Order_type)(T *self, Acces_mode mode, char* new);
typedef OrderStatus(Order_status)(T *self, Acces_mode mode, OrderStatus new);
typedef double(Order_limit)(T *self, Acces_mode mode, double new);
typedef double(Order_price)(T *self, Acces_mode mode, double new);
typedef Side(Order_side)(T *self, Acces_mode mode, Side new);
typedef double(Order_size)(T *self, Acces_mode mode, double new);
typedef double(Order_executed_price)(T *self, Acces_mode mode, double new);
typedef int(Order_ttl)(T *self, Acces_mode mode, int new);

typedef struct T {
  Order_destructor *destructor;
  Order_id *id;
  Order_type *type;
  Order_status *status;
  Order_limit *limit;
  Order_price *price;
  Order_side *side;
  Order_size *size;
  Order_executed_price *executed_price;
  Order_ttl *ttl;
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
