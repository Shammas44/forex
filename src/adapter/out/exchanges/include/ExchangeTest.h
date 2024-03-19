#ifndef EXCHANGETEST_H
#define EXCHANGETEST_H
#include "Exchange.h"
#include "Metadata.h"

#define T ExchangeTest

typedef Exchange T;

T* exchangeTest_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser,Metadata*metadata);
#undef T
#endif 
