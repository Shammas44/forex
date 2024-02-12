#ifndef STRATEGIES_H
#define STRATEGIES_H
// strategies
#include "Strategy_test.h"
#include "Strategy_trend_following.h"

typedef enum Strategies {
  STRATEGY_TEST,
  STRATEGY_TREND_FOLLOWING,
} Strategies;

static Strategy_constructor *strategies[] = {
  strategy_test_constructor, 
  strategy_trend_following_constructor,
};

#endif
