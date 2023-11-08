#ifndef OBSERVER_H
#define OBSERVER_H

struct Observer;
typedef void (*Observer_update_func)(struct Observer* observer, void* newState);

typedef struct Observer {
    Observer_update_func update;
} Observer;

Observer *observer_constructor(Observer_update_func func);

void observer_destroy(Observer* observer);

#endif
