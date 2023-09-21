#ifndef STRATEGY_TEST_H
#define STRATEGY_TEST_H
#include "candle.h"
#include "csv.h"
#include "order.h"
#include "tsmetadata.h"

Order *strategy_test(Tsmetadata *metadata, Candle candle);

#endif
