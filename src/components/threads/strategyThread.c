#include "strategyThread.h"
#include "CandleWrapper.h"
#include "globalState.h"
#include "StrategyProcessor.h"
#include "MtsQueue.h"
#include "Metadata.h"
#include "Wrapper.h"
#include <unistd.h>

static GlobalState *state;
static int id =0;

void *strategyThread(void *arg) {
  state = (GlobalState *)arg;
  MtsQueue *candle_queue = (MtsQueue *)state->candles;
  Metadata *metadata = state->metadata;
  StrategyProcessor *strategy = strategyProcessor_constructor(metadata);
  strategy->set_strategy(strategy, STRATEGY_TEST);
  Sync *sync = state->sync;

  while (1) {
    sync_wait_on_state(sync, SYNC_STATE_BARS);
    Wrapper *wrapper = candle_queue->dequeue(candle_queue);
    CandleWrapper *candle = wrapper->content(wrapper);
    wrapper->destructor(wrapper);
    // puts("-------");
    // printf("Time: %s\n", candle->time(candle));
    // printf("Date: %s\n", candle->date(candle));
    // printf("Timestamp: %ld\n", candle->timestamp(candle));
    // printf("Volume: %f\n", candle->volume(candle));
    // printf("Close: %f\n", candle->close(candle));
    printf("order: %d\n", id);
    // Order * order = strategy->run(strategy, candle);
    candle->destructor(candle);
    printf("order processed: %d\n",id);
    id++;
    sync_set_state(sync, SYNC_STATE_EXCHANGE);
  }
}
