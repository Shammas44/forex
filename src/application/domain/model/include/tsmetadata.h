#ifndef TSMETADATA_H
#define TSMETADATA_H
#include <pthread.h>
#include <stdio.h>

typedef enum { 
  TSMETADATA_MODE_BACKTEST,
  TSMETADATA_MODE_REAL 
} Tsmetadata_mode;

typedef struct Tsmetadata {
  pthread_mutex_t mutex;
  double capital;
  int leverage;
  Tsmetadata_mode mode;
  double market_position;
  double equity;
  double last_price;
  int strategy;
  char strategy_name[50];
  char backtest_data[50];
  char symbol[10];
  double spread;
  int number_of_trades;
  double broker_commision_dpm;
  // equity_timeseries = []
  // list_of_orders = []
  // orders_buffer = [] # added
  // number_of_trades = 0 # added
} Tsmetadata;


void tsmetadata_init(Tsmetadata *metadata);

void tsmetadata_print(Tsmetadata *m);

void tsmetadata_set_equity(Tsmetadata *m, double current_price);

double tsmetadata_get_available_funds(Tsmetadata *m);

double tsmetadata_get_equity(Tsmetadata *m);

void tsmetadata_set_capital(Tsmetadata *m, double capital);

double tsmetadata_get_capital(Tsmetadata *m);

int tsmetadata_get_leverage(Tsmetadata *m);

void tsmetadata_set_leverage(Tsmetadata *m, int leverage);

double tsmetadata_get_market_position(Tsmetadata *m);

void tsmetadata_set_market_position(Tsmetadata *m, int order_size);

double tsmetadata_get_number_of_trades(Tsmetadata *m);

void tsmetadata_increment_number_of_trades(Tsmetadata *m, int number);

double tsmetadata_get_last_price(Tsmetadata *m);

void tsmetadata_set_last_price(Tsmetadata *m, double last_price);

void tsmetadata_set_spread(Tsmetadata *m, double spread);

double tsmetadata_get_spread(Tsmetadata *m);

void tsmetadata_set_broker_commision_dpm(Tsmetadata *m, double dpm);

double tsmetadata_get_broker_commision_dpm(Tsmetadata *m);

char *tsmetadata_get_backtest_data(Tsmetadata *m);

#endif
