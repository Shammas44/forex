#include "Order.h"
#include "common.h"
#include "hashmap.h"
#define T Order

#define PRIVATE(self,error) \
({ \
  if(self == NULL){ \
    RUNTIME_ERROR("self is NULL",1); \
    return error; \
  } \
  Private *_private = self->__private; \
  if(private == NULL){ \
    RUNTIME_ERROR("private is NULL",1); \
    return error; \
  } \
  _private; \
})

static void __destructor(T *self);
static int __id(T *self, Acces_mode mode, int new);
static char*  __type(T *self, Acces_mode mode, char* new);
static OrderStatus __status(T *self, Acces_mode mode, OrderStatus new);
static double __limit(T *self, Acces_mode mode, double new);
static double __price(T *self, Acces_mode mode, double new);
static Side __side(T *self, Acces_mode mode, Side new);
static double __size(T *self, Acces_mode mode, double new);
static double __executed_price(T *self, Acces_mode mode, double new);
static int __ttl(T *self, Acces_mode mode, int new);

static void _$set_double(Hashmap*map, char* key, double value);
static void _$set_int(Hashmap*map, char* key, int value);

typedef struct {
  Hashmap*map;
} Private;

T* order_constructor(int id){
  T* self = (T*) malloc(sizeof(T));
  self->destructor = __destructor;
  self->id = __id;
  self->type = __type;
  self->status = __status;
  self->limit = __limit;
  self->price = __price;
  self->side = __side;
  self->size = __size;
  self->executed_price = __executed_price;
  self->ttl = __ttl;
  return self;
}

static void __destructor(T *self){
  if(self == NULL){
    RUNTIME_ERROR("self is NULL",1);
    return;
  }
  Private *private = self->__private;
  if(private != NULL){
    RUNTIME_ERROR("private is NULL",1);
    free(self);
    return;
  }
  Hashmap *map = private->map;
  free(private);
  if(map != NULL){
    RUNTIME_ERROR("map is NULL",1);
    free(self);
    return;
  }
  map->destructor(map);
  free(self);
}

static int __id(T *self, Acces_mode mode, int new){
  char *key = "id";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return hashmap_get_int(map,key);
}

static char*  __type(T *self, Acces_mode mode, char* new){
  char *key = "type";
  Private *private = PRIVATE(self,NULL);
  Hashmap *map = private->map;
  if(mode == WRITE){
    map->push(map,key,new,0);
  }
  return map->get(map,key,0);
}

static OrderStatus __status(T *self, Acces_mode mode, OrderStatus new){
  char *key = "status";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return hashmap_get_int(map,key);
}

static double __limit(T *self, Acces_mode mode, double new){
  char *key = "limit";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return hashmap_get_double(map,key);
}

static double __price(T *self, Acces_mode mode, double new){
  char *key = "price";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return hashmap_get_double(map,key);
}

static Side __side(T *self, Acces_mode mode, Side new){
  char *key = "side";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return hashmap_get_int(map,key);
}

static double __size(T *self, Acces_mode mode, double new){
  char *key = "size";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return hashmap_get_double(map,key);
}

static double __executed_price(T *self, Acces_mode mode, double new){
  char *key = "executed_price";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return hashmap_get_double(map,key);
}

static int __ttl(T *self, Acces_mode mode, int new){
  char *key = "ttl";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return hashmap_get_int(map,key);
}

static void _$set_double(Hashmap*map, char* key, double value){
    int length = sizeof(int);
    char*buffer = malloc(length);
    sprintf(buffer,"%f",value);
    map->push(map,key,buffer,0);
}

static void _$set_int(Hashmap*map, char* key, int value){
    int length = sizeof(int);
    char*buffer = malloc(length);
    sprintf(buffer,"%d",value);
    map->push(map,key,buffer,0);
}
#undef T
