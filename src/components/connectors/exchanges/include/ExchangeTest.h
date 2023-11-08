#ifndef EXCHANGETEST_H
#define EXCHANGETEST_H
#include "connectors.h"
#include "Exchange.h"
#include "url.h"

 struct Exchange* exchangeTest_constructor(Mtqueue_list *args, Url* url, char* auth_token);

#endif 
