#include "ConfigWrapper.h"
#include "json.h"
#include <stdbool.h>
#include <string.h>

#define T ConfigWrapper

typedef struct {
  Hashmap *map;
} Private;

static void __destructor(T*wrapper);
static char* __get(T*wrapper,char*key);

static char* __mode(T*wrapper);
static char* __symbol(T*wrapper);
static char* __strategy(T*wrapper);
static bool __paper(T*wrapper);
static char* __backtest_data(T*wrapper);
static double __spread(T*wrapper);
static double __capital(T*wrapper);
static int __leverage(T*wrapper);
static double __broker_commision_dpm(T*wrapper);

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

static void __destructor(T*wrapper){
  Hashmap*map = wrapper->__private;
  map->destructor(map);
}

static char* __get(T*wrapper,char*key){
  Hashmap*map = wrapper->__private;
  return (void*)hashmap_get_string(map,key);
}

static char* __mode(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return hashmap_get_string(map,"mode");
}

static char* __symbol(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return hashmap_get_string(map,"symbol");
}

static char* __strategy(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return hashmap_get_string(map,"strategy");
}

static char* __backtest_data(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return hashmap_get_string(map,"backtest_data");
}

static bool __paper(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return hashmap_get_bool(map,"paper");
}

static double __spread(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  return hashmap_get_double(map,"strategy");
}

static double __capital(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  return hashmap_get_double(map,"capital");
}

static int __leverage(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  return hashmap_get_int(map,"leverage");
}

static double __broker_commision_dpm(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  return hashmap_get_double(map,"broker_commision_dpm");
}

#undef T
