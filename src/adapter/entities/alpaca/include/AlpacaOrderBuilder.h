#ifndef ALPACAORDERBUILDER_H
#define ALPACAORDERBUILDER_H
#include "AlpacaOrder.entity.h"

#define T AlpacaOrderBuilder

typedef struct T T;

typedef struct  {
  char* symbol;
  AlpacaOrder_Side side;
  AlpacaOrder_Type type;
  AlpacaOrder_Time_in_force time_in_force;
}AlpacaOrderBuilderBuildArgs;

struct T {
  IsDestroyable __destructor;
  void (*destructor)(T*self);
  void (*build)(T*self, AlpacaOrderBuilderBuildArgs args);
  void (*qty)(T*self, double qty);
  void (*notional)(T*self, char* arg);
  void (*limit_price)(T*self, double arg);
  void (*stop_price)(T*self, double arg);
  void (*trail_price)(T*self, double arg);
  void (*trail_percent)(T*self, char* arg);
  void (*extended_hours)(T*self, bool arg);
  void (*client_order_id)(T*self, char* arg);
  void (*order_class)(T*self, AlpacaOrder_Class arg);
  void (*take_profit)(T*self, Hashmap* arg);
  void (*stop_loss)(T*self, Hashmap* arg);
  AlpacaOrderEntity *(*get)(T*self);
  void * __private;
};

T *alpacaOrderBuilder_constructor();

#undef T
#endif 
