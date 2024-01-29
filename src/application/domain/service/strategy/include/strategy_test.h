#ifndef STRATEGY_TEST_H
#define STRATEGY_TEST_H
#include "Strategy.h"

#define T Strategy_test

typedef Strategy T;

T* strategy_test_constructor(Metadata*metadata);

#undef T
#endif
