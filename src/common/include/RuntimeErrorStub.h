#ifndef RUNTIMEERRORSTUB_H
#define RUNTIMEERRORSTUB_H
#include "RuntimeError.h"

void runtimeErrorStub_reset(void);
const char * runtimeErrorStub_get_last_error(void);
int runtimeErrorStub_get_last_parameter(void);

#endif 
