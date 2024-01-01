#ifndef CANDLE_ORDER_H
#define CANDLE_ORDER_H
#include "candle.h"
#include "Order.h"

typedef struct Candle_order {
  Order *order;
  Candle *candle;
} Candle_order;

#endif 
