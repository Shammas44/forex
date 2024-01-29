#ifndef RISKPROCESSOR_H
#define RISKPROCESSOR_H
#include "IsDestroyable.h"
#include "Metadata.h"
#include "order.h"
#define T RiskProcessor

typedef struct T T;

typedef struct T {
  IsDestroyable __destructor;
  void (*destructor)(T *self);
  Order* (*process)(T *self, Order *order);
  void *__private;
} T;

T*riskProcessor_constructor(Metadata*metadata);

#endif
#undef T
