#include "exchangeThread.h"
#include "CsvParser.h"
#include "ExchangeTest.h"
#include "ExchangeTestBacktest.h"
#include "globalState.h"
#include "CandleWrapper.h"
#include "MtsQueue.h"

static void __live_callback(void* newState);
static void __backtest_callback(void* newState);
static GlobalState *state;

void* exchangeThread(void *args){
  state = (GlobalState *)args;
  ConfigWrapper *config = state->config;
  Exchange *exchange = state->exchange;
  Parser*parser = jsonParser_constructor();
  Observer *observer;

  bool isBacktest = strcmp(config->mode(config),"BACKTEST")==0 ? true:false;
  if(isBacktest){
    observer = observer_constructor(__backtest_callback);
  }else {
    observer = observer_constructor(__live_callback);
  }

  int status = exchange->connect(exchange);
  if(status != 0) return NULL;
  exchange->attach_observer(exchange,observer);
  exchange->subscribe(exchange,NULL);
  return NULL;
}

static void __live_callback(void* newState){
  Hashmap*map = (Hashmap*)newState;
  puts("-------");
  printf("type: %s\n",(char*)map->get(map, "type").value);
  printf("instrument_id: %s\n",(char*)map->get(map, "instrument_id").value);
  map->destructor(map);
}

static void __backtest_callback(void* newState){
  Hashmap*map = (Hashmap*)newState;
  Sync *sync = state->sync;
  CandleWrapper * candle = candleWrapper_constructor(map);
  Wrapper *wrapper = wrapper_constructor("Candle",candle);
  MtsQueue *candle_queue = state->candles;
  puts("enqueue");
  sync_wait_on_state(sync, SYNC_STATE_EXCHANGE);
  candle_queue->enqueue(candle_queue,wrapper);
  sync_set_state(sync, SYNC_STATE_BARS);
}
