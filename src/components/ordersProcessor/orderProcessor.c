#include "mtqueue.h"
#include "ordersProcessor.h"

void order_exectuted(Order *order, Mtqueue_list *queues);
void send_order(Order *order, Mtqueue_list *queues);
double calculate_slippage(double size);
double calculate_broker_commision(Tsmetadata *m, Order *order);
double get_value_of_one_pipe(double exchange_rate, double size);

void order_exectuted(Order *order, Mtqueue_list *queues) {
  Tsmetadata *metadata = queues->metadata;
  tsmetadata_increment_number_of_trades(metadata, 1);
  metadata->last_price = order->executed_price;
  int factor = order->side == SELL ? -1 : 1;
  tsmetadata_set_market_position(metadata, (order->size) * factor);
}

void emulate_broker(Order *order, Mtqueue_list *queues) {
  double spread = tsmetadata_get_spread(queues->metadata);
  double slippage = calculate_slippage(order->size);
  double commision = calculate_broker_commision(queues->metadata, order);
  order->status = ORDER_EXECUTED;
  order->executed_price = order->price + spread + slippage;
  mtqueue_enqueue(queues->orders, order);
}

double get_value_of_one_pipe(double exchange_rate, double size){
return 0.0001 / exchange_rate * size;
}

double calculate_broker_commision(Tsmetadata *m, Order *order){
  double size = order->size * order->price;
  double value = size / 1000000 * m->broker_commision_dpm;
  // printf("n: %f\n", value);
  // double pipe = get_value_of_one_pipe(order->price, order->size);
  // printf("pipe: %f\n", pipe);
  // printf("l: %f\n", value / pipe );
  return value;
}

double calculate_slippage(double size){
  double slippage = 0;
  if(size > 50000){
    slippage = 0.0001;
  }else if(size > 500000){
    slippage = 0.0002;
  }
  return slippage;
}

void send_order(Order *order, Mtqueue_list *queues) {
  Tsmetadata *metadata = queues->metadata;
  double available_funds = tsmetadata_get_available_funds(metadata);
  if (order->size < available_funds && order->size > 0) {
    // char *json = order_to_json(order);
    // ws_send_message(queues.ssl, json);
    emulate_broker(order, queues);
  }
}

void *process_orders(void *arg) {
  Mtqueue_list *queues = (Mtqueue_list *)arg;
  Tsmetadata *metadata = queues->metadata;
  bool stop = false;
  Candle *candle;

  while (1) {
    sync_wait_on_state(queues->sync, SYNC_STATE_ORDERS);
    Order *order = (Order *)mtqueue_dequeue(queues->orders);
    // printf("order %s: %d\n", order_get_status(*order), order->id);

    switch (order->status) {
    case ORDER_CANCELLED:
      free(order);
      sync_set_state(queues->sync, SYNC_STATE_BARS);
      break;
    case ORDER_PENDING:
      send_order(order, queues);
      break;
    case ORDER_EXECUTED:
      order_exectuted(order, queues);
      free(order);
      sync_set_state(queues->sync, SYNC_STATE_BARS);
      break;
    case ORDER_REJECTED:
      free(order);
      sync_set_state(queues->sync, SYNC_STATE_BARS);
      break;
    case ORDER_STOP:
      free(order);
      return NULL;
    }
  }
  return NULL;
}
