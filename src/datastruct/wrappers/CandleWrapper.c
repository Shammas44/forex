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
static time_t  __timestamp(T *self);
static double  __volume(T *self);

static void __set_timestamp(T*self);
static void __set_volume(T*self);

T * candleWrapper_constructor(Hashmap *map){
  T *self = malloc(sizeof(T));
  if (self == NULL) map = hashmap_constructor(20);
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
  __set_timestamp(self);
  __set_volume(self);
  return self;
}

static void __destructor(T*self){
  Hashmap*map = self->__private;
  map->destructor(map);
}

static char* __get(T*self,char*key){
  return (void*)hashmap_get_string(MAP(self),key);
}
static char* __date(T*self){
  if(self == NULL) return NULL;
  return (void*)hashmap_get_string(MAP(self),"Date");
}

static char* __time(T*self){
  if(self == NULL) return NULL;
  return hashmap_get_string(MAP(self),"Time");
}

static double __open(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"Open");
}

static double __high(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"High");
}

static double __low(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"Low");
}

static double __close(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"Close");
}

static double __up_volume(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"UpVolume");
}

static double __down_volume(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"DownVolume");
}

static double __total_volume(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"TotalVolume");
}

static double __up_ticks(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"UpTicks");
}

static double __down_ticks(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"DownTicks");
}

static int __total_ticks(T*self){
  if(self == NULL) return -1;
  return hashmap_get_double(MAP(self),"Total_ticks");
}

static time_t __timestamp(T*self){
  if(self == NULL) return -1;
  Hashmap*map = self->__private; 
  time_t *time = map->get(map,"Timestamp",0);
  return *time;
}

static void __set_timestamp(T*self){
  char*date =  hashmap_get_string(MAP(self),"Date");
  char*time =  hashmap_get_string(MAP(self),"Time");
  if(date==NULL || time == NULL) return;
  char datetime_str[20];
  snprintf(datetime_str, sizeof(datetime_str), "%s %s", date, time);

  struct tm tm;
  time_t epoch_time;

#ifdef __APPLE__
  if (strptime(datetime_str, "%m/%d/%Y %H:%M:%S", &tm) != NULL) {
    epoch_time = mktime(&tm);
    if (epoch_time == -1) {
      fprintf(stderr, "Error converting to time_t\n");
      return;
    }
  } else {
    fprintf(stderr, "Error parsing the date-time string\n");
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
  map->push(map,"Timestamp",out,0);
}

static void __set_volume(T*self){
  Hashmap* map = self->__private; 
  double up_volume =  hashmap_get_double(map,"DownVolume");
  double down_volume =  hashmap_get_double(map,"UpVolume");
  double *volume = malloc(sizeof(double));
  *volume = up_volume + down_volume;
  map->push(map,"Volume",volume,0);
}

static double __volume(T*self){
  if(self == NULL) return -1;
  Hashmap* map = self->__private; 
  double* volume = map->get(MAP(self),"Volume",0);
  return *volume;
}

#undef T
#undef MAP
