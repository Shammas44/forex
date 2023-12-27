#include "CandleWrapper.h"
#include "json.h"
#include <stdbool.h>
#include <string.h>

#define MAP(wrapper) \
({ \
  Hashmap*_map = wrapper->__private; \
  _map; \
})

#define T CandleWrapper
typedef struct {
  Hashmap *map;
} Private;

static void __destructor(T *self);
static char*  __get(T *self,char*key);
static char*  __date(T *self);
static char*  __time(T *self);
static double  __open(T *self);
static double  __high(T *self);
static double  __low(T *self);
static double  __close(T *self);
static double  __up_volume(T *self);
static double  __down_volume(T *self);
static double  __total_volume(T *self);
static double  __up_ticks(T *self);
static double  __down_ticks(T *self);
static int  __total_ticks(T *self);

T * configWrapper_constructor(Hashmap *map){
  T *self = malloc(sizeof(T));
  if (self == NULL) return NULL;
  self->__private = map;
  self->destructor = __destructor;
  self->get = __get;
  self->date = __date;
  self->time = __time;
  self->open = __open;
  self->high = __high;
  self->low = __low;
  self->close = __close;
  self->up_volume = __up_volume;
  self->down_volume = __down_volume;
  self->total_volume = __total_volume;
  self->up_ticks = __up_ticks;
  self->down_ticks = __down_ticks;
  self->total_ticks = __total_ticks;
  return self;
}

static void __destructor(T*self){
  Hashmap*map = self->__private;
  map->destructor(map);
}

static char* __get(T*wrapper,char*key){
  return (void*)wrapper_get_string(MAP(wrapper),key);
}
static char* __date(T*wrapper){
  if(wrapper == NULL) return NULL;
  return (void*)wrapper_get_string(MAP(wrapper),"date");
}

static char* __time(T*wrapper){
  if(wrapper == NULL) return NULL;
  return wrapper_get_string(MAP(wrapper),"time");
}

static double __open(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"open");
}

static double __high(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"high");
}

static double __low(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"low");
}

static double __close(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"close");
}

static double __up_volume(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"upVolume");
}

static double __down_volume(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"downVolume");
}

static double __total_volume(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"totalVolume");
}

static double __up_ticks(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"upTicks");
}

static double __down_ticks(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"downTicks");
}

static int __total_ticks(T*wrapper){
  if(wrapper == NULL) return -1;
  return wrapper_get_double(MAP(wrapper),"total_ticks");
}

#undef T
#undef MAP
