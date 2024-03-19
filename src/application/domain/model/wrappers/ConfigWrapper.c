#include "ConfigWrapper.h"
#include "Strategies.h"
#include "common.h"
#include <stdbool.h>
#include <string.h>

typedef enum {
  _Mode,
  _Symbol,
  _Strategy,
  _Backtest_data,
  _Spread,
  _Capital,
  _Leverage,
  _Paper,
  _Broker_commision_dpm,
} Keys;

static char *keys[]={
  "mode",
  "symbol",
  "strategy",
  "backtest_data",
  "spread",
  "capital",
  "leverage",
  "paper",
  "broker_commision_dpm",
};

#define KEY(index) \
({ \
  char* _key = keys[index]; \
  _key; \
})
#define T ConfigWrapper

typedef struct {
  Hashmap *map;
} Private;

typedef struct {char*name;int code;} N;

static void __destructor(T*self);
static Item __get(T*self,char*key);

static Metadata_mode __mode(T*self);
static char* __symbol(T*self);
static int __strategy(T*self);
static bool __paper(T*self);
static char* __backtest_data(T*self);
static double __spread(T*self);
static double __capital(T*self);
static int __leverage(T*self);
static double __broker_commision_dpm(T*self);

static int _$enum_iterator(char* comparator, N array[], size_t array_length);
static int _$to_strategy(char*strategy);
static Metadata_mode _$to_mode(char* mode);

T * configWrapper_constructor(Hashmap *map){
  T *self = malloc(sizeof(T));
  if (self == NULL) return NULL;
  self->__private = map;
  self->destructor = __destructor;
  self->get = __get;
  self->mode = __mode;
  self->symbol = __symbol;
  self->strategy = __strategy;
  self->backtest_data = __backtest_data;
  self->spread = __spread;
  self->capital = __capital;
  self->leverage = __leverage;
  self->paper = __paper;
  self->broker_commision_dpm = __broker_commision_dpm;
  return self;
}

static void __destructor(T*self){
  Hashmap*map = self->__private;
  map->destructor(map);
  free(self);
}

static Item __get(T*self,char*key){
  Hashmap*map = self->__private;
  return map->get(map,key);
}

static Metadata_mode __mode(T*self){
  if(self == NULL) return 0;
  Hashmap*map = self->__private;
  char* value = map->get(map,KEY(_Mode)).value;
  return _$to_mode(value);
}

static char* __symbol(T*self){
  if(self == NULL) return NULL;
  Hashmap*map = self->__private;
  return map->get(map,KEY(_Symbol)).value;
}

static int __strategy(T*self){
  if(self == NULL) return 0;
  Hashmap*map = self->__private;
  char* value = map->get(map,KEY(_Strategy)).value;
  return _$to_strategy(value);
}

static char* __backtest_data(T*self){
  if(self == NULL) return NULL;
  Hashmap*map = self->__private;
  return map->get(map,KEY(_Backtest_data)).value;
}

static bool __paper(T*self){
  if(self == NULL) return NULL;
  Hashmap*map = self->__private;
  bool* value =map->get(map,KEY(_Paper)).value;
  return *value;
}

static double __spread(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private;
  double* value =map->get(map,KEY(_Spread)).value;
  return *value;
}

static double __capital(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private;
  double* value =map->get(map,KEY(_Capital)).value;
  return *value;
}

static int __leverage(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private;
  double* value =map->get(map,KEY(_Leverage)).value;
  return *value;
}

static double __broker_commision_dpm(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private;
  double* value = map->get(map,KEY(_Broker_commision_dpm)).value;
  return *value;
}

static int _$enum_iterator(char* comparator, N array[], size_t array_length){
  for (int i = 0; i < array_length; i++) {
    if(strcmp(array[i].name, comparator) == 0){
      return array[i].code;
    }
  }
  return -1;
}

static int _$to_strategy(char*strategy){
  N strategies[] = {
    {.name = "NONE", .code = 0},
    {.name = "STRATEGY_TEST", .code = STRATEGY_TEST},
    {.name = "STRATEGY_TREND_FOLLOWING", .code = STRATEGY_TREND_FOLLOWING},
  };
  size_t length = sizeof(strategies) / sizeof(strategies[0]);
  Strategies result = _$enum_iterator(strategy,strategies,length);
  if(result > 0) return result;
  RUNTIME_ERROR("Error: strategy not found\n",1);
  exit(EXIT_FAILURE);
}

static Metadata_mode _$to_mode(char* mode){
  N modes[] = {
    {.name = "NONE", .code = 0},
    {.name = "BACKTEST", .code = Metadata_mode_backtest},
    {.name = "REAL", .code = Metadata_mode_real},
  };
  size_t length = sizeof(modes) / sizeof(modes[0]);
  Metadata_mode result = _$enum_iterator(mode,modes,length);
  if(result > 0) return result;
  RUNTIME_ERROR("Mode not found",1);
  exit(EXIT_FAILURE);
}

#undef T
