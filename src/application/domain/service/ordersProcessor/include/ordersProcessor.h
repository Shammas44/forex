#ifndef ORDERSPROCESSOR_H
#define ORDERSPROCESSOR_H
#include "candle.h"
#include "mtqueue.h"
#include "Order.h"
#include "candle_order.h"


void *process_orders(void *arg);
#endif 
