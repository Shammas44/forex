#include "strategyThread.h"
#include "CandleWrapper.h"
#include "Message.h"
#include "Metadata.h"
#include "MtsQueue.h"
#include "RiskProcessor.h"
#include "StrategyProcessor.h"
#include "globalState.h"
#include <unistd.h>

static GlobalState *state;
static int id = 0;
static RiskProcessor *risk;

static void _$clenup();

void *strategyThread(void *arg) {
  state = (GlobalState *)arg;
  MtsQueue *candle_queue = (MtsQueue *)state->candles;
  Metadata *metadata = state->metadata;
  risk = riskProcessor_constructor(metadata);
  int strategy_num = metadata->get_strategy(metadata);
  StrategyProcessor *strategy = strategyProcessor_constructor(metadata);
  strategy->set_strategy(strategy, strategy_num);
  Sync *sync = state->sync;

  while (1) {
    sync_wait_on_state(sync, SYNC_STATE_BARS);
    Message *message = candle_queue->dequeue(candle_queue);
    Msg_type type = message->code(message, READ, 0);
    if (type == Msg_stop) {
      message->destructor(message);
      break;
    }
    if (type == Msg_candle) {
      CandleWrapper *candle = message->value(message, READ, (Item){}).value;
      double price = candle->close(candle);
      message->destructor(message);
      Order *order = strategy->run(strategy, candle);
      OrderStatus status = order->status(order, READ, 0);

      while (status == ORDER_PENDING) {
        // printf("order pending: %d\n", id);
        order = risk->process(risk, order);
        // printf("order processed: %d\n", id);
        Exchange *exchange = state->exchange;
        exchange->send_order(exchange,order);
        break;
      }
      if (status == ORDER_CANCELLED) {
        // printf("order cancelled: %d\n", id);
        order->destructor(order);
      }
    }
    id++;
    sync_set_state(sync, SYNC_STATE_EXCHANGE);
  }
  _$clenup();
  return NULL;
}

static void _$clenup() { risk->destructor(risk); }
