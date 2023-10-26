#ifndef EXCHANGETEST_H
#define EXCHANGETEST_H
#include "connectors.h"
#include "Exchange.h"

struct Exchange* exchangeTest_constructor(Exchange_protocols protocols, char* url, char* auth_token);

#endif 
