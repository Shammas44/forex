#include "ConfigWrapper.h"
#include "json.h"
#include <stdbool.h>
#include <string.h>

#define T ConfigWrapper

typedef struct {
  Hashmap *map;
} Private;

static void __destructor(T*wrapper);
static Item __get(T*wrapper,char*key);

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

static Item __get(T*wrapper,char*key){
  Hashmap*map = wrapper->__private;
  return map->get(map,key);
}

static char* __mode(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return map->get(map,"mode").value;
}

static char* __symbol(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return map->get(map,"symbol").value;
}

static char* __strategy(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return map->get(map,"strategy").value;
}

static char* __backtest_data(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  return map->get(map,"backtest_data").value;
}

static bool __paper(T*wrapper){
  if(wrapper == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  char* value =map->get(map,"backtest_data").value;
  if(!value) return NULL;
  return strcmp(value,"")==0 ? true:false;
}

static double __spread(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  char* value =map->get(map,"spread").value;
  if(!value) return -1;
  return atof(value);
}

static double __capital(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  char* value =map->get(map,"capital").value;
  if(!value) return -1;
  return atof(value);
}

static int __leverage(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  char* value =map->get(map,"leverage").value;
  if(!value) return -1;
  return atoi(value);
}

static double __broker_commision_dpm(T*wrapper){
  if(wrapper == NULL) return -1;
  Hashmap*map = wrapper->__private;
  char* value =map->get(map,"broker_commision_dpm").value;
  if(!value) return -1;
  return atoi(value);
}

#undef T
