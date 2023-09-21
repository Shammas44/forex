#include "order.h"

void order_print(Order order) {
  printf("\nid: %d\n", order.id);
  printf("type: %s\n", order.type);
  printf("limit: %f\n", order.limit);
  printf("price: %f\n", order.price);
  char *side = order.side == BUY ? "BUY" : "SELL";
  printf("side: %s\n", side);
  printf("size: %f\n", order.size);
  char *status = order_get_status(order);
  printf("status: %s\n", status);
  printf("executed_price: %f\n", order.executed_price);
}

char *order_get_status(Order order) {
  switch (order.status) {
  // case ORDER_OPEN:
  //   return "OPEN";
  // case ORDER_FILLED:
  //   return "FILLED";
  case ORDER_CANCELLED:
    return "CANCELLED";
  // case ORDER_REJECTED:
  //   return "REJECTED";
  // case ORDER_EXPIRED:
  //   return "EXPIRED";
  case ORDER_PENDING:
    return "PENDING";
  case ORDER_EXECUTED:
    return "EXECUTED";
  case ORDER_STOP:
    return "STOP";
  default:
    return "UNKNOWN";
  }
}

char *order_to_json(Order *order) {
  if (!order)
    return NULL;
  size_t max_len = 200;
  char *json_string = (char *)malloc(max_len);
  if (!json_string)
    return NULL;
  snprintf(json_string, max_len,
           "{\"type\":\"%s\",\"status\":\"%s\",\"limit\":%f,\"price\":%f,"
           "\"side\":\"%d\","
           "\"size\":%f}",
           order->type, order_get_status(*order), order->limit, order->price, order->side,
           order->size);
  return json_string;
}

Order order_create(char *json, jsmntok_t **tokens, int token_num) {
  Order order;

  struct f {
    Convert_callback fun;
    char *label;
    void *prop;
  };

  struct f prop[ORDER_PROPS_NUM] = {
      {to_char, "type", &order.type},
      {to_char, "status", &order.status},
      {to_float, "limit", &order.limit},
      {to_float, "price", &order.price},
      {to_bool, "side", &order.side},
      {to_float, "size", &order.size},
      {to_float, "executed_price", &order.executed_price},
  };

  for (int i = 1; i < token_num; i++) {
    for (int j = 0; j < ORDER_PROPS_NUM; j++) {
      if (json_cmp_token_to_string(json, &((*tokens)[i]), prop[j].label) == 0) {
        json_extract_token(prop[j].label, *tokens, i, json, prop[j].prop,
                           prop[j].fun);
        i++;
      }
    }
  }

  return order;
}
