#include "tick.h"
#define PROPS_NUM 11


bool tick_is_valid(const Tick tick){
double bid = tick.best_bid;
  double ask = tick.best_ask;
  if(ask - bid < 0.015){
    return true;
  }
  return false;
}

Tick * tick_create(char *json, jsmntok_t **tokens, int token_num){
  Tick *tick = malloc(sizeof(Tick));

  struct f  {
    Convert_callback fun;
    char *label;
    void *prop;
  };

  struct f prop[PROPS_NUM] = {
    {to_char, "type", &tick->type},
    {to_char, "trade_id",&tick->trade_id},
    {to_char, "instrument_id", &tick->instrument_id},
    {to_time, "timestamp", &tick->timestamp},
    {to_float, "best_bid",&tick->best_bid},
    {to_float, "best_ask",&tick->best_ask},
    {to_float, "last_quantity",&tick->last_quantity},
    {to_float, "last_price",&tick->last_price},
    {to_float, "session_open",&tick->session_open},
    {to_float, "session_low",&tick->session_low},
    {to_float, "session_high",&tick->session_high},
  };

  for (int i = 1; i < token_num; i++) {
    for (int j = 0; j < PROPS_NUM; j++) {
      if (json_cmp_token_to_string(json, &((*tokens)[i]), prop[j].label) == 0) {
        json_extract_token(prop[j].label, *tokens, i, json, prop[j].prop, prop[j].fun);
        i++;
      }
    }
  }
  return tick;
}

void tick_print(Tick *tick){
  printf("type: %s\n", tick->type);
  printf("instrument_id: %s\n", tick->instrument_id);
  printf("timestamp: %ld\n", tick->timestamp);
  printf("trade_id: %s\n", tick->trade_id);
  printf("best_bid: %f\n", tick->best_bid);
  printf("best_ask: %f\n", tick->best_ask);
  printf("last_quantity: %f\n", tick->last_quantity);
  printf("last_price: %f\n", tick->last_price);
  printf("session_open: %f\n", tick->session_open);
  printf("session_low: %f\n", tick->session_low);
  printf("session_high: %f\n", tick->session_high);
}
