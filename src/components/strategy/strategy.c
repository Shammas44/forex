#include "Strategy.h"
#include "Order.h"

// void *strategy_processor(void *arg) {
//   Mtqueue_list *queues = (Mtqueue_list *)arg;
//   Tsmetadata *metadata = queues->metadata;
//   Strategy_callback *strategy = strategies[metadata->strategy];
//   int id = 0;

//   while (1) {
//     sync_wait_on_state(queues->sync, SYNC_STATE_BARS);
//     Candle *candle = (Candle *)mtqueue_dequeue(queues->bars);

//     if (candle->status == CANDLE_STOP) {
//       Order *stop = malloc(sizeof(Order));
//       stop->status = ORDER_STOP;
//       stop->id = -1;
//       strategy(metadata, *candle);
//       mtqueue_enqueue(queues->orders, stop);
//       sync_set_state(queues->sync, SYNC_STATE_ORDERS);
//       free(candle);
//       break;
//     }

//     tsmetadata_set_equity(metadata, candle->close);
//     tsmetadata_set_last_price(metadata, candle->close);

//     Order *order = strategy(metadata, *candle);
//     free(candle);

//     if (order->size != 0) {
//       order->id = id++;
//     }

//     mtqueue_enqueue(queues->orders, order);
//     // printf("order CREATED: %d\n", order->id);
//     sync_set_state(queues->sync, SYNC_STATE_ORDERS);
//   }

//   return NULL;
// }

