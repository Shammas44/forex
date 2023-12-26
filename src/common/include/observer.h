#ifndef OBSERVER_H
#define OBSERVER_H
#include <stdlib.h>
#define T Observer
#define S Subject
typedef struct S S;
typedef struct T T;

struct T;
typedef void (*Observer_update)(void* newState);
struct Subject;

struct T {
    Observer_update update;
};

typedef void (Subject_destructor)(S* subject);
typedef void (Subject_attach)(S* subject, T* observer);
typedef void (Subject_detach)(S* subject, T* observer);
typedef void (Subject_notify)(S* subject);
typedef void (Subject_set_state)(S* subject, void* newState);

struct S {
  Subject_destructor* destructor;
  void* state;
  int numObservers;
  Observer** observers; // Array of pointers to Observers
  Subject_attach* attach;
  Subject_detach* detach;
  Subject_notify* notify;
  Subject_set_state* set_state;
};

S *subject_constructor(void* initial_state);

T *observer_constructor(Observer_update update);

#undef T
#undef S
#endif
