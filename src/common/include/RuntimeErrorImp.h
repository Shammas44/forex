#ifndef RUNTIMEERRORIMP_H
#define RUNTIMEERRORIMP_H
#include "RuntimeError.h"

void runtimeError_reset(void);
Error runtimeError_get_last_error(void);
int runtimeError_get_last_parameter(void);

#endif
