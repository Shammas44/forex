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
#include "globalState.h"
#include "MtsQueue.h"
#include "CsvParser.h"
#include "strategyThread.h"
#include "exchangeThread.h"

int main(int argc, char *argv[]) {
  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);
  char * config_path = argv[1];
  if (argv[1] == NULL) config_path = "./config.json";

  MtsQueue *candles_queue = mtsQueue_constructor(1);
  if (!candles_queue)
    return RUNTIME_ERROR("Failed to init candles queue",1);
  MtsQueue *orders_queue = mtsQueue_constructor(1);
  if (!orders_queue)
    return RUNTIME_ERROR("Failed to init orders queue",1);

  pthread_t exchange_thread, strategy_thread, order_process_thread, server_thread;

  Parser*parser = jsonParser_constructor();
  char *file_content = file_read(config_path);
  Hashmap * config_map = parser->parse(parser,file_content);
  if(config_map == NULL) return 1;
  ConfigWrapper *config = configWrapper_constructor(config_map);

  bool isBacktest = strcmp(config->mode(config),"BACKTEST")==0 ? true:false;

  Https *https = https_constructor();
  WsHandler *ws = wsHandler_constructor(https);
  Exchange * exchange = NULL;

  if(isBacktest){
    parser->destructor(parser);
    CsvParser_config csv_config;
    csv_config.delimiter = ",";
    parser = csvParser_constructor();
    Parser_config_obj *c = (Parser_config_obj *) &csv_config;
    parser->config(parser,c);
    exchange = exchangeTestBacktest_constructor(ws, config,parser);
  }else {
    parser = jsonParser_constructor();
    exchange = exchangeTest_constructor(ws, config,parser);
  }

  SSL *ssl = NULL;
  // Sync *sync = sync_init(SYNC_STATE_BARS);

  GlobalState state;
  state.candles = candles_queue;
  state.orders = orders_queue;
  state.config = config;
  state.ssl = ssl;
  state.exchange = exchange;
  state.sync = NULL;
  state.metadata = metadata_constructor();

  pthread_create(&exchange_thread, NULL, exchangeThread, &state);
  pthread_create(&strategy_thread, NULL, strategyThread, &state);

  pthread_join(exchange_thread, NULL);
  pthread_join(strategy_thread, NULL);

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
