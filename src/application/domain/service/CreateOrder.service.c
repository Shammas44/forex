#include "CreateOrder.service.h"
#define T CreateOrderService

typedef struct {
  OrderRepository *repo;
} Private;

static void __destructor(T *self);
static Order* execute(T*self,OrderUpdateArgs args);

T*createOrderService_constructor(OrderRepository *repo) {
  T*self = malloc(sizeof(Usecase));
  Private *private = malloc(sizeof(Private));
  private->repo = repo;
  self->__private = private;
  self->__destructor = (IsDestroyable){.destructor = __destructor};
  return self;
}

static Order* execute(T*self,OrderUpdateArgs args){
  Private *private = self->__private;
  OrderRepository *repo = private->repo;
  char*id = args.id;
  Order *order = args.order;
  Order * value = repo->create(repo,order);
  return value;
}

static void __destructor(T *self) {
  free(self->__private);
  free(self);
}

#undef T
