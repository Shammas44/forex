#include "tsmetadata.h"

// =========================================================================="
// Prototypes functions
// =========================================================================="

// =========================================================================="
// Private functions
// =========================================================================="

// =========================================================================="
// Public functions
// =========================================================================="

/**
 * @public - set metadata equity
 * @param m - metadata
 * @param current_price - new price
 * @return void
 */
void tsmetadata_set_equity(Tsmetadata *m, double current_price) {
  m->equity += (current_price - m->last_price) * m->market_position;
}

void tsmetadata_init(Tsmetadata *metadata) {
  metadata->capital = 0;
  metadata->leverage = 0;
  metadata->market_position = 0;
  metadata->equity = 0;
  metadata->last_price = 0;
  metadata->mode = TSMETADATA_MODE_BACKTEST;
  metadata->strategy = 0;
  metadata->spread = 0;
  metadata->number_of_trades = 0;
  metadata->broker_commision_dpm = 0;
  pthread_mutex_init(&metadata->mutex, NULL);
}

double tsmetadata_get_available_funds(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = (m->capital + m->equity) * m->leverage - m->market_position / m->leverage;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

double tsmetadata_get_equity(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = m->equity;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_set_capital(Tsmetadata *m, double capital) {
  pthread_mutex_lock(&m->mutex);
  m->capital = capital;
  pthread_mutex_unlock(&m->mutex);
}

double tsmetadata_get_capital(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = m->capital;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

int tsmetadata_get_leverage(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  int value = m->leverage;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_set_leverage(Tsmetadata *m, int leverage) {
  pthread_mutex_lock(&m->mutex);
  m->leverage = leverage;
  pthread_mutex_unlock(&m->mutex);
}

double tsmetadata_get_market_position(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = m->market_position;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_set_market_position(Tsmetadata *m, int order_size) {
  pthread_mutex_lock(&m->mutex);
  m->market_position += order_size;
  pthread_mutex_unlock(&m->mutex);
}

double tsmetadata_get_last_price(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = m->last_price;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_set_last_price(Tsmetadata *m, double last_price) {
  pthread_mutex_lock(&m->mutex);
  m->last_price = last_price;
  pthread_mutex_unlock(&m->mutex);
}

void tsmetadata_print(Tsmetadata *m){
  printf("symbol: %s\n", m->symbol);
  printf("capital: %f\n", m->capital);
  printf("leverage: %d\n", m->leverage);
  printf("market_position: %f\n", m->market_position);
  printf("equity: %f\n", m->equity);
  printf("last_price: %f\n", m->last_price);
  printf("mode: %d\n", m->mode);
  printf("strategy: %d\n", m->strategy);
  printf("strategy_name: %s\n", m->strategy_name);
  printf("spread: %f\n", m->spread);
  printf("backtest_data: %s\n", m->backtest_data);
  printf("broker_commision_dpm: %f\n", m->broker_commision_dpm);
  printf("number_of_trades: %d\n\n", m->number_of_trades);
}

double tsmetadata_get_number_of_trades(Tsmetadata *m){
  pthread_mutex_lock(&m->mutex);
  double value = m->number_of_trades;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_increment_number_of_trades(Tsmetadata *m, int number){
  pthread_mutex_lock(&m->mutex);
  m->number_of_trades+=number;
  pthread_mutex_unlock(&m->mutex);
}

void tsmetadata_set_spread(Tsmetadata *m, double spread) {
  pthread_mutex_lock(&m->mutex);
  m->spread = spread; 
  pthread_mutex_unlock(&m->mutex);
}

double tsmetadata_get_spread(Tsmetadata *m) {
  pthread_mutex_lock(&m->mutex);
  double value = m->spread;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

void tsmetadata_set_broker_commision_dpm(Tsmetadata *m, double dpm){
  pthread_mutex_lock(&m->mutex);
  m->broker_commision_dpm = dpm; 
  pthread_mutex_unlock(&m->mutex);
}

double tsmetadata_get_broker_commision_dpm(Tsmetadata *m){
  pthread_mutex_lock(&m->mutex);
  double value = m->broker_commision_dpm;
  pthread_mutex_unlock(&m->mutex);
  return value;
}

char *tsmetadata_get_backtest_data(Tsmetadata *m){
  pthread_mutex_lock(&m->mutex);
  char *value = m->backtest_data;
  pthread_mutex_unlock(&m->mutex);
  return value;
}
