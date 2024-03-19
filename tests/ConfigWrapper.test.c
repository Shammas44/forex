#include "ConfigWrapper.h"
#include "json.h"
#include <criterion/criterion.h>

static JsonParser *parser;
static ConfigWrapper *config;

char json1[] = "{"
  "\"mode\": \"BACKTEST\","
  "\"symbol\": \"EUR/USD\","
  "\"strategy\": \"STRATEGY_TREND_FOLLOWING\","
  "\"backtest_data\": \"./data/EURUSD_SHORT_1_MINUTE.csv\","
  "\"spread\": 0.0001,"
  "\"capital\": 100000,"
  "\"leverage\": 30,"
  "\"paper\": true,"
  "\"broker_commision_dpm\": 30,"
  "\"test\": 30"
  "}";

static void setup(void) {
  parser = jsonParser_constructor();
}

static void teardown(void){
  config->destructor(config);
}

Test(ConfigWrapper, valid_object, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  cr_assert_not_null(config, "ConfigWrapper is null");
}

Test(ConfigWrapper, get, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  double *value = config->get(config, "test").value;
  cr_assert_eq(*value, 30, "Wrong value");
}

Test(ConfigWrapper, mode, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  int value = config->mode(config);
  cr_assert_eq(value,1, "Wrong value");
}

Test(ConfigWrapper, symbol, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  char* value = config->symbol(config);
  cr_assert_eq(strcmp(value, "EUR/USD"),0, "Wrong value");
}

Test(ConfigWrapper, strategy, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  int value = config->strategy(config);
  cr_assert_eq(value,2, "Wrong value");
}

Test(ConfigWrapper, backtest_data, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  char*value = config->backtest_data(config);
  cr_assert_eq(strcmp(value, "./data/EURUSD_SHORT_1_MINUTE.csv"),0, "Wrong value");
}

Test(ConfigWrapper, spread, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  double value = config->spread(config);
  cr_assert_eq(value, 0.0001, "Wrong value");
}

Test(ConfigWrapper, capital, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  double value = config->capital(config);
  cr_assert_eq(value, 100000, "Wrong value");
}

Test(ConfigWrapper, leverage, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  int value = config->leverage(config);
  cr_assert_eq(value, 30, "Wrong value");
}

Test(ConfigWrapper, paper, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  bool value = config->paper(config);
  cr_assert_eq(value, true, "Wrong value");
}

Test(ConfigWrapper, broker_commision_dpm, .init = setup, .fini = teardown) {
  Hashmap *map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  double value = config->broker_commision_dpm(config);
  cr_assert_eq(value, 30, "Wrong value");
}
