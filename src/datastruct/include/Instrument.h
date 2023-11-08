#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include <stdbool.h>

typedef enum ProdType {
  ProdType_FOREX,
} ProdType;

typedef struct Instrument {
  char name[20];
  bool is_open_for_update;
  char Exchange[20];
  // OrderSet * order_set;
  ProdType prod_type;
  char prod_name[20]; // example: EURUSD
  double tick_size;
} Instrument;

void *instrument_constructor();

void instrument_destructor();
#endif
