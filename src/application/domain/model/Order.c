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
static int __id(T *self, Access_mode mode, int new);
static char*  __type(T *self, Access_mode mode, char* new);
static OrderStatus __status(T *self, Access_mode mode, OrderStatus new);
static double __limit(T *self, Access_mode mode, double new);
static double __price(T *self, Access_mode mode, double new);
static Side __side(T *self, Access_mode mode, Side new);
static double __size(T *self, Access_mode mode, double new);
static double __executed_price(T *self, Access_mode mode, double new);
static int __ttl(T *self, Access_mode mode, int new);

static void _$set_double(Hashmap*map, char* key, double value);
static void _$set_int(Hashmap*map, char* key, int value);
static char* _$get(T*self,char*key);
static double _$get_double(T*self,char*key);
static int _$get_int(T*self,char*key);

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
  Hashmap_Entry identifiant = {.key="id",.type=Item_string,.value=strdup("-1")};
  map->push(map,identifiant);
  Hashmap_Entry type = {.key="type",.type=Item_string,.value=strdup("Market")};
  map->push(map,type);
  Hashmap_Entry status = {.key="status",.type=Item_string,.value=strdup("0")};
  map->push(map,status);
  Hashmap_Entry limit = {.key="limit",.type=Item_string,.value=strdup("0")};
  map->push(map,limit);
  Hashmap_Entry price = {.key="price",.type=Item_string,.value=strdup("0")};
  map->push(map,price);
  Hashmap_Entry side = {.key="side",.type=Item_string,.value=strdup("0")};
  map->push(map,side);
  Hashmap_Entry size = {.key="size",.type=Item_string,.value=strdup("0")};
  map->push(map,size);
  Hashmap_Entry executed_price = {.key="executed_price",.type=Item_string,.value=strdup("0")};
  map->push(map,executed_price);
  Hashmap_Entry ttl = {.key="ttl",.type=Item_string,.value=strdup("0")};
  map->push(map,ttl);
  private->map = map;
  self->__private = private;
  return self;
}

static void __destructor(T *self){
  static int count = -1;
  count++;
  printf("destructor count: %d\n",count);
  if(self == NULL){
    RUNTIME_ERROR("self is NULL",1);
    return;
  }
  Private *private = self->__private;
  if(private == NULL){
    RUNTIME_ERROR("private is NULL",1);
    free(self);
    return;
  }
  Hashmap *map = private->map;
  free(private);
  if(map == NULL){
    RUNTIME_ERROR("map is NULL",1);
    free(self);
    return;
  }
  map->destructor(map);
  free(self);
}

static int __id(T *self, Access_mode mode, int new){
  char *key = "id";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return _$get_int(self,key);
}

static char*  __type(T *self, Access_mode mode, char* new){
  char *key = "type";
  Private *private = PRIVATE(self,NULL);
  Hashmap *map = private->map;
  if(mode == WRITE){
    Hashmap_Entry item ={.key=key,.type=Item_string,.value=new};
    map->push(map,item);
  }
  return _$get(self,key);
}

static OrderStatus __status(T *self, Access_mode mode, OrderStatus new){
  char *key = "status";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return _$get_int(self,key);
}

static double __limit(T *self, Access_mode mode, double new){
  char *key = "limit";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return _$get_double(self,key);
}

static double __price(T *self, Access_mode mode, double new){
  char *key = "price";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return _$get_double(self,key);
}

static Side __side(T *self, Access_mode mode, Side new){
  char *key = "side";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return _$get_int(self,key);
}

static double __size(T *self, Access_mode mode, double new){
  char *key = "size";
  Private *private = PRIVATE(self,0);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return _$get_double(self,key);
}

static double __executed_price(T *self, Access_mode mode, double new){
  char *key = "executed_price";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_double(map,key,new);
  }
  return _$get_double(self,key);
}

static int __ttl(T *self, Access_mode mode, int new){
  char *key = "ttl";
  Private *private = PRIVATE(self,-1);
  Hashmap *map = private->map;
  if(mode == WRITE){
    _$set_int(map,key,new);
  }
  return _$get_int(self,key);
}

static void _$set_double(Hashmap* map, char* key, double value) {
    char* buffer = malloc(MAX_STR_LENGTH * sizeof(char));
    if (buffer != NULL) {
      snprintf(buffer, MAX_STR_LENGTH, "%f", value);
      Hashmap_Entry item ={.key=key,.type=Item_string,.value=buffer};
      map->push(map, item);
    } else {
      RUNTIME_ERROR("memory allocation failed",1);
      // Handle memory allocation failure
    }
}

static void _$set_int(Hashmap* map, char* key, int value) {
    char* buffer = malloc(MAX_STR_LENGTH * sizeof(char)); 
    if (buffer != NULL) {
      snprintf(buffer, MAX_STR_LENGTH, "%d", value); // Safely write formatted string
      Hashmap_Entry item = {.key=key,.type=Item_string,.value=buffer};
      map->push(map, item);
    } else {
      RUNTIME_ERROR("memory allocation failed",1);
      // Handle memory allocation failure
    }
}

static char* _$get(T*self,char*key){
  Private *private = PRIVATE(self,NULL);
  Hashmap*map = private->map;
  return map->get(map,key).value;
}

static double _$get_double(T*self,char*key){
  char* value = _$get(self,key);
  if(!value) return -1;
  return atof(value);
}

static int _$get_int(T*self,char*key){
  char* value = _$get(self,key);
  if(!value) return -1;
  return atoi(value);
}
#undef T
#undef PRIVATE
#undef MAX_STR_LENGTH
