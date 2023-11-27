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
#include "RuntimeErrorStub.h"
#include "wsHandler.h"

// void observer_callback(Observer* observer, void* newState){
//   puts("observer_callback");
// }

int main(int argc, char *argv[]) {
  // int value = 1;
  // const char *msg = "pas cool"; 
  // RUNTIME_ERROR(msg, value);
  const char*error = runtimeErrorStub_get_last_error();
  printf("%s\n", error);

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
  WsHandler *ws = wsHandler_constructor(https);
  HttpsRequestBuilder *req_builder = httpsRequestBuilder_constructor();
  char authorization[] = "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJhZGFkIiwiZXhwIjoxNzAxNjAwOTA2LCJzY29wZSI6InVzZXIiLCJpYXQiOjE3MDA5OTYxMDZ9.XJNIetdnqH5f68xERchlI18TAuCASP12NNh6H1A62zQ";


  // req_builder->build(req_builder,"https://127.0.0.1:443");
  req_builder->build(req_builder,"https://127.0.0.1:443/api");
  // req_builder->set_body(req_builder,body);
  req_builder->add_header(req_builder,authorization);
  // req_builder->add_header(req_builder,"Connection: close");
  req_builder->add_header(req_builder,"Connection: Upgrade");
  req_builder->add_header(req_builder,"Upgrade: websocket");
  req_builder->add_header(req_builder,"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==");
  req_builder->add_header(req_builder,"Sec-WebSocket-Version: 13");
  Request * req = req_builder->get(req_builder);
  // req->print_func(req);

  SSL* ssl = https->ws_handshake(https,req);
  ssl != NULL ? puts("connection established") : puts("connection failed");
  req_builder->destructor(req_builder);
  ws->listen(ws,ssl);
  https->destructor(https);


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
