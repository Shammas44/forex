#ifndef EXCHANGETESTBACKTEST_H
#define EXCHANGETESTBACKTEST_H
#include "Exchange.h"
#include "Metadata.h"

#define T ExchangeTestBacktest

typedef Exchange T;

T* exchangeTestBacktest_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser,Metadata*metadata);
#undef T
#endif 
