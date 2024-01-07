#include "Order.h"
#include "common.h"
#define T Order
#define MAX_STR_LENGTH 20

#define PRIVATE(self,error) \
({ \
  if(self == NULL){ \
    RUNTIME_ERROR("self is NULL",1); \
    return error; \
  } \
  Private *_private = self->__private; \
  if(_private == NULL){ \
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
  if(self == NULL){
    RUNTIME_ERROR("self is NULL",1);
    return NULL;
  }
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
  Private *private = malloc(sizeof(Private));
  Hashmap*map = hashmap_constructor(10);
  map->push(map,"id",(Item){.type=Item_default,.value="-1"});
  map->push(map,"type",(Item){.type=Item_default,.value="Market"});
  map->push(map,"status",(Item){.type=Item_default,.value="0"});
  map->push(map,"limit",(Item){.type=Item_default,.value="0"});
  map->push(map,"price",(Item){.type=Item_default,.value="0"});
  map->push(map,"side",(Item){.type=Item_default,.value="0"});
  map->push(map,"size",(Item){.type=Item_default,.value="0"});
  map->push(map,"executed_price",(Item){.type=Item_default,.value="0"});
  map->push(map,"ttl",(Item){.type=Item_default,.value="0"});
  private->map = map;
  self->__private = private;
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
    map->push(map,key,(Item){.type=Item_default,.value=new});
  }
  return map->get(map,key).value;
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
  Private *private = PRIVATE(self,0);
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

static void _$set_double(Hashmap* map, char* key, double value) {
    char* buffer = malloc(MAX_STR_LENGTH * sizeof(char));
    if (buffer != NULL) {
      snprintf(buffer, MAX_STR_LENGTH, "%f", value);
      map->push(map, key, (Item){.type=Item_default,.value=buffer});
    } else {
      RUNTIME_ERROR("memory allocation failed",1);
      // Handle memory allocation failure
    }
}

static void _$set_int(Hashmap* map, char* key, int value) {
    char* buffer = malloc(MAX_STR_LENGTH * sizeof(char)); 
    if (buffer != NULL) {
      snprintf(buffer, MAX_STR_LENGTH, "%d", value); // Safely write formatted string
      map->push(map, key, (Item){.type=Item_default,.value=buffer});
    } else {
      RUNTIME_ERROR("memory allocation failed",1);
      // Handle memory allocation failure
    }
}
#undef T
#undef PRIVATE
#undef MAX_STR_LENGTH
