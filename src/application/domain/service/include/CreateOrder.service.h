#ifndef CREATEAORDER_SERVICE_H
#define CREATEAORDER_SERVICE_H
#include "CreateOrder.usecase.h"
#include "Order.repository.h"
#define T CreateOrderService

typedef struct Usecase T;

typedef struct OrderUpdateArgs {
  Order* order;
  char* id;
} OrderUpdateArgs; 

typedef struct OrderId {
  char* id;
}OrderId; 

T*createOrderService_constructor(OrderRepository *orderRepository);

#undef T
#endif
