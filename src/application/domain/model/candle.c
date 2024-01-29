#include "candle.h"
#include "tick.h"

void candle_print(Candle *candle) {
  printf("------------\n");
  printf("open: %f\n", candle->open);
  printf("close: %f\n", candle->close);
  printf("high: %f\n", candle->high);
  printf("low: %f\n", candle->low);
  printf("volume: %f\n", candle->volume);
  printf("timestamp: %ld\n", candle->timestamp);
}

Candle_state candle_create(Candle *candle, Tick *tick, int duration) {
  static time_t current_interval_start = 0;
  static float high = 0;
  static float low = 0;
  static float open = 0;
  static float volume = 0;
  static time_t timestamp = 0;

  if (!tick_is_valid(*tick)) {
    return CURRENT_TICK_SKIPPED;
  };

  // Initialization when starting
  if (current_interval_start == 0) {
    current_interval_start = tick->timestamp;
    high = tick->last_price;
    low = tick->last_price;
    open = tick->last_price;
    volume = tick->last_quantity;
    timestamp = tick->timestamp;
    return CANDLE_IN_PROGRESS;
  }

  // Check if this tick is still within the current interval
  if (tick->timestamp < current_interval_start + duration) {
    if (tick->last_price > high)
      high = tick->last_price;
    if (tick->last_price < low)
      low = tick->last_price;
    volume += tick->last_quantity;
    return CANDLE_IN_PROGRESS;
  }

  // If we've reached here, the current tick is outside of the current interval,
  // meaning we need to finalize the current candle and start a new one

  // Set the old candle values
  candle->high = high;
  candle->low = low;
  candle->open = open;
  candle->volume = volume;
  candle->timestamp = timestamp;
  candle->close =
      tick->last_price; // This wasn't in your original code for this part

  // Reset for the new candle interval
  current_interval_start = tick->timestamp;
  high = tick->last_price;
  low = tick->last_price;
  open = tick->last_price;
  volume = tick->last_quantity;
  timestamp = tick->timestamp;

  return CANDLE_COMPLETED; // We've finalized a candle and returned it
}

char *candle_to_json(Candle *candle) {
  if (!candle)
    return NULL;
  size_t max_len = 200;
  char *json_string = (char *)malloc(max_len);
  if (!json_string)
    return NULL;
  snprintf(json_string, max_len,
           "{\"open\":%f,\"close\":%f,\"high\":%f,\"low\":%f,\"timestamp\":%ld,"
           "\"volume\":%f}",
           candle->open, candle->close, candle->high, candle->low,
           candle->timestamp, candle->volume);
  return json_string;
}

Candle* candle_copy(Candle candle){
Candle *out = malloc(sizeof(Candle));
  if(!out) return NULL;
  out->status = candle.status;
  out->timestamp = candle.timestamp;
  out->volume = candle.volume;
  out->open = candle.open;
  out->close = candle.close;
  out->high = candle.high;
  out->low = candle.low;
  return out;
}
