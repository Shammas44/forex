#ifndef CANDLE_ORDER_H
#define CANDLE_ORDER_H
#include "candle.h"
#include "order.h"

typedef struct Candle_order {
  Order *order;
  Candle *candle;
} Candle_order;

#endif 
