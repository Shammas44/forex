#include "candle.h"
#include "candleprod.h"
#include "connectors.h"
#include "csv.h"
#include "server.h"
#include "strategy.h"
#include "ordersProcessor.h"
#include "configparser.h"
#include "ExchangeTest.h"
#include "url.h"
#include "observer.h"
#include "httpsRequestBuilder.h"
#include "httpsResponseBuilder.h"
#include "https.h"

// void observer_callback(Observer* observer, void* newState){
//   puts("observer_callback");
// }

int main(int argc, char *argv[]) {
  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);
  // Tsmetadata *metadata = malloc(sizeof(Tsmetadata));
  // char * config = argv[1];
  // if (argv[1] == NULL) config = "./config.json";

  // configparser_init(config,metadata);
  // tsmetadata_print(metadata);

  // Mtqueue *ticks_queue = mtqueue_init(1);
  // if (!ticks_queue)
  //   return get_error("Failed to init queue\n");
  // Mtqueue *bars_queue = mtqueue_init(1);
  // if (!bars_queue)
  //   return get_error("Failed to init queue\n");
  // Mtqueue *orders_queue = mtqueue_init(1);
  // if (!orders_queue)
  //   return get_error("Failed to init queue\n");

  // SSL *ssl = NULL;

  // Sync *sync = sync_init(SYNC_STATE_BARS);

  // Mtqueue_list queues;
  // queues.bars = bars_queue;
  // queues.ticks = ticks_queue;
  // queues.orders = orders_queue;
  // queues.ssl = ssl;
  // queues.metadata = metadata;
  // queues.sync = sync;

  // pthread_t prod_tick, prod_candle, trade_logic, order_process, server;

  Https *https = https_constructor();

  HttpsRequestBuilder *req_builder = httpsRequestBuilder_constructor();
  struct HttpsRequestBuilder* __req_builder = (struct HttpsRequestBuilder*)req_builder;

  req_builder->build(__req_builder,"https://google.com");
  // req_builder->set_body_func(__req_builder,"hello world");
  // req_builder->add_header_func(__req_builder,"daf");
  // req_builder->add_header_func(__req_builder,"daf");
  req_builder->add_header(__req_builder,"Connection: close");
  Request * req = req_builder->request;
  req->print_func((struct Request*)req);

  //TODO: debug following call
  Response * res = https->get(https,req);
  // req->destructor_func((struct Request*)req);
  // req_builder->destructor(__req_builder);
  https->destructor(https);
  req_builder->destructor((struct HttpsRequestBuilder*)req_builder);
  res->print_func((struct Response*)res);
  res->destructor_func((struct Response*)res);


  // HttpsResponseBuilder *res_builder = httpsResponseBuilder_constructor();
  // struct HttpsResponseBuilder* __res_builder = (struct HttpsResponseBuilder*)res_builder;
  // res_builder->build_func(__res_builder, req->get_connection_func((struct Request*)req));
  // Response * res = res_builder->get_func(__res_builder);
  // res->receive_func((struct Response*)res);
  // res->print_func((struct Response*)res);
  // req->destructor_func((struct Request*)req);
  //TODO: debug the followings function call
  // res->destructor_func((struct Response*)res);

  // Url *url = url_constructor("ws://127.0.0.1");
  // struct Exchange *exchangeTest = exchangeTest_constructor(&queues, url, NULL);
  // struct Exchange *exchange = exchange_constructor(exchangeTest);
  // int status = exchange_connect(exchange);
  // printf("status: %d\n", status);

  // if (metadata->mode == TSMETADATA_MODE_BACKTEST) {
  // pthread_create(&prod_candle, NULL, candleprod_produce_from_file, &queues);
  // } else if (metadata->mode == TSMETADATA_MODE_REAL) {
  // pthread_create(&prod_tick, NULL, connectors_produce_tick, &queues);
  // pthread_join(prod_tick, NULL);
  // pthread_create(&prod_candle, NULL, connectors_produce_candle, &queues);
  // pthread_join(prod_candle, NULL);
  // }

  // pthread_create(&trade_logic, NULL, strategy_processor, &queues);

  // pthread_create(&order_process, NULL, process_orders, &queues);

  // pthread_create(&server, NULL, server_init, bar_queue);
  // pthread_join(server, NULL);

  // pthread_join(prod_candle, NULL);
  // pthread_join(trade_logic, NULL);
  // pthread_join(order_process, NULL);


  // int number_of_trades = metadata->number_of_trades;
  // double equity = metadata->equity;
  // mtqueue_free_list(&queues);

  // printf("Number of trades: %d\n", number_of_trades);
  // printf("Average trade: %f\n",equity/number_of_trades);

  gettimeofday(&end_time, NULL); 
  // Calculate and print the total execution time
  double total_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1.0e6;
  printf("Total Real Time: %f seconds\n", total_time);

  // char * cmd = malloc(sizeof(char) * 100);
  // sprintf(cmd,"python3 ./python/%s.py equity.csv", metadata->strategy_name);
  // int return_value = system(cmd);
  // free(cmd);
  //  if (return_value == 0) {
  //       printf("Python script executed successfully.\n");
  //   } else {
  //       printf("Error executing Python script.\n");
  //   }
  return 0;
}
