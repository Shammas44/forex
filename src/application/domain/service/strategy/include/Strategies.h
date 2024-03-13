#ifndef STRATEGIES_H
#define STRATEGIES_H
// strategies
#include "RuntimeError.h"
#include "Strategy_test.h"
#include "Strategy_trend_following.h"
#include "Strategy_none.h"

typedef enum Strategies {
  STRATEGY_TEST = 1,
  STRATEGY_TREND_FOLLOWING = 2,
} Strategies;

static Strategy_constructor *strategies[] = {
  strategy_none_constructor,
  strategy_test_constructor, 
  strategy_trend_following_constructor,
};

#endif
