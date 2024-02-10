#include "AlpacaOrder.entity.h"
#include "RuntimeErrorImp.h"
#include <criterion/criterion.h>
#define T AlpacaOrderEntity

AlpacaOrderArgs args;

static void setup(void) {
  runtimeerrors_mode = RuntimeErrors_mode_silent;

  double * value1 = malloc(sizeof(double));
  *value1 = 1.5;
  Hashmap *take_profit = hashmap_constructor(1);
  Hashmap_Entry take_profit_item ={.key="limit_price", .type = Item_double, .value = value1 };
  take_profit->push(take_profit, take_profit_item);

  double * value2 = malloc(sizeof(double));
  *value2 = 1.5;
  Hashmap *stop_loss = hashmap_constructor(1);
  Hashmap_Entry stop_loss_item ={.key="stop_price", .type = Item_double, .value = value2 };
  stop_loss->push(stop_loss, stop_loss_item);

  args = (AlpacaOrderArgs){
      .symbol = "AAPL",
      .qty = 1.5,
      .trail_price = 1.5,
      .limit_price = 1.5,
      .stop_price = 1.5,
      .client_order_id = "1234",
      .trail_percent = "1234",
      .notional = "1234",
      .extended_hours = false,
      .order_class = AlpacaOrder_Class_Simple,
      .side = AlpacaOrder_Side_BUY,
      .type = AlpacaOrder_Type_Market,
      .time_in_force = AlpacaOrder_Time_in_force_Gtc,
      .stop_loss = stop_loss,
      .take_profit = take_profit,
  };
}

static void teardown(void) {}

Test(alpacaOrderEntity_constructor, isValid_object, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_not_null(self, "should not be NULL");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, type_required, .init = setup, .fini = teardown) {
  args.type = 0;
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_null(self, "should be NULL");
}

Test(alpacaOrderEntity_constructor, symbol_required, .init = setup, .fini = teardown) {
  args.symbol = NULL;
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_null(self, "should be NULL");
}

Test(alpacaOrderEntity_constructor, side_required, .init = setup, .fini = teardown) {
  args.side = 0;
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_null(self, "should be NULL");
}

Test(alpacaOrderEntity_constructor, time_in_force_required, .init = setup, .fini = teardown) {
  args.time_in_force = 0;
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_null(self, "should be NULL");
}

Test(alpacaOrderEntity_constructor, symbol, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->symbol(self), "AAPL"), 0, "Wrong value");
}

Test(alpacaOrderEntity_constructor, type, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->type(self).value, "market"), 0, "Wrong value");
  cr_assert_eq(self->type(self).code, 1, "Wrong value");
}

Test(alpacaOrderEntity_constructor, side, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->side(self).value, "buy"), 0, "Wrong value");
  cr_assert_eq(self->side(self).code, 1, "Wrong value");
}

Test(alpacaOrderEntity_constructor, time_in_force, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->time_in_force(self).value, "gtc"), 0, "Wrong value");
  cr_assert_eq(self->time_in_force(self).code, 2, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, order_class, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->order_class(self).value, "simple"), 0, "Wrong value");
  cr_assert_eq(self->order_class(self).code, 1, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, quantity, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(self->qty(self), 1.5, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, limit_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(self->limit_price(self), 1.5, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, stop_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(self->stop_price(self), 1.5, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, trail_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(self->trail_price(self), 1.5, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, client_order_id, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->client_order_id(self),"1234"), 0, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, extended_hours, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(self->extended_hours(self),false, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, trail_percent, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->trail_percent(self),"1234"), 0, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, notional, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  cr_assert_eq(strcmp(self->notional(self),"1234"), 0, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, take_profit, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  Hashmap *take_profit = self->take_profit(self);
  cr_assert_not_null(take_profit, "should not be NULL");
  double *limit_price = take_profit->get(take_profit, "limit_price").value;
  cr_assert_eq(*limit_price, 1.5, "Wrong value");
  self->destructor(self);
}

Test(alpacaOrderEntity_constructor, stop_loss, .init = setup, .fini = teardown) {
  T *self = alpacaOrderEntity_constructor(args);
  Hashmap *stop_loss = self->stop_loss(self);
  cr_assert_not_null(stop_loss, "should not be NULL");
  double *stop_price = stop_loss->get(stop_loss, "stop_price").value;
  cr_assert_eq(*stop_price, 1.5, "Wrong value");
  self->destructor(self);
}

#undef T
