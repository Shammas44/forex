#ifndef CREATEAORDER_USECASE_H
#define CREATEAORDER_USECASE_H
#include "Usecase.h"
#include "Order.repository.h"
#define T CreateOrderUsecase

typedef struct Usecase T;

T*createOrderUsecase_constructor(OrderRepository *orderRepository);

#undef T
#endif
