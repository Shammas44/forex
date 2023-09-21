#ifndef STRATEGY_TREND_FOLLOWING_H
#define STRATEGY_TREND_FOLLOWING_H
#include "candle.h"
#include "csv.h"
#include "movingaverage.h"
#include "order.h"
#include "slidingwindow.h"
#include "tsmetadata.h"

Order *strategy_trend_following(Tsmetadata *metadata, Candle candle);

#endif
