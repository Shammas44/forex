#include "Strategy_trend_following.h"
#include "common.h"
#include "slidingwindow.h"

#define SMALL_SIZE 5
#define LARGE_SIZE 20
#define EQUITY_FILE "equity.csv"

#define T Strategy_trend_following

typedef struct {
  Metadata*metadata;
  bool first_run;
} Private;

static void __destructor(T*self);
static Order* __execute(T*self, CandleWrapper*candle);

T* strategy_trend_following_constructor(Metadata*metadata){
  if(metadata==NULL){
    RUNTIME_ERROR("metadata is NULL",1);
    return NULL;
  }
  T*self=malloc(sizeof(T));
  Private * private = malloc(sizeof(Private));
  private->first_run = true;
  private->metadata = metadata;
  self->__private = private;
  self->destructor = __destructor;
  self->execute = __execute;
  return self;
}

static void __destructor(T*self){
  if(self==NULL){
    RUNTIME_ERROR("self is NULL",1);
    return;
  }
  free(self->__private);
  free(self);
}

static Order* __execute(T*self, CandleWrapper*candle){
  // static Slidingwindow *w_small;
  // static Slidingwindow *w_large;
  // static int count = 0;

  // int small_size = SMALL_SIZE;
  // int large_size = LARGE_SIZE;

  // if(candle->status(candle) == CANDLE_STOP){
  //   slidingwindow_free(w_small);
  //   slidingwindow_free(w_large);
  //   return NULL;
  // }

//   Candle *candle_a = candle_copy(candle);
//   if (candle_a == NULL) {
//     fprintf(stderr, "Memory allocation failed\n");
//     exit(EXIT_FAILURE);
//   }
//   Candle *candle_b = candle_copy(candle);
//   if (candle_b == NULL) {
//     fprintf(stderr, "Memory allocation failed\n");
//     exit(EXIT_FAILURE);
//   }

//   if (count == 0) {
//     w_small = malloc(sizeof(Slidingwindow));
//     w_large = malloc(sizeof(Slidingwindow));

//     if (w_small == NULL || w_large == NULL) {
//       fprintf(stderr, "Memory allocation failed\n");
//       exit(EXIT_FAILURE);
//     }
//     slidingwindow_init_from_null(w_small, small_size);
//     slidingwindow_init_from_null(w_large, large_size);
//   }

//   double close = candle.close;

//   Order *order = malloc(sizeof(Order));
//   order->limit = 0;
//   order->size = 0;
//   order->id = -1;
//   order->price = close;
//   order->status = ORDER_CANCELLED;
//   sprintf(order->type, "%s", "Market");

//   int market_position = (int)tsmetadata_get_market_position(metadata);
//   if (count < large_size)
//     count++;

//   slidingwindow_add(w_small, candle_a);
//   slidingwindow_add(w_large, candle_b);

//   double ma_small = movingaverage(w_small);
//   double ma_large = movingaverage(w_large);

//   if (count < large_size)
//     goto end;

//   if (close < ma_small && ma_small < ma_large && market_position >= 0) {
//     order->side = SELL;
//     order->status = ORDER_PENDING;
//     if (market_position == 0) {
//       order->size = 10000;
//     } else {
//       order->size = 20000;
//     }
//   }

//   if (close > ma_small && ma_small > ma_large && market_position <= 0) {
//     order->side = BUY;
//     order->status = ORDER_PENDING;
//     if (market_position == 0) {
//       order->size = 10000;
//     } else {
//       order->size = 20000;
//     }
//   }

// end:
//   strategy_trend_following_report(metadata, candle, ma_small, ma_large);
//   return order;
return NULL;
}

#undef T
#undef EQUITY_FILE
#undef SMALL_SIZE
#undef LARGE_SIZE

// Order *strategy_trend_following(Tsmetadata *metadata, Candle candle) {
//   static Slidingwindow *w_small;
//   static Slidingwindow *w_large;
//   static int count = 0;

//   int small_size = SMALL_SIZE;
//   int large_size = LARGE_SIZE;

//   if(candle.status == CANDLE_STOP){
//     slidingwindow_free(w_small);
//     slidingwindow_free(w_large);
//     return NULL;
//   }

//   Candle *candle_a = candle_copy(candle);
//   if (candle_a == NULL) {
//     fprintf(stderr, "Memory allocation failed\n");
//     exit(EXIT_FAILURE);
//   }
//   Candle *candle_b = candle_copy(candle);
//   if (candle_b == NULL) {
//     fprintf(stderr, "Memory allocation failed\n");
//     exit(EXIT_FAILURE);
//   }

//   if (count == 0) {
//     w_small = malloc(sizeof(Slidingwindow));
//     w_large = malloc(sizeof(Slidingwindow));

//     if (w_small == NULL || w_large == NULL) {
//       fprintf(stderr, "Memory allocation failed\n");
//       exit(EXIT_FAILURE);
//     }
//     slidingwindow_init_from_null(w_small, small_size);
//     slidingwindow_init_from_null(w_large, large_size);
//   }

//   double close = candle.close;

//   Order *order = malloc(sizeof(Order));
//   order->limit = 0;
//   order->size = 0;
//   order->id = -1;
//   order->price = close;
//   order->status = ORDER_CANCELLED;
//   sprintf(order->type, "%s", "Market");

//   int market_position = (int)tsmetadata_get_market_position(metadata);
//   if (count < large_size)
//     count++;

//   slidingwindow_add(w_small, candle_a);
//   slidingwindow_add(w_large, candle_b);

//   double ma_small = movingaverage(w_small);
//   double ma_large = movingaverage(w_large);

//   if (count < large_size)
//     goto end;

//   if (close < ma_small && ma_small < ma_large && market_position >= 0) {
//     order->side = SELL;
//     order->status = ORDER_PENDING;
//     if (market_position == 0) {
//       order->size = 10000;
//     } else {
//       order->size = 20000;
//     }
//   }

//   if (close > ma_small && ma_small > ma_large && market_position <= 0) {
//     order->side = BUY;
//     order->status = ORDER_PENDING;
//     if (market_position == 0) {
//       order->size = 10000;
//     } else {
//       order->size = 20000;
//     }
//   }

// end:
//   strategy_trend_following_report(metadata, candle, ma_small, ma_large);
//   return order;
// }

// void strategy_trend_following_report(Tsmetadata *m, Candle candle, double ma_small, double ma_large) {
//   static bool first_run = true;
//   if (first_run) {
//     first_run = false;
//     csv_erase_content(EQUITY_FILE);
//     csv_add_new_line(EQUITY_FILE, "equity,timestamp,ma_small,ma_large,price");
//   }else {
//   double equity = tsmetadata_get_equity(m);
//   char line[50];
//   double close = candle.close;
//   time_t timestamp = candle.timestamp;
//   sprintf(line, "%f,%ld,%f,%f,%f", equity, timestamp, ma_small, ma_large,
//           close);
//   csv_add_new_line(EQUITY_FILE, line);
//   }
// }
