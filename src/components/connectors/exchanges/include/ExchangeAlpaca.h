#ifndef EXCHANGEALPACA_H
#define EXCHANGEALPACA_H
#include "Exchange.h"

#define T ExchangeAlpaca

typedef Exchange T;

T* exchangeAlpaca_constructor(WsHandler*ws,ConfigWrapper*config,Parser*parser);

#undef T
#endif
