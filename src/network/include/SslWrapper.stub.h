#ifndef SSLWRAPPER_STUB_H
#define SSLWRAPPER_STUB_H
#include "SslWrapper.h"

#define T SslWrapperStub

typedef SslWrapper T;

T* sslWrapperStub_constructor();
#undef T
#endif
