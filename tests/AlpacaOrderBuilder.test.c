#include "AlpacaOrderBuilder.h"
#include "RuntimeErrorImp.h"
#include <criterion/criterion.h>
#define T AlpacaOrderBuilder

AlpacaOrderBuilderBuildArgs args;

static void setup(void) {
  runtimeerrors_mode = RuntimeErrors_mode_silent;
  args = (AlpacaOrderBuilderBuildArgs){
      .symbol = "AAPL",
      .side = AlpacaOrder_Side_BUY,
      .type = AlpacaOrder_Type_Market,
      .time_in_force = AlpacaOrder_Time_in_force_Gtc,
  };
}

static void teardown(void) {}

Test(alpacaOrderBuilder_constructor, isValid_object, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  cr_assert_not_null(self, "should not be NULL");
  self->destructor(self);
}

Test(alpacaOrderBuilder_constructor, basic_creation, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  AlpacaOrderEntity *entity = self->get(self);
  char* symbol = entity->symbol(entity);
  int side = entity->side(entity).code;
  int type = entity->type(entity).code;
  int time_in_force = entity->time_in_force(entity).code;
  cr_assert_eq(side, AlpacaOrder_Side_BUY, "should be equal");
  cr_assert_eq(type, AlpacaOrder_Type_Market, "should be equal");
  cr_assert_eq(time_in_force, AlpacaOrder_Time_in_force_Gtc, "should be equal");
  cr_assert_eq(strcmp(symbol, "AAPL"),0, "should not be NULL");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, qty, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->qty(self, 1.5);
  AlpacaOrderEntity *entity = self->get(self);
  double value = entity->qty(entity);
  cr_assert_eq(value, 1.5, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, notional, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->notional(self, "1234");
  AlpacaOrderEntity *entity = self->get(self);
  char *value = entity->notional(entity);
  cr_assert_eq(strcmp(value, "1234"), 0, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, limit_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->limit_price(self, 1.5);
  AlpacaOrderEntity *entity = self->get(self);
  double value = entity->limit_price(entity);
  cr_assert_eq(value, 1.5, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, stop_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->stop_price(self, 1.5);
  AlpacaOrderEntity *entity = self->get(self);
  double value = entity->stop_price(entity);
  cr_assert_eq(value, 1.5, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, trail_price, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->trail_price(self, 1.5);
  AlpacaOrderEntity *entity = self->get(self);
  double value = entity->trail_price(entity);
  cr_assert_eq(value, 1.5, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, trail_percent, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->trail_percent(self, "1234");
  AlpacaOrderEntity *entity = self->get(self);
  char *value = entity->trail_percent(entity);
  cr_assert_eq(strcmp(value, "1234"), 0, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, extended_hours, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->extended_hours(self, true);
  AlpacaOrderEntity *entity = self->get(self);
  bool value = entity->extended_hours(entity);
  cr_assert_eq(value, true, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, client_order_id, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->client_order_id(self, "1234");
  AlpacaOrderEntity *entity = self->get(self);
  char* value = entity->client_order_id(entity);
  cr_assert_eq(strcmp(value, "1234"),0, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, order_class, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  self->build(self, args);
  self->order_class(self, AlpacaOrder_Class_Simple);
  AlpacaOrderEntity *entity = self->get(self);
  int value = entity->order_class(entity).code;
  cr_assert_eq(value, AlpacaOrder_Class_Simple, "should be equal");
  self->destructor(self);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, take_profit, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  Hashmap *take_profit = hashmap_constructor(1);
  self->build(self, args);
  self->take_profit(self, take_profit);
  AlpacaOrderEntity *entity = self->get(self);
  Hashmap* value = entity->take_profit(entity);
  cr_assert_not_null(value, "should be equal");
  self->destructor(self);
  take_profit->destructor(take_profit);
  entity->destructor(entity);
}

Test(alpacaOrderBuilder_constructor, stop_loss, .init = setup, .fini = teardown) {
  T *self = alpacaOrderBuilder_constructor();
  Hashmap *stop_loss = hashmap_constructor(1);
  self->build(self, args);
  self->stop_loss(self, stop_loss);
  AlpacaOrderEntity *entity = self->get(self);
  Hashmap* value = entity->stop_loss(entity);
  cr_assert_not_null(value, "should be equal");
  self->destructor(self);
  stop_loss->destructor(stop_loss);
  entity->destructor(entity);
}

#undef T
