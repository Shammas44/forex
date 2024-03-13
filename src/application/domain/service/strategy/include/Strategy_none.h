#ifndef STRATEGY_NONE_H
#define STRATEGY_NONE_H
#include "Strategy.h"

#define T Strategy_none

typedef Strategy T;

T* strategy_none_constructor(Metadata*metadata);

#undef T
#endif
