#include "Metadata.h"
#include <stdlib.h>
#include <string.h>
#define T Metadata

typedef struct {
  pthread_mutex_t mutex;
  double capital;
  int leverage;
  double market_position;
  double equity;
  double last_price;
  int strategy;
  double spread;
  int number_of_trades;
  double broker_commision_dpm;
  Array *positions;
} Private;

static void __destructor(T *self);
static void __set_equity(T *self, double current_price);
static double __get_equity(T *self);
static void __set_leverage(T *self, int leverage);
static int __get_leverage(T *self);
static double __get_available_funds(T *self);
static void __set_capital(T *self, double capital);
static double __get_capital(T *self);
static double __get_market_position(T *self);
static void __set_market_position(T *self, int order_size);
static void __set_number_of_trades(T *self, int number);
static double __get_number_of_trades(T *self);
static void __set_last_price(T *self, double last_price);
static double __get_last_price(T *self);
static void __set_spread(T *self, double spread);
static double __get_spread(T *self);
static void __set_broker_commision_dpm(T *self, double dpm);
static double __get_broker_commision_dpm(T *self);
static void __set_strategy(T *self, int strategy);
static int __get_strategy(T *self);
static void __open_position(T*self,Order*order);
static void __close_position(T*self,char*order_id);

T* metadata_constructor(ConfigWrapper *config){
  T* self = malloc(sizeof(T));
  self->destructor = __destructor;
  self->set_equity = __set_equity;
  self->get_equity = __get_equity;
  self->set_leverage = __set_leverage;
  self->get_leverage = __get_leverage;
  self->get_available_funds = __get_available_funds;
  self->set_capital = __set_capital;
  self->get_capital = __get_capital;
  self->get_market_position = __get_market_position;
  self->set_market_position = __set_market_position;
  self->set_number_of_trades = __set_number_of_trades;
  self->get_number_of_trades = __get_number_of_trades;
  self->set_last_price = __set_last_price;
  self->get_last_price = __get_last_price;
  self->set_spread = __set_spread;
  self->get_spread = __get_spread;
  self->set_broker_commision_dpm = __set_broker_commision_dpm;
  self->get_broker_commision_dpm = __get_broker_commision_dpm;
  self->set_strategy = __set_strategy;
  self->get_strategy = __get_strategy;
  Private *private = malloc(sizeof(Private));
  memset(private,0,sizeof(Private));
  private->capital = config->capital(config);
  private->leverage = config->leverage(config);
  private->spread = config->spread(config);
  private->broker_commision_dpm = config->broker_commision_dpm(config);
  private->strategy = config->strategy(config);
  private->positions = array_constructor(20);
  pthread_mutex_init(&private->mutex, NULL);
  self->__private = private;
  return self;
}

void __destructor(T*self){
  Private *private = self->__private;
  free(private);
  free(self);
}

static void __set_equity(T *self, double current_price) {
  Private *private = self->__private;
  private->equity += (current_price - private->last_price) * private->market_position;
}

static double __get_equity(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->equity;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static double __get_available_funds(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = (private->capital + private->equity) * private->leverage - private->market_position / private->leverage;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_capital(T *self, double capital) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->capital = capital;
  pthread_mutex_unlock(&private->mutex);
}

static double __get_capital(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->capital;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static int __get_leverage(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  int value = private->leverage;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_leverage(T *self, int leverage) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->leverage = leverage;
  pthread_mutex_unlock(&private->mutex);
}

static double __get_market_position(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->market_position;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_market_position(T *self, int order_size) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->market_position += order_size;
  pthread_mutex_unlock(&private->mutex);
}

static double __get_last_price(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->last_price;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_last_price(T *self, double last_price) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->last_price = last_price;
  pthread_mutex_unlock(&private->mutex);
}

static void __print(T *self){
  Private *private = self->__private;
  printf("capital: %f\n", private->capital);
  printf("leverage: %d\n", private->leverage);
  printf("market_position: %f\n", private->market_position);
  printf("equity: %f\n", private->equity);
  printf("last_price: %f\n", private->last_price);
  printf("strategy: %d\n", private->strategy);
  printf("spread: %f\n", private->spread);
  printf("broker_commision_dpm: %f\n", private->broker_commision_dpm);
  printf("number_of_trades: %d\n\n", private->number_of_trades);
}

static double __get_number_of_trades(T *self){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->number_of_trades;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_number_of_trades(T *self, int number){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->number_of_trades =number;
  pthread_mutex_unlock(&private->mutex);
}

static void __set_spread(T *self, double spread) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->spread = spread; 
  pthread_mutex_unlock(&private->mutex);
}

static double __get_spread(T *self) {
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->spread;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_broker_commision_dpm(T *self, double dpm){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->broker_commision_dpm = dpm; 
  pthread_mutex_unlock(&private->mutex);
}

static double __get_broker_commision_dpm(T *self){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  double value = private->broker_commision_dpm;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __set_strategy(T *self, int strategy){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  private->strategy = strategy; 
  pthread_mutex_unlock(&private->mutex);
}

static int __get_strategy(T *self){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  int value = private->strategy;
  pthread_mutex_unlock(&private->mutex);
  return value;
}

static void __open_position(T*self,Order*order){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  Array* positions = private->positions;
  positions->push(positions,(Item){.type=Item_map,.value=order});
  pthread_mutex_unlock(&private->mutex);
}
static void __close_position(T*self,char*order_id){
  Private *private = self->__private;
  pthread_mutex_lock(&private->mutex);
  Array* positions = private->positions;
  // positions->delete(positions,order_id);
  pthread_mutex_unlock(&private->mutex);
}

#undef T
