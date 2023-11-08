#include "observer.h"
#include <stdlib.h>

Observer * observer_constructor(Observer_update_func func){
  Observer * observer = malloc(sizeof(Observer));
  observer->update = func;
  return observer;
}

void observer_destroy(Observer* observer){
  free(observer);
}
