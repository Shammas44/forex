#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H
#include "hashmap.h"
#include "wrapper.h"
#include <stdbool.h>

#define T ConfigWrapper

typedef struct T T;

typedef void(ConfigWrapper_destructor)(T *config);
typedef char* (ConfigWrapper_get)(T *config,char*key);

typedef char *(ConfigWrapper_mode)(T *config);
typedef char *(ConfigWrapper_symbol)(T *config);
typedef char *(ConfigWrapper_strategy)(T *config);
typedef char *(ConfigWrapper_backtest_data)(T *config);
typedef double(ConfigWrapper_spread)(T *config);
typedef bool (ConfigWrapper_paper)(T *config);
typedef double(ConfigWrapper_capital)(T *config);
typedef int(ConfigWrapper_leverage)(T *config);
typedef double(ConfigWrapper_broker_commision_dpm)(T *config);

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
#endif
