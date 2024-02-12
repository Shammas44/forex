#include "ConfigWrapper.h"
#include "Metadata.h"
#include "Strategies.h"
#include "json.h"
#include <criterion/criterion.h>
#define T Metadata

static JsonParser *parser;
static ConfigWrapper *config;
static T *metadata;
static Hashmap *map;

static char json1[] = "{"
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
  map = parser->parse(parser, json1);
  config = configWrapper_constructor(map);
  metadata = metadata_constructor(config);
}

static void teardown(void){
  config->destructor(config);
  parser->destructor(parser);
  metadata->destructor(metadata);
}

Test(T, get, .init = setup, .fini = teardown) {
  int leverage = metadata->get_leverage(metadata);
  cr_assert_eq(leverage, 30, "Wrong value");
  metadata->set_leverage(metadata, 2);
  leverage = metadata->get_leverage(metadata);
  cr_assert_eq(leverage, 2, "Wrong value");
}

Test(T, capital, .init = setup, .fini = teardown) {
  double capital = metadata->get_capital(metadata);
  cr_assert_eq(capital, 100000, "Wrong value");
  metadata->set_capital(metadata, 2);
  capital = metadata->get_capital(metadata);
  cr_assert_eq(capital, 2, "Wrong value");
}

Test(T, spread, .init = setup, .fini = teardown) {
  double spread = metadata->get_spread(metadata);
  cr_assert_eq(spread, 0.0001, "Wrong value");
  metadata->set_spread(metadata, 2);
  spread = metadata->get_spread(metadata);
  cr_assert_eq(spread, 2, "Wrong value");
}

Test(T, market_position, .init = setup, .fini = teardown) {
  double market_position = metadata->get_market_position(metadata);
  cr_assert_eq(market_position, 0, "Wrong value");
  metadata->set_market_position(metadata, 2);
  market_position = metadata->get_market_position(metadata);
  cr_assert_eq(market_position, 2, "Wrong value");
}

Test(T, last_price, .init = setup, .fini = teardown) {
  double last_price = metadata->get_last_price(metadata);
  cr_assert_eq(last_price, 0, "Wrong value");
  metadata->set_last_price(metadata, 2);
  last_price = metadata->get_last_price(metadata);
  cr_assert_eq(last_price, 2, "Wrong value");
}

Test(T, number_of_trades, .init = setup, .fini = teardown) {
  int number_of_trades = metadata->get_number_of_trades(metadata);
  cr_assert_eq(number_of_trades, 0, "Wrong value");
  metadata->set_number_of_trades(metadata, 2);
  number_of_trades = metadata->get_number_of_trades(metadata);
  cr_assert_eq(number_of_trades, 2, "Wrong value");
}

Test(T, broker_commision_dpm, .init = setup, .fini = teardown) {
  double broker_commision_dpm = metadata->get_broker_commision_dpm(metadata);
  cr_assert_eq(broker_commision_dpm, 30, "Wrong value");
  metadata->set_broker_commision_dpm(metadata, 2);
  broker_commision_dpm = metadata->get_broker_commision_dpm(metadata);
  cr_assert_eq(broker_commision_dpm, 2, "Wrong value");
}

Test(T, strategy, .init = setup, .fini = teardown) {
  int strategy = metadata->get_strategy(metadata);
  cr_assert_eq(strategy, STRATEGY_TREND_FOLLOWING, "Wrong value");
  metadata->set_strategy(metadata, STRATEGY_TEST);
  strategy = metadata->get_strategy(metadata);
  cr_assert_eq(strategy, STRATEGY_TEST, "Wrong value");
}

Test(T, equity, .init = setup, .fini = teardown) {
  double last_price = 2.5;
  double current_price = 3.5;
  double market_position = 1;
  double expected_res = (current_price - last_price) * market_position;
  double equity = metadata->get_equity(metadata);
  cr_assert_eq(equity, 0, "Wrong value");
  metadata->set_last_price(metadata, last_price);
  metadata->set_market_position(metadata, market_position);
  metadata->set_equity(metadata, current_price);
  equity = metadata->get_equity(metadata);
  cr_assert_eq(equity, expected_res, "Wrong value");
}

