#ifndef STRATEGY_TREND_FOLLOWING_H
#define STRATEGY_TREND_FOLLOWING_H
#include "Strategy.h"

#define T Strategy_trend_following

typedef Strategy T;

T* strategy_trend_following_constructor(Metadata*metadata);

#undef T
#endif
