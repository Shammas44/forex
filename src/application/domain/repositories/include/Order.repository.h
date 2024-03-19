#ifndef ORDER_REPOSITORY_H
#define ORDER_REPOSITORY_H
#include "IsDestroyable.h"
#include "Order.h"
#define T OrderRepository

typedef struct T T;

typedef struct OrderUpdateArgs OrderUpdateArgs;
typedef struct OrderId OrderId;

typedef struct T {
    IsDestroyable __destructor;
    Order* (*create)(T *self, Order* order);
    Order* (*delete)(T *self,OrderId id);
    Order* (*update)(T *self, OrderUpdateArgs args);
    Order* (*replace)(T *self, OrderUpdateArgs args);
    void (*destructor)(T *self);
    void*__private;
} T;

#undef T
#endif
