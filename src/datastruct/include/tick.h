#ifndef TICK_H
#define TICK_H
#include <time.h>
#include "jsmn.h"
#include "json.h"
#include <stdio.h>

typedef struct Tick {
  char type[10];
  char instrument_id[10];
  time_t timestamp;
  char trade_id[16];
  float best_bid;
  float best_ask;
  float last_quantity;
  float last_price;
  float session_open;
  float session_low;
  float session_high;
} Tick;

Tick * tick_create(char *json, jsmntok_t **tokens, int token_num);
void tick_print(Tick *tick);
bool tick_is_valid(const Tick tick);
#endif 
