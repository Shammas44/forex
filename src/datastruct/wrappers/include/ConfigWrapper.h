#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H
#include "hashmap.h"
#include "Wrapper.h"
#include <stdbool.h>

#define T ConfigWrapper

typedef struct T T;

typedef void(ConfigWrapper_destructor)(T *self);
typedef char* (ConfigWrapper_get)(T *self,char*key);

typedef char *(ConfigWrapper_mode)(T *self);
typedef char *(ConfigWrapper_symbol)(T *self);
typedef char *(ConfigWrapper_strategy)(T *self);
typedef char *(ConfigWrapper_backtest_data)(T *self);
typedef double(ConfigWrapper_spread)(T *self);
typedef bool (ConfigWrapper_paper)(T *self);
typedef double(ConfigWrapper_capital)(T *self);
typedef int(ConfigWrapper_leverage)(T *self);
typedef double(ConfigWrapper_broker_commision_dpm)(T *self);

typedef struct T {
  ConfigWrapper_destructor *destructor;
  ConfigWrapper_get *get;
  ConfigWrapper_mode *mode;
  ConfigWrapper_symbol *symbol;
  ConfigWrapper_strategy *strategy;
  ConfigWrapper_paper *paper;
  ConfigWrapper_backtest_data *backtest_data;
  ConfigWrapper_spread *spread;
  ConfigWrapper_capital *capital;
  ConfigWrapper_leverage *leverage;
  ConfigWrapper_broker_commision_dpm *broker_commision_dpm;
  void *__private;
} T;

T *configWrapper_constructor(Hashmap *map);

#undef T
#undef F
#endif
