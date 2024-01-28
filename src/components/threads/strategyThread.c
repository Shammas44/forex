#include "strategyThread.h"
#include "CandleWrapper.h"
#include "Message.h"
#include "globalState.h"
#include "StrategyProcessor.h"
#include "MtsQueue.h"
#include "Metadata.h"
#include "wrapper.h"
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
    Message *message = candle_queue->dequeue(candle_queue);
    Msg_type type = message->code(message, READ, 0);
    if(type == Msg_stop){
      message->destructor(message);
      break;
    }
    if(type ==Msg_candle){
      CandleWrapper *candle = message->value(message,READ,(Item){}).value;
      message->destructor(message);
      Order * order = strategy->run(strategy, candle);
      OrderStatus status = order->status(order,READ,NULL);
      if(status == ORDER_PENDING) {
        printf("order pending: %d\n",id);
        // perform risk management check here 
        // send order to broker
      }
      printf("order processed: %d\n",id);
    }
    id++;
    sync_set_state(sync, SYNC_STATE_EXCHANGE);
  }
  return NULL;
}
