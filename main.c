#include "candle.h"
#include "candleprod.h"
#include "csv.h"
#include "json.h"
#include "server.h"
#include "strategy.h"
#include "ordersProcessor.h"
#include "configparser.h"
#include "ExchangeTest.h"
#include "ExchangeTestBacktest.h"
#include "url.h"
#include "observer.h"
#include "https.h"
#include "wsHandler.h"
#include "CandleWrapper.h"
#include "ConfigWrapper.h"
#include "common.h"
#include "CsvParser.h"

void live_callback(void* newState){
  Hashmap*map = (Hashmap*)newState;
  puts("-------");
  printf("type: %s\n",HASHMAP_GET_STRING(map, "type"));
  printf("instrument_id: %s\n",HASHMAP_GET_STRING(map, "instrument_id"));
  map->destructor(map);
}

void backtest_callback(void* newState){
  Hashmap*map = (Hashmap*)newState;
  CandleWrapper * candle = candleWrapper_constructor(map);

  puts("-------");
  printf("Time: %s\n",candle->time(candle));
  printf("Date: %s\n",candle->date(candle));
  printf("Timestamp: %ld\n",candle->timestamp(candle));
  printf("Volume: %f\n",candle->volume(candle));
  candle->destructor(candle);
}

int main(int argc, char *argv[]) {
  // RUNTIME_ERROR("pas cool", 1);
  // Error error = runtimeError_get_last_error();

  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);
  // Tsmetadata *metadata = malloc(sizeof(Tsmetadata));
  char * config_path = argv[1];
  if (argv[1] == NULL) config_path = "./config.json";

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

  Parser*parser = jsonParser_constructor();
  char *file_content = file_read(config_path);
  Hashmap * config_map = parser->parse(parser,file_content);
  if(config_map == NULL) return 1;
  ConfigWrapper *config = configWrapper_constructor(config_map);

  bool isBacktest = strcmp(config->mode(config),"BACKTEST")==0 ? true:false;

  Https *https = https_constructor();
  WsHandler *ws = wsHandler_constructor(https);
  Exchange * exchange = NULL;
  Observer *observer;

  if(isBacktest){
    parser->destructor(parser);
    CsvParser_config csv_config;
    csv_config.delimiter = ",";
    parser = csvParser_constructor();
    Parser_config_obj *c = (Parser_config_obj *) &csv_config;
    parser->config(parser,c);
    exchange = exchangeTestBacktest_constructor(ws, config,parser);
    observer = observer_constructor(backtest_callback);
  }else {
    parser = jsonParser_constructor();
    exchange = exchangeTest_constructor(ws, config,parser);
    observer = observer_constructor(live_callback);
  }

  int status = exchange->connect(exchange);
  if(status != 0) exit(1);
  exchange->attach_observer(exchange,observer);
  exchange->subscribe(exchange,NULL);
  exit(0);

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
