#ifndef ORDER_H
#define ORDER_H
#include "jsmn.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>


typedef enum Order_status {
  // ORDER_OPEN,
  // ORDER_FILLED,
  ORDER_CANCELLED,
  ORDER_REJECTED,
  // ORDER_EXPIRED,
  ORDER_PENDING,
  ORDER_EXECUTED,
  ORDER_STOP,
} Order_status;

typedef enum Side {
  BUY,
  SELL,
} Side;

typedef struct Order {
  int id;
  char type[20];
  Order_status status;
  double limit;
  double price;
  Side side;
  double size;
  double executed_price;
  int ttl;
} Order;

#define ORDER_PROPS_NUM 7


void order_print(Order order);

char *order_to_json(Order *order);

Order order_create(char *json, jsmntok_t **tokens, int token_num);

char *order_get_status(Order order);

#endif
