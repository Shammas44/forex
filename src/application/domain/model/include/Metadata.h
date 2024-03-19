#ifndef METADATA_H
#define METADATA_H
#include "ConfigWrapper.h"
#include "Order.h"
#include <pthread.h>
#include <stdio.h>
#define T Metadata

typedef struct T T;

typedef struct T {
  void (*destructor)(T *self);
  void (*set_equity)(T *self, double current_price);
  double (*get_equity)(T *self);
  void (*set_leverage)(T *self, int leverage);
  int (*get_leverage)(T *self);
  double (*get_available_funds)(T *self);
  void (*set_capital)(T *self, double capital);
  double (*get_capital)(T *self);
  double (*get_market_position)(T *self);
  void (*set_market_position)(T *self, int order_size);
  void (*set_number_of_trades)(T *self, int number);
  double (*get_number_of_trades)(T *self);
  void (*set_last_price)(T *self, double last_price);
  double (*get_last_price)(T *self);
  void (*set_spread)(T *self, double spread);
  double (*get_spread)(T *self);
  void (*set_broker_commision_dpm)(T *self, double dpm);
  double (*get_broker_commision_dpm)(T *self);
  void (*set_strategy)(T *self, int strategy);
  int (*get_strategy)(T *self);
  void (*open_position)(T*self,Order*order);
  void (*close_position)(T*self,char*order_id);
  void *__private;
} T;

T *metadata_constructor(ConfigWrapper *config);

#undef T
#endif
