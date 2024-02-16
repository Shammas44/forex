#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H
#include "Hashmap.h"
#include <stdbool.h>

#define T ConfigWrapper

typedef struct T T;

typedef enum {
  Metadata_mode_backtest,
  Metadata_mode_real,
} Metadata_mode;

typedef struct T {
  void (*destructor)(T *self);
  Item (*get)(T *self, char *key);
  Metadata_mode (*mode)(T *self);
  char *(*symbol)(T *self);
  int (*strategy)(T *self);
  char *(*backtest_data)(T *self);
  double (*spread)(T *self);
  bool (*paper)(T *self);
  double (*capital)(T *self);
  int (*leverage)(T *self);
  double (*broker_commision_dpm)(T *self);
  void *__private;
} T;

T *configWrapper_constructor(Hashmap *map);

#undef T
#undef F
#endif
