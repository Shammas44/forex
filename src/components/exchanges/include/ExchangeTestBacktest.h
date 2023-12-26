#ifndef EXCHANGETESTBACKTEST_H
#define EXCHANGETESTBACKTEST_H
#include "Exchange.h"

#define T ExchangeTestBacktest

typedef Exchange T;

T* exchangeTestBacktest_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser);
#undef T
#endif 
