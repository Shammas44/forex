#include "CandleWrapper.h"
#include "json.h"
#include <criterion/criterion.h>
#define T CandleWrapper

static JsonParser *parser;
static Hashmap *map;
static T *wrapper;

static char json1[] = "{"
  "\"Date\": \"1/27/2015\","
  "\"Time\": \"13:39:00\","
  "\"Open\": 1.11,"
  "\"High\": 1.12,"
  "\"Low\": 1.13,"
  "\"Close\": 1.14,"
  "\"UpVolume\": 1.15,"
  "\"DownVolume\": 1.16,"
  "\"TotalVolume\": 1.17,"
  "\"UpTicks\": 1.18,"
  "\"DownTicks\": 1.19,"
  "\"TotalTicks\": 2"
  "}";

static void setup(void) {
  parser = jsonParser_constructor();
  map = parser->parse(parser, json1);
  wrapper = candleWrapper_constructor(map);
}

static void teardown(void){
  wrapper->destructor(wrapper);
  parser->destructor(parser);
}

Test(T, valid_object, .init = setup, .fini = teardown) {
  char *value = wrapper->get(wrapper, "Date");
  cr_assert_eq(strcmp(value, "1/27/2015"), 0, "Wrong value");
}

Test(T, time, .init = setup, .fini = teardown) {
  char *value = wrapper->time(wrapper);
  cr_assert_eq(strcmp(value, "13:39:00"), 0, "Wrong value");
}

Test(T, open, .init = setup, .fini = teardown) {
  double value = wrapper->open(wrapper);
  cr_assert_eq(value, 1.11, "Wrong value");
}

Test(T, high, .init = setup, .fini = teardown) {
  double value = wrapper->high(wrapper);
  cr_assert_eq(value, 1.12, "Wrong value");
}

Test(T, low, .init = setup, .fini = teardown) {
  double value = wrapper->low(wrapper);
  cr_assert_eq(value, 1.13, "Wrong value");
}
Test(T, close, .init = setup, .fini = teardown) {
  double value = wrapper->close(wrapper);
  cr_assert_eq(value, 1.14, "Wrong value");
}

Test(T, UpVolume, .init = setup, .fini = teardown) {
  double value = wrapper->up_volume(wrapper);
  cr_assert_eq(value, 1.15, "Wrong value");
}

Test(T, DownVolume, .init = setup, .fini = teardown) {
  double value = wrapper->down_volume(wrapper);
  cr_assert_eq(value, 1.16, "Wrong value");
}

Test(T, TotalVolume, .init = setup, .fini = teardown) {
  double value = wrapper->total_volume(wrapper);
  cr_assert_eq(value, 1.17, "Wrong value");
}

Test(T, UpTicks, .init = setup, .fini = teardown) {
  double value = wrapper->up_ticks(wrapper);
  cr_assert_eq(value, 1.18, "Wrong value");
}

Test(T, DownTicks, .init = setup, .fini = teardown) {
  double value = wrapper->down_ticks(wrapper);
  cr_assert_eq(value, 1.19, "Wrong value");
}

Test(T, TotalTicks, .init = setup, .fini = teardown) {
  int value = wrapper->total_ticks(wrapper);
  cr_assert_eq(value, 2, "Wrong value");
}

#undef T
