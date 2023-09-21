#include "strategy_test.h"
#define EQUITY_FILE "equity.csv"

void strategy_test_report(Tsmetadata *m, time_t timestamp);

Order *strategy_test(Tsmetadata *metadata, Candle candle) {
  static bool first_run = true;

  if(candle.status == CANDLE_STOP){
    return NULL;
  }

  if (first_run) {
    first_run = false;
    csv_erase_content(EQUITY_FILE);
    csv_add_new_line(EQUITY_FILE, "equity,timestamp");
  }
  double close = candle.close;
  double open = candle.open;
  int market_position = (int)tsmetadata_get_market_position(metadata);

  Order *order = malloc(sizeof(Order));
  order->limit = 0;
  order->id = -1;
  order->price = close;
  order->status = ORDER_CANCELLED;
  sprintf(order->type, "%s", "Market");

  if (close > open && market_position >= 0) {
    order->side = SELL;
    order->status = ORDER_PENDING;
    if (market_position == 0) {
      order->size = 10000;
    } else {
      order->size = 20000;
    }
  }

  if (close < open && market_position <= 0) {
    order->side = BUY;
    order->status = ORDER_PENDING;
    if (market_position == 0) {
      order->size = 10000;
    } else {
      order->size = 20000;
    }
  }
  strategy_test_report(metadata, candle.timestamp);
  return order;
}

void strategy_test_report(Tsmetadata *m, time_t timestamp) {
  double equity = tsmetadata_get_equity(m);
  char line[50];
  sprintf(line, "%f,%ld", equity, timestamp);
  csv_add_new_line(EQUITY_FILE, line);
}
