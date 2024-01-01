#include "Order.h"
#include "common.h"
#define T Order

#define PRIVATE(self) \
({ \
Private *_private; \
  if(!self){ \
    RUNTIME_ERROR("self is NULL",1); \
    _private = NULL; \
  } else { \
    _private = self->__private; \
  } \
  _private; \
})

typedef struct {
  int id;
  char* type;
  OrderStatus status;
  double limit;
  double price;
  Side side;
  double size;
  double executed_price;
  int ttl;
} Private;

static void __destructor(T *self);
static int __id(T *self);
static char* __type(T *self, Acces_mode mode, char* new);
static OrderStatus __status(T *self, Acces_mode mode, OrderStatus new);
static double __limit(T *self, Acces_mode mode, double new);
static double __price(T *self, Acces_mode mode, double new);
static Side __side(T *self, Acces_mode mode, Side new);
static double __size(T *self, Acces_mode mode, double new);
static double __executed_price(T *self, Acces_mode mode, double new);
static double __ttl(T *self, Acces_mode mode, int new);

T* order_constructor(int id){
  T* self = malloc(sizeof(T));
  Private *private = malloc(sizeof(Private));
  memset(private, 0, sizeof(Private));
  private->id = id;
  private->status = ORDER_PENDING;
  self->__private = private;
  self->destructor = __destructor;
  self->id = __id;
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
  Private *private = self->__private;
  if(private == NULL) {
    RUNTIME_ERROR("private is NULL",1);
    return;
  }
  free(private->type);
  free(private);
  free(self);
}

static int __id(T *self){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  return private->id;
}

static char* __type(T *self, Acces_mode mode, char* new){
  Private *private = PRIVATE(self);
  if(private == NULL) return NULL;
  if(mode == WRITE){
    strcpy(private->type, new);
  }
  return private->type;
}

static OrderStatus __status(T *self, Acces_mode mode, OrderStatus new){
  Private *private = PRIVATE(self);
  if(private == NULL) return NULL;
  if(mode == WRITE){
    private->status = new;
  }
  return private->status;
}

static double __limit(T *self, Acces_mode mode, double new){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  if(mode == WRITE){
    private->limit = new;
  }
  return private->limit;
}

static double __price(T *self, Acces_mode mode, double new){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  if(mode == WRITE){
    private->price = new;
  }
  return private->price;
}

static Side __side(T *self, Acces_mode mode, Side new){
  Private *private = PRIVATE(self);
  if(private == NULL) return NULL;
  if(mode == WRITE){
    private->side = new;
  }
  return private->side;
}

static double __size(T *self, Acces_mode mode, double new){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  if(mode == WRITE){
    private->size = new;
  }
  return private->size;
}

static double __executed_price(T *self, Acces_mode mode, double new){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  if(mode == WRITE){
    private->executed_price = new;
  }
  return private->executed_price;
}

static double __ttl(T *self, Acces_mode mode, int new){
  Private *private = PRIVATE(self);
  if(private == NULL) return -1;
  if(mode == WRITE){
    private->ttl = new;
  }
  return private->ttl;
}

#undef T
#undef PRIVATE
// void order_print(Order order) {
//   printf("\nid: %d\n", order.id);
//   printf("type: %s\n", order.type);
//   printf("limit: %f\n", order.limit);
//   printf("price: %f\n", order.price);
//   char *side = order.side == BUY ? "BUY" : "SELL";
//   printf("side: %s\n", side);
//   printf("size: %f\n", order.size);
//   char *status = order_get_status(order);
//   printf("status: %s\n", status);
//   printf("executed_price: %f\n", order.executed_price);
// }

// char *order_get_status(Order order) {
//   switch (order.status) {
//   // case ORDER_OPEN:
//   //   return "OPEN";
//   // case ORDER_FILLED:
//   //   return "FILLED";
//   case ORDER_CANCELLED:
//     return "CANCELLED";
//   // case ORDER_REJECTED:
//   //   return "REJECTED";
//   // case ORDER_EXPIRED:
//   //   return "EXPIRED";
//   case ORDER_PENDING:
//     return "PENDING";
//   case ORDER_EXECUTED:
//     return "EXECUTED";
//   case ORDER_STOP:
//     return "STOP";
//   default:
//     return "UNKNOWN";
//   }
// }

// char *order_to_json(Order *order) {
//   if (!order)
//     return NULL;
//   size_t max_len = 200;
//   char *json_string = (char *)malloc(max_len);
//   if (!json_string)
//     return NULL;
//   snprintf(json_string, max_len,
//            "{\"type\":\"%s\",\"status\":\"%s\",\"limit\":%f,\"price\":%f,"
//            "\"side\":\"%d\","
//            "\"size\":%f}",
//            order->type, order_get_status(*order), order->limit, order->price, order->side,
//            order->size);
//   return json_string;
// }

// Order order_create(char *json, jsmntok_t **tokens, int token_num) {
//   Order order;

//   struct f {
//     Convert_callback fun;
//     char *label;
//     void *prop;
//   };

//   struct f prop[ORDER_PROPS_NUM] = {
//       {to_char, "type", &order.type},
//       {to_char, "status", &order.status},
//       {to_float, "limit", &order.limit},
//       {to_float, "price", &order.price},
//       {to_bool, "side", &order.side},
//       {to_float, "size", &order.size},
//       {to_float, "executed_price", &order.executed_price},
//   };

//   for (int i = 1; i < token_num; i++) {
//     for (int j = 0; j < ORDER_PROPS_NUM; j++) {
//       if (json_cmp_token_to_string(json, &((*tokens)[i]), prop[j].label) == 0) {
//         json_extract_token(*tokens, i, json, prop[j].prop,
//                            prop[j].fun);
//         i++;
//       }
//     }
//   }

//   return order;
// }
