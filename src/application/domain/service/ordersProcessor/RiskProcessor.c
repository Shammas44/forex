#include "RiskProcessor.h"
#include "common.h"
#define T RiskProcessor

typedef struct {
  Metadata *metadata;
} Private;

static Order* __process(T *self, Order *order);
static void __destructor(T *self);

static bool _$check_available_funds(T*self,Order*order);
static bool _$check_size_limit(T*self,Order*order);

T *riskProcessor_constructor(Metadata* metadata){
  T *self = malloc(sizeof(T));
  Private *private = malloc(sizeof(Private));
  private->metadata = metadata;
  self->__private = private;
  self->destructor = __destructor;
  self->process = __process;
  return self;
}

static void __destructor(T *self) {
  free(self->__private);
  free(self);
}

static Order *__process(T *self, Order *order) { 
  bool status = true;
  while (status){
    status = _$check_available_funds(self,order);
    status = _$check_size_limit(self,order);
    break;
  }
  if (!status) {
    order->status(order,WRITE,ORDER_CANCELLED);
  }
  return order; 
}

static bool _$check_available_funds(T*self,Order*order){
  bool status = false;
  Private *private = self->__private;
  Metadata *metadata = private->metadata;
  double order_size = order->size(order,READ,0);
  double available_funds = metadata->get_available_funds(metadata);
  if (order_size < available_funds && order_size > 0) {
    status = true;
  }
  // if(!status){
  //   RUNTIME_ERROR("Not enough available funds",1);
  // }   
  return status;
}

static bool _$check_size_limit(T*self,Order*order){
  double percentage_limit = 2;
  bool status = true;
  Private *private = self->__private;
  Metadata *metadata = private->metadata;
  double available_funds = metadata->get_available_funds(metadata);
  Side side = order->side(order,READ,0);
  if(side == BUY){
    double order_size = order->size(order,READ,0);
    double limit = available_funds * percentage_limit / 100;
    if(order_size >= limit || order_size <= 0){
      status = false;
    }
  }
  // if(!status) RUNTIME_ERROR("order size limit outbound",1);
  return status;
}

#undef T
