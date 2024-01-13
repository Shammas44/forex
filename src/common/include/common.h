#include "ConfigWrapper.h"
#include "RuntimeErrorImp.h"
#ifndef COMMON_H
#define COMMON_H

extern ConfigWrapper *CONFIG;

typedef enum {
  READ,
  WRITE,
} Access_mode;

typedef enum {
  ADD,
  REMOVE,
} Edition_mode;

#endif

