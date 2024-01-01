#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H
#include "MtsQueue.h"
#include "Exchange.h"
#include "synchronizer.h"
#include "ConfigWrapper.h"
#include "Metadata.h"

typedef struct {
  MtsQueue *candles;
  MtsQueue *orders;
  Exchange*exchange;
  SSL *ssl;
  ConfigWrapper *config;
  Metadata *metadata;
  Sync *sync;
} GlobalState;

#endif
