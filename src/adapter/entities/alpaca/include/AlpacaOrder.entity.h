#ifndef ALPACAORDERENTITY_H
#define ALPACAORDERENTITY_H
#include "Hashmap.h"
#include <stdbool.h>
#define T AlpacaOrderEntity

typedef enum {
  AlpacaOrder_Side_BUY = 1,
  AlpacaOrder_Side_SELL,
} AlpacaOrder_Side;

typedef enum {
  AlpacaOrder_Type_Market = 1,
  AlpacaOrder_Type_Limit,
  AlpacaOrder_Type_Stop,
  AlpacaOrder_Type_Stop_Limit,
  AlpacaOrder_Type_Trailing_Stop,
} AlpacaOrder_Type;

typedef enum {
  AlpacaOrder_Time_in_force_Day = 1,
  AlpacaOrder_Time_in_force_Gtc,
  AlpacaOrder_Time_in_force_Opg,
  AlpacaOrder_Time_in_force_Cls,
  AlpacaOrder_Time_in_force_Ioc,
  AlpacaOrder_Time_in_force_Fok,
} AlpacaOrder_Time_in_force;

typedef enum {
  AlpacaOrder_Class_Simple = 1,
  AlpacaOrder_Class_Bracket,
  AlpacaOrder_Class_Oco,
  AlpacaOrder_Class_Oto,
} AlpacaOrder_Class;

typedef struct T T;

typedef struct {
  int code;
  char *value;
} AlpacaOrderItem;

typedef struct T {
  IsDestroyable __destructor;
  void (*destructor)(T *self);
  char *(*symbol)(T *self);
  double (*qty)(T *self);
  char *(*notional)(T *self);
  AlpacaOrderItem (*side)(T *self);
  AlpacaOrderItem (*type)(T *self);
  AlpacaOrderItem (*time_in_force)(T *self);
  double (*limit_price)(T *self);
  double (*stop_price)(T *self);
  double (*trail_price)(T *self);
  char *(*trail_percent)(T *self);
  bool (*extended_hours)(T *self);
  char *(*client_order_id)(T *self);
  AlpacaOrderItem (*order_class)(T *self);
  Hashmap *(*take_profit)(T *self);
  Hashmap *(*stop_loss)(T *self);
  void *__private;
} T;

typedef struct {
  //@required
  char *symbol;
  double qty;
  char *notional;
  //@required
  AlpacaOrder_Side side;
  //@required
  AlpacaOrder_Type type;
  //@required
  AlpacaOrder_Time_in_force time_in_force;
  double limit_price;
  double stop_price;
  double trail_price;
  char *trail_percent;
  bool extended_hours;
  char *client_order_id;
  AlpacaOrder_Class order_class;
  Hashmap *take_profit;
  Hashmap *stop_loss;
} AlpacaOrderArgs;

T *alpacaOrderEntity_constructor(AlpacaOrderArgs args);

#undef T
#endif
