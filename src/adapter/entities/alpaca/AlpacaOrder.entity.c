#include "AlpacaOrder.entity.h"
#include "common.h"
#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#define T AlpacaOrderEntity
#define TYPE_LENGTH 5
#define SIDE_LENGTH 2
#define CLASS_LENGTH 4
#define TIME_IN_FORCE_LENGTH 6
#define MAP(self) \
({ \
  Private *_private = self->__private; \
  Hashmap *_map = _private->map; \
  _map; \
})

static AlpacaOrderItem type_items[TYPE_LENGTH] = {
    {.code = AlpacaOrder_Type_Market, .value = "market"},
    {.code = AlpacaOrder_Type_Limit, .value = "limit"},
    {.code = AlpacaOrder_Type_Stop, .value = "stop"},
    {.code = AlpacaOrder_Type_Stop_Limit, .value = "stop_limit"},
    {.code = AlpacaOrder_Type_Trailing_Stop, .value = "trailing_stop"},
};

static AlpacaOrderItem class_items[CLASS_LENGTH] = {
    {.code = AlpacaOrder_Class_Oco, .value = "oco"},
    {.code = AlpacaOrder_Class_Simple, .value = "simple"},
    {.code = AlpacaOrder_Class_Bracket, .value = "bracket"},
    {.code = AlpacaOrder_Class_Oto, .value = "oto"},
};

static AlpacaOrderItem time_in_force_items[TIME_IN_FORCE_LENGTH] = {
    {.code = AlpacaOrder_Time_in_force_Gtc, .value = "gtc"},
    {.code = AlpacaOrder_Time_in_force_Day, .value = "day"},
    {.code = AlpacaOrder_Time_in_force_Opg, .value = "opg"},
    {.code = AlpacaOrder_Time_in_force_Cls, .value = "cls"},
    {.code = AlpacaOrder_Time_in_force_Ioc, .value = "ioc"},
    {.code = AlpacaOrder_Time_in_force_Fok, .value = "fok"},
};

static AlpacaOrderItem side_items[SIDE_LENGTH] = {
    {.code = AlpacaOrder_Side_BUY, .value = "buy"},
    {.code = AlpacaOrder_Side_SELL, .value = "sell"},
};

typedef struct {
  Hashmap *map;
} Private;

static void __destructor(T *self);
static char *__symbol(T *self);
static double __qty(T *self);
static char *__notional(T *self);
static AlpacaOrderItem __side(T *self);
static AlpacaOrderItem __type(T *self);
static AlpacaOrderItem __time_in_force(T *self);
static double __limit_price(T *self);
static double __stop_price(T *self);
static double __trail_price(T *self);
static char *__trail_percent(T *self);
static bool __extended_hours(T *self);
static char *__client_order_id(T *self);
static AlpacaOrderItem __order_class(T *self);
static Hashmap *__take_profit(T *self);
static Hashmap *__stop_loss(T *self);

static char *_$get_type(AlpacaOrder_Type type);
static char *_$get_side(AlpacaOrder_Side side);
static Hashmap* _$set_items(AlpacaOrderItem items[],size_t array_length);
static int _$get(AlpacaOrderItem items[], size_t array_length, char *key);
static double _$get_double(T *self, char*key);
static AlpacaOrderItem _$get_item(T *self, char*key, AlpacaOrderItem items[], size_t array_length);
static char *_$get_string(T *self, char*key);
static bool _$get_bool(T *self, char*key);
static Hashmap* _$get_map(T *self, char*key);

static bool first = true;
static Hashmap *types;
static Hashmap *sides;
static Hashmap *time_in_forces;
static Hashmap *order_classes;

// =========================================================================="
// Public functions
// =========================================================================="

T *alpacaOrderEntity_constructor(AlpacaOrderArgs args) {
  if (first) {
    types = _$set_items(type_items,TYPE_LENGTH);
    sides = _$set_items(side_items,SIDE_LENGTH);
    time_in_forces = _$set_items(time_in_force_items,TIME_IN_FORCE_LENGTH);
    order_classes = _$set_items(class_items,CLASS_LENGTH);
    first = false;
  }

  T *self = malloc(sizeof(T));
  if (args.symbol == NULL) {
    RUNTIME_ERROR("symbol is required", 1);
    return NULL;
  }
  if (args.side == 0) {
    RUNTIME_ERROR("side is required", 1);
    return NULL;
  }
  if (args.type == 0) {
    RUNTIME_ERROR("type is required", 1);
    return NULL;
  }
  if (args.time_in_force == 0) {
    RUNTIME_ERROR("time_in_force is required", 1);
    return NULL;
  }
  Private *private = calloc(0, sizeof(Private));
  Hashmap *map = hashmap_constructor(10);
  char key[2];
  private->map = map;

  Hashmap_Entry symbol ={.key="symbol",.type = Item_string, .value = args.symbol};
  map->push(map, symbol );
  if(args.client_order_id != NULL){
    Hashmap_Entry client_order_id ={.key="client_order_id",.type = Item_string, .value = args.client_order_id};
    map->push(map, client_order_id );
  }
  if(args.notional != NULL){
    Hashmap_Entry notional ={.key="notional",.type = Item_string, .value = args.notional};
    map->push(map, notional );
  }
  if(args.trail_percent != NULL){
    Hashmap_Entry trail_percent ={.key="trail_percent",.type = Item_string, .value = args.trail_percent};
    map->push(map, trail_percent );
  }
  if(args.stop_loss != NULL){
    Hashmap_Entry stop_loss ={.key="stop_loss",.type = Item_string, .value = args.stop_loss};
    map->push(map, stop_loss );
  }
  if(args.take_profit != NULL){
    Hashmap_Entry take_profit ={.key="take_profit",.type = Item_string, .value = args.take_profit};
    map->push(map, take_profit );
  }

  if(args.qty != 0){
    double *qty = malloc(sizeof(double));
    *qty = args.qty;
    Hashmap_Entry q ={.key="qty",.type = Item_double, .value = qty};
    map->push(map, q );
  }

  if(args.limit_price != 0){
    double *limit_price = malloc(sizeof(double));
    *limit_price = args.limit_price;
    Hashmap_Entry limit ={.key="limit_price",.type = Item_double, .value = limit_price};
    map->push(map, limit );
  }

  if(args.stop_price != 0){
    double *stop_price = malloc(sizeof(double));
    *stop_price = args.stop_price;
    Hashmap_Entry stop ={.key="stop_price",.type = Item_double, .value = stop_price};
    map->push(map, stop );
  }

  if(args.trail_price != 0){
    double *trail_price = malloc(sizeof(double));
    *trail_price = args.trail_price;
    Hashmap_Entry trail ={.key="trail_price",.type = Item_double, .value = trail_price};
    map->push(map, trail );
  }

  // TODO: replace bool type by an enum
  bool * extended_hours = malloc(sizeof(bool));
  *extended_hours = args.extended_hours;
  Hashmap_Entry hours ={.key="extended_hours",.type = Item_bool, .value = extended_hours};
  map->push(map, hours );

  if(args.type != 0){
    sprintf(key, "%d", args.type);
    AlpacaOrderItem *type = (AlpacaOrderItem *)types->get(types, key).value;
    Hashmap_Entry item = {.key="type",.type = Item_string, .value = type->value};
    map->push(map, item);
  }

  if(args.side != 0){
    sprintf(key, "%d", args.side);
    AlpacaOrderItem *side = (AlpacaOrderItem *)sides->get(sides, key).value;
    Hashmap_Entry item ={.key="side",.type = Item_string, .value = side->value};
    map->push(map, item);
  }

  if(args.time_in_force != 0){
    sprintf(key, "%d", args.time_in_force);
    AlpacaOrderItem *time_in_force = (AlpacaOrderItem *)time_in_forces->get(time_in_forces, key).value;
    Hashmap_Entry item ={.key="time_in_force",.type = Item_string, .value = time_in_force->value};
    map->push(map, item);
  }

  if(args.order_class != 0){
    sprintf(key, "%d", args.order_class);
    AlpacaOrderItem *order_class = (AlpacaOrderItem *)order_classes->get(order_classes, key).value;
    Hashmap_Entry item ={.key="order_class",.type = Item_string, .value = order_class->value};
    map->push(map, item);
  }

  self->__private = private;
  self->destructor = __destructor;
  self->symbol = __symbol;
  self->type = __type;
  self->side = __side;
  self->qty = __qty;
  self->time_in_force = __time_in_force;
  self->limit_price = __limit_price;
  self->stop_price = __stop_price;
  self->trail_price = __trail_price;
  self->order_class = __order_class;
  self->client_order_id = __client_order_id;
  self->extended_hours = __extended_hours;
  self->notional = __notional;
  self->trail_percent = __trail_percent;
  self->stop_loss = __stop_loss;
  self->take_profit = __take_profit;
  return self;
}

// =========================================================================="
// Public Private
// =========================================================================="

static void __destructor(T *self) { free(self); }

static char *__symbol(T *self) {
  return _$get_string(self, "symbol");
}

static AlpacaOrderItem __time_in_force(T *self) {
  return _$get_item(self, "time_in_force", time_in_force_items, TIME_IN_FORCE_LENGTH);
}

static AlpacaOrderItem __side(T *self) {
  return _$get_item(self, "side", side_items, SIDE_LENGTH);
}

static double __qty(T *self) {
  return _$get_double(self,"qty");
}

static char *__notional(T *self){
  return _$get_string(self, "notional");
}

static AlpacaOrderItem __type(T *self) {
  return _$get_item(self, "type", type_items, TYPE_LENGTH);
}

static double __limit_price(T *self) { 
  return _$get_double(self,"limit_price");
}

static double __stop_price(T *self) { 
  return _$get_double(self,"stop_price");
}

static double __trail_price(T *self) {
  return _$get_double(self,"trail_price");
}

static char *__trail_percent(T *self){
  return _$get_string(self, "trail_percent");
}

static bool __extended_hours(T *self){
  return _$get_bool(self, "extended_hours");
}

static char *__client_order_id(T *self){
  return _$get_string(self, "client_order_id");
}

static AlpacaOrderItem __order_class(T *self) {
  return _$get_item(self, "order_class", class_items, CLASS_LENGTH);
}

static Hashmap *__take_profit(T *self){
  return _$get_map(self, "take_profit");
}

static Hashmap *__stop_loss(T *self){
  return _$get_map(self, "stop_loss");
}

// =========================================================================="
// Private Private
// =========================================================================="

static AlpacaOrderItem *_$create_item(char *value, int code) {
  AlpacaOrderItem *item = malloc(sizeof(AlpacaOrderItem));
  item->value = value;
  item->code = code;
  return item;
}

static char *_$set_key(int code) {
  char *value = calloc(0, sizeof(int));
  sprintf(value, "%d", code);
  return value;
}

static Hashmap* _$set_items(AlpacaOrderItem items[],size_t array_length){
  Hashmap *map = hashmap_constructor(array_length);
  for (int i = 0; i < array_length; i++) {
    Hashmap_Entry item = {
      .key=_$set_key(items[i].code),
      .type = Item_string,
      .value = _$create_item(items[i].value, items[i].code)
    };
    map->push(map, item);
  }
  return map;
}

static int _$get(AlpacaOrderItem items[], size_t array_length, char *key) {
  for (int i = 0; i < array_length; i++) {
    if (strcmp(items[i].value, key) == 0) {
      return items[i].code;
    }
  }
  return -1;
}

static double _$get_double(T *self, char*key){
  Hashmap *map = MAP(self);
  double *value = map->get(map,key ).value;
  return *value;
}

static AlpacaOrderItem _$get_item(T *self, char*key, AlpacaOrderItem items[], size_t array_length){
  Hashmap *map = MAP(self);
  char *value = map->get(map,key ).value;
  int code =_$get(items, array_length, value);
  return (AlpacaOrderItem){.value = value, .code = code};
}

static char *_$get_string(T *self, char*key) {
  Hashmap *map = MAP(self);
  return map->get(map,key ).value;
}

static bool _$get_bool(T *self, char*key) {
  Hashmap *map = MAP(self);
  bool* value = map->get(map,key ).value;
  return *value;
}

static Hashmap* _$get_map(T *self, char*key){
  Hashmap *map = MAP(self);
  return map->get(map,key ).value;
}

#undef TYPE_LENGTH
#undef SIDE_LENGTH
#undef CLASS_LENGTH
#undef TIME_IN_FORCE_LENGTH
#undef MAP
#undef T
