#include "configWrapper.h"
#include "json.h"
#include <stdbool.h>
#include <string.h>
#define T ConfigWrapper
typedef struct {
  Hashmap *map;
} Private;

static char* __get_string(T*wrapper,const char*key);
static int __get_int(T*wrapper,const char*key);
static double __get_double(T*wrapper,const char*key);
static bool __get_bool(T*wrapper,const char*key);
static void __destructor(T*wrapper);

static char* __mode(T*wrapper);
static char* __get(T*wrapper,char*key);
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
  Hashmap_types*type = NULL;
  char*value = map->get(map,key,&type);
  return (void*)__get_string(wrapper,key);
}

static char* __get_string(T*wrapper,const char*key){
  if(key == NULL) return NULL;
  Hashmap*map = wrapper->__private;
  char* value = HASHMAP_GET_STRING(map, key);
  size_t len = strlen(value);
  char* out = malloc(sizeof(char) * len +1);
  strcpy(out,value);
  return out;
}

static int __get_int(T*wrapper,const char*key){
  if(key == NULL) return 0;
  Hashmap*map = wrapper->__private;
  char*value = HASHMAP_GET_STRING(map, key);
  int out = atoi(value);
  free(value);
  return out;
}

static bool __get_bool(T*wrapper,const char*key){
  if(key == NULL) return 0;
  Hashmap*map = wrapper->__private;
  char*value = HASHMAP_GET_STRING(map, key);
  bool out = NULL;
  if(strcmp(value,"true") == 0) out = true;
  if(strcmp(value,"false") == 0) out = false;
  free(value);
  return out;
}

static double __get_double(T*wrapper,const char*key){
  if(key == NULL) return 0;
  Hashmap*map = wrapper->__private;
  char*value = HASHMAP_GET_STRING(map, key);
  int out = atof(value);
  free(value);
  return out;
}

static char* __mode(T*wrapper){
  if(wrapper == NULL) return NULL;
  return __get_string(wrapper,"mode");
}

static char* __symbol(T*wrapper){
  if(wrapper == NULL) return NULL;
  return __get_string(wrapper,"symbol");
}

static char* __strategy(T*wrapper){
  if(wrapper == NULL) return NULL;
  return __get_string(wrapper,"strategy");
}

static char* __backtest_data(T*wrapper){
  if(wrapper == NULL) return NULL;
  return __get_string(wrapper,"backtest_data)");
}

static bool __paper(T*wrapper){
  if(wrapper == NULL) return NULL;
  return __get_bool(wrapper,"paper");
}

static double __spread(T*wrapper){
  if(wrapper == NULL) return -1;
  return __get_double(wrapper,"strategy");
}

static double __capital(T*wrapper){
  if(wrapper == NULL) return -1;
  return __get_double(wrapper,"capital");
}

static int __leverage(T*wrapper){
  if(wrapper == NULL) return -1;
  return __get_int(wrapper,"leverage");
}

static double __broker_commision_dpm(T*wrapper){
  if(wrapper == NULL) return -1;
  return __get_double(wrapper,"broker_commision_dpm");
}

#undef T
