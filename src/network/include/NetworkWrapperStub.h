#ifndef NETWORKWRAPPERSTUB_H
#define NETWORKWRAPPERSTUB_H
#include "url.h"
#include "network.h"

#define T NetworkWrapperStub

typedef Network T;

T* networkWrapperStub_constructor();
#undef T
#endif
