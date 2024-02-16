#include "CandleWrapper.h"
#include "common.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define T CandleWrapper

#define MAP(wrapper) \
({ \
  Hashmap*_map = wrapper->__private; \
  _map; \
})

#define KEY(index) \
({ \
  char* _key = keys[index]; \
  _key; \
})

#define GENERATE_ACCESSOR_FUNCTION(name, type) \
static type __##name(T* self) { \
  return *(type*)__get(self, KEY(_##name)); \
}

#define GENERATE_STRING_ACCESSOR_FUNCTION(name) \
static char* __##name(T* self) { \
  return __get(self, KEY(_##name)); \
}

typedef enum {
  _date,
  _time,
  _open,
  _high,
  _low,
  _close,
  _up_volume,
  _down_volume,
  _total_volume,
  _up_ticks,
  _down_ticks,
  _total_ticks,
  _timestamp,
  _volume,
} Keys;

static char *keys[]={
  "Date",
  "Time",
  "Open",
  "High",
  "Low",
  "Close",
  "UpVolume",
  "DownVolume",
  "TotalVolume",
  "UpTicks",
  "DownTicks",
  "TotalTicks",
  "Timestamp",
  "Volume",
};

typedef struct {
  Hashmap *map;
} Private;

static void __destructor(T *self);
static void*  __get(T *self,char*key);
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
static double  __total_ticks(T *self);
static time_t  __timestamp(T *self);
static double  __volume(T *self);

static void _$set_timestamp(T*self);
static void _$set_volume(T*self);

T * candleWrapper_constructor(Hashmap *map){
  T *self = malloc(sizeof(T));
  if (map == NULL) map = hashmap_constructor(20);
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
  self->timestamp = __timestamp;
  self->volume = __volume;
  _$set_timestamp(self);
  _$set_volume(self);
  return self;
}

static void __destructor(T*self){
  Hashmap*map = self->__private;
  map->destructor(map);
}

static void* __get(T*self,char*key){
  if(self == NULL) return NULL;
  Hashmap*map = self->__private;
  return map->get(map,key).value;
}

GENERATE_STRING_ACCESSOR_FUNCTION(date)
GENERATE_STRING_ACCESSOR_FUNCTION(time)
GENERATE_ACCESSOR_FUNCTION(open, double)
GENERATE_ACCESSOR_FUNCTION(high, double)
GENERATE_ACCESSOR_FUNCTION(low, double)
GENERATE_ACCESSOR_FUNCTION(close, double)
GENERATE_ACCESSOR_FUNCTION(up_volume, double)
GENERATE_ACCESSOR_FUNCTION(down_volume, double)
GENERATE_ACCESSOR_FUNCTION(total_volume, double)
GENERATE_ACCESSOR_FUNCTION(up_ticks, double)
GENERATE_ACCESSOR_FUNCTION(down_ticks, double)
GENERATE_ACCESSOR_FUNCTION(total_ticks, double)

static time_t __timestamp(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private; 
  time_t *time = map->get(map,KEY(_timestamp)).value;
  return *time;
}

static double __volume(T*self){
  if(self == NULL) return -1;
  Hashmap* map = self->__private; 
  double* volume = map->get(MAP(self),KEY(_volume)).value;
  return *volume;
}

static void _$set_timestamp(T*self){
  char*date =  __date(self);
  char*time =  __time(self);
  if(date==NULL || time == NULL) return;
  char datetime_str[20];
  snprintf(datetime_str, sizeof(datetime_str), "%s %s", date, time);

  struct tm tm;
  time_t epoch_time;

#ifdef __APPLE__
  if (strptime(datetime_str, "%m/%d/%Y %H:%M:%S", &tm) != NULL) {
    epoch_time = mktime(&tm);
    if (epoch_time == -1) {
      RUNTIME_ERROR("Error converting to time_t\n",1);
      return;
    }
  } else {
    RUNTIME_ERROR("Error parsing the date-time string\n",1);
    return;
  }
#else
  if (sscanf(datetime_str, "%d/%d/%d %d:%d:%d", &tm.tm_mon, &tm.tm_mday,
             &tm.tm_year, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
      tm.tm_year -= 1900; // Adjust year to be in years since 1900
      tm.tm_mon -= 1;     // Adjust month to be in [0, 11]
      tm.tm_isdst = -1;
      epoch_time = mktime(&tm);

      if (epoch_time == -1) {
        fprintf(stderr, "Error converting to time_t\n");
        return -1;
      }
    } else {
    fprintf(stderr, "Error parsing the date-time string\n");
    return -1;
  }

#endif
  time_t* out = malloc(sizeof(time_t));
  *out = epoch_time;
  Hashmap*map = self->__private; 
  Hashmap_Entry item ={.key=KEY(_timestamp),.type=Item_default,.value=out};
  map->push(map,item);
}

static void _$set_volume(T*self){
  Hashmap* map = self->__private; 
  char* up_volume =  map->get(map,KEY(_down_volume)).value;
  char* down_volume =  map->get(map,KEY(_up_volume)).value;
  double *volume = malloc(sizeof(double));
  *volume = atof(up_volume) + atof(down_volume);
  Hashmap_Entry item ={.key=KEY(_volume),.type=Item_double,.value=volume};
  map->push(map,item);
}

#undef T
#undef MAP
#undef KEY
#undef GENERATE_ACCESSOR_FUNCTION
#undef GENERATE_STRING_ACCESSOR_FUNCTION
