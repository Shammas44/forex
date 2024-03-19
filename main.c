#include "Metadata.h"
#include "network.h"
#include "SslWrapper.h"
#include "json.h"
#include "configparser.h"
#include "ExchangeTest.h"
#include "ExchangeTestBacktest.h"
#include "https.h"
#include "wsHandler.h"
#include "ConfigWrapper.h"
#include "common.h"
#include "globalState.h"
#include "MtsQueue.h"
#include "CsvParser.h"
#include "strategyThread.h"
#include "exchangeThread.h"
#include "EventsLoop.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Async async_printer(void *args, Resolve resolve, Reject reject, Finally finally) {
  (void)(reject);
  (void)(finally);
  sleep(1);
  return resolve(args);
}

void print_chart(Hashmap* config){
  char * cmd = malloc(sizeof(char) * 100);
  char*strategy_name = config->get(config,"strategy").value;
  sprintf(cmd,"python3 ./python/%s.py equity.csv", strategy_name);
  int return_value = system(cmd);
  free(cmd);
   if (return_value == 0) {
        printf("Python script executed successfully.\n");
    } else {
        printf("Error executing Python script.\n");
    }
}

// void average_trade(){
//     CsvParser_config csv_config;
//     csv_config.delimiter = ",";
//     csv_config.type = CsvParser_type_file;
//     Parser_config_obj *c = (Parser_config_obj *) &csv_config;
//     CsvParser *parser = csvParser_constructor();
//     parser->config(parser, c);
//     Array *array = parser->parse(parser, "executed.csv");
//     int rows = array->length(array);
//     for (int i = 0; i < rows; i++) {
//       Hashmap *row = array->get(array, i).value;
//       double *equity = row->get(row, "equity").value;
//       printf("Equity: %f\n", *equity);
//     }
// }

static int count = -1;
int max = 2;

void *printer(void *args) {
  printf("Data: %d\n", *(int *)args);
  free(args);
  count++;
  if(count == max -1){ 
    puts("finish");
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  AsyncQueue *queue = asyncQueue_constructor(10);
  MtsLinkedList *async_callbacks = mtsLinkedList_constructor();
  EventsLoop * eventsLoop = eventsLoop_constructor(queue, async_callbacks);
  if(eventsLoop == NULL) exit(EXIT_FAILURE); 
  pthread_t thread = eventsLoop->start(eventsLoop);

  // int i = 0;
  // while (i < max) {
  //   int *args = malloc(sizeof(int));
  //   *args = i;
  //   Promise *promise = ASYNC(async_printer, args);
  //   promise->then(promise, printer, NULL, NULL);
  //   printf("i: %d\n", i);
  //   i++;
  // }

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
  int t = config->strategy(config);

  bool isBacktest = config->mode(config) == Metadata_mode_backtest ? true:false;

  Network *network = network_constructor();
  SslWrapper *sslWrapper = sslWrapper_constructor();
  Https *https = https_constructor(network,sslWrapper);
  WsHandler *ws = wsHandler_constructor(https);
  Exchange * exchange = NULL;

  Metadata*metadata = metadata_constructor(config);
  if(metadata == NULL){
    exit(RUNTIME_ERROR("Metadata is null",1));
  }

  if(isBacktest){
    parser->destructor(parser);
    CsvParser_config csv_config;
    csv_config.delimiter = ",";
    parser = csvParser_constructor();
    Parser_config_obj *c = (Parser_config_obj *) &csv_config;
    parser->config(parser,c);
    exchange = exchangeTestBacktest_constructor(ws, config,parser,metadata);
  }else {
    parser = jsonParser_constructor();
    exchange = exchangeTest_constructor(ws, config,parser,metadata);
  }

  SSL *ssl = NULL;
  Sync *sync = sync_init(SYNC_STATE_EXCHANGE);

  GlobalState state;
  state.candles = candles_queue;
  state.orders = orders_queue;
  state.config = config;
  state.ssl = ssl;
  state.exchange = exchange;
  state.sync = sync;
  state.metadata = metadata;

  puts("");
  pthread_create(&exchange_thread, NULL, exchangeThread, &state);
  pthread_create(&strategy_thread, NULL, strategyThread, &state);

  pthread_join(exchange_thread, NULL);
  pthread_join(strategy_thread, NULL);

  eventsLoop->stop();
  pthread_join(thread, NULL);
  eventsLoop->destructor(eventsLoop);

  // pthread_create(&server, NULL, server_init, bar_queue);
  // pthread_join(server, NULL);

  int number_of_trades = metadata->get_number_of_trades(metadata);
  if(number_of_trades == 0) number_of_trades = 1;
  double equity = metadata->get_equity(metadata);
  // mtqueue_free_list(&queues);

  printf("Number of trades: %d\n", number_of_trades);
  printf("Average trade: %f\n",equity/number_of_trades);

  gettimeofday(&end_time, NULL); 
  // Calculate and print the total execution time
  double total_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1.0e6;
  printf("Total Real Time: %f seconds\n", total_time);

  // print_chart(config_map);
  return 0;
}
