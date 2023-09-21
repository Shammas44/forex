#ifndef STRATEGY_H
#define STRATEGY_H
#include "mtqueue.h"
#include "candle.h"
#include "order.h"
#include "tsmetadata.h"
#include "http.h"
#include "order.h"
#include "csv.h"
#include "synchronizer.h"
#include "strategy_test.h"
#include "strategy_trend_following.h"

void *strategy_processor(void *arg);

typedef enum Strategies {
  STRATEGY_TEST,
  STRATEGY_TREND_FOLLOWING,
} Strategies;

typedef Order * (Strategy_callback)(Tsmetadata *metadata, Candle candle);

static Strategy_callback *strategies[2] = {
  strategy_test, 
  strategy_trend_following,
};

#endif 
