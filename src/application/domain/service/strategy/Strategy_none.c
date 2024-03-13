#include "Strategy_none.h"
#define T Strategy_none

T* strategy_none_constructor(Metadata*metadata){
  RUNTIME_ERROR("no strategy provided", 1);
  return NULL;
}
