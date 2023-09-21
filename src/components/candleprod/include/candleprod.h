#ifndef CANDLEPROD_H
#define CANDLEPROD_H
#include "candle.h"
#include "csv.h"
#include "mtqueue.h"

// int candleprod_from_csv_row(const char *line, void *record);
int candleprod_from_csv_row(const char *line, Mtqueue_list *queues);

void *candleprod_produce(void *arg);

void *candleprod_produce_from_file(void *arg);

#endif 
