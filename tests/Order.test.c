#include "Order.h"
#include <criterion/criterion.h>
#define T Order

static T *order;

char *json_default = "{\"executed_price\":\"0\",\"id\":\"1\",\"limit\":\"0\","
                     "\"price\":\"0\",\"side\":\"0\",\"size\":\"0\",\"status\":"
                     "\"0\",\"ttl\":\"0\",\"type\":\"Market\"}";

static void setup(void) { order = order_constructor(1); }

static void teardown(void) { order->destructor(order); }

Test(T, constructor, .init = setup, .fini = teardown) {
  char *json = order->to_json(order);
  cr_expect_not_null(order);
  cr_expect_str_eq(json, json_default, "Should be equal");
}

Test(T, id, .init = setup, .fini = teardown) {
  int value = order->id(order, READ, 0);
  cr_expect_eq(value, 1, "Should be equal");
  value = order->id(order, WRITE, 5);
  cr_expect_eq(value, 5, "Should be equal");
  value = order->id(order, READ, 5);
  cr_expect_eq(value, 5, "Should be equal");
}

Test(T, limit, .init = setup, .fini = teardown) {
  double value = order->limit(order, READ, 0);
  cr_expect_eq(value, 0, "Should be equal");
  value = order->limit(order, WRITE, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
  value = order->limit(order, READ, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
}

Test(T, price, .init = setup, .fini = teardown) {
  double value = order->price(order, READ, 0);
  cr_expect_eq(value, 0, "Should be equal");
  value = order->price(order, WRITE, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
  value = order->price(order, READ, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
}

Test(T, size, .init = setup, .fini = teardown) {
  double value = order->size(order, READ, 0);
  cr_expect_eq(value, 0, "Should be equal");
  value = order->size(order, WRITE, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
  value = order->size(order, READ, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
}

Test(T, executed_price, .init = setup, .fini = teardown) {
  double value = order->executed_price(order, READ, 0);
  cr_expect_eq(value, 0, "Should be equal");
  value = order->executed_price(order, WRITE, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
  value = order->executed_price(order, READ, 5.5);
  cr_expect_eq(value, 5.5, "Should be equal");
}

Test(T, ttl, .init = setup, .fini = teardown) {
  int value = order->ttl(order, READ, 0);
  cr_expect_eq(value, 0, "Should be equal");
  value = order->ttl(order, WRITE, 5);
  cr_expect_eq(value, 5, "Should be equal");
  value = order->ttl(order, READ, 5);
  cr_expect_eq(value, 5, "Should be equal");
}

Test(T, type, .init = setup, .fini = teardown) {
  char* value = order->type(order, READ, 0);
  cr_expect_eq(strcmp(value,"Market"), 0, "Should be equal");
  value = order->type(order, WRITE, strdup("Limit"));
  cr_expect_eq(strcmp(value,"Limit"), 0, "Should be equal");
  value = order->type(order, READ, strdup("Limit"));
  cr_expect_eq(strcmp(value,"Limit"), 0, "Should be equal");
}

Test(T, status, .init = setup, .fini = teardown) {
  OrderStatus value = order->status(order, READ, 0);
  cr_expect_eq(value, ORDER_CANCELLED, "Should be equal");
  value = order->status(order, WRITE, ORDER_STOP);
  cr_expect_eq(value, ORDER_STOP, "Should be equal");
  value = order->status(order, READ, ORDER_STOP);
  cr_expect_eq(value, ORDER_STOP, "Should be equal");
}

Test(T, side, .init = setup, .fini = teardown) {
  Side value = order->side(order, READ, 0);
  cr_expect_eq(value, BUY, "Should be equal");
  value = order->side(order, WRITE, SELL);
  cr_expect_eq(value, SELL, "Should be equal");
  value = order->side(order, READ, SELL);
  cr_expect_eq(value, SELL, "Should be equal");
}

#undef T
