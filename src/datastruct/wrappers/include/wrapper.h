#ifndef WRAPPER_H
#define WRAPPER_H
#include "hashmap.h"
#include <stdbool.h>

#define T Wrapper

typedef struct T T;

char* wrapper_get_string(Hashmap*map,const char*key);

int wrapper_get_int(Hashmap*map,const char*key);

bool wrapper_get_bool(Hashmap*map,const char*key);

double wrapper_get_double(Hashmap*map,const char*key);

#undef T
#endif
