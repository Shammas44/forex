#include "candleprod.h"

typedef struct {
  char date[11]; // "1/27/2015\0"
  char time[9];  // "13:29:00\0"
  double open, high, low, close;
  int upVolume, downVolume, totalVolume;
  int upTicks, downTicks, totalTicks;
} Record;

void print_record(Record *r) {
  printf("\nDate: %s\nTime: %s\nOpen: %lf", r->date, r->time, r->open);
  printf("\nHigh: %lf\nLow: %lf\nClose: %lf", r->high, r->low, r->close);
  printf("\nUpVolume: %d\nDownVolume: %d", r->upVolume, r->downVolume);
  printf("\nTotalVolume: %d\nUpTicks: %d", r->totalVolume, r->upTicks);
  printf("\nDownTicks: %d\nTotalTicks: %d\n", r->downTicks, r->totalTicks);
}

void *candleprod_produce(void *arg) {
  Mtqueue_list *queues = (Mtqueue_list *)arg;

  int capacity = 10;
  Candle *candle = NULL;
  int current_sec = -1;
  bool isFirst = true;
  while (1) {
    void *data = mtqueue_dequeue(queues->ticks);
    Tick *tick = (Tick *)data;
    if (candle == NULL)
      candle = malloc(sizeof(Candle));
    if (candle_create(candle, tick, 5) == CANDLE_COMPLETED) {
      mtqueue_enqueue(queues->bars, &candle);
      candle = NULL;
    };
    free(tick);
  }
  return NULL;
}

void *candleprod_produce_from_file(void *arg) {
  Mtqueue_list *queues = (Mtqueue_list *)arg;
  char *file = tsmetadata_get_backtest_data(queues->metadata);
  int status = csv_parse_foreach(file, queues, candleprod_from_csv_row);
  if (status) {
    printf("Failed to parse csv\n");
  }

  Candle *stop = malloc(sizeof(Candle));
  stop->status = CANDLE_STOP;
  mtqueue_enqueue(queues->bars, stop);
  return NULL;
}

int candleprod_from_csv_row(const char *line, Mtqueue_list *queues) {
  static int count = 1;
  Record r;
  sscanf(line, "%10[^,],%8[^,],%lf,%lf,%lf,%lf,%d,%d,%d,%d,%d,%d", r.date,
         r.time, &r.open, &r.high, &r.low, &r.close, &r.upVolume, &r.downVolume,
         &r.totalVolume, &r.upTicks, &r.downTicks, &r.totalTicks);

  char datetime_str[20];
  snprintf(datetime_str, sizeof(datetime_str), "%s %s", r.date, r.time);

  struct tm tm;
  time_t epoch_time;

#ifdef __APPLE__
  if (strptime(datetime_str, "%m/%d/%Y %H:%M:%S", &tm) != NULL) {
    epoch_time = mktime(&tm);
    if (epoch_time == -1) {
      fprintf(stderr, "Error converting to time_t\n");
      return -1;
    }
  } else {
    fprintf(stderr, "Error parsing the date-time string\n");
    return -1;
  }
#else
  if (sscanf(datetime_str, "%d/%d/%d %d:%d:%d", &tm.tm_mon, &tm.tm_mday,
             &tm.tm_year, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
    tm.tm_year -= 1900; // Adjust year to be in years since 1900
    tm.tm_mon -= 1;     // Adjust month to be in [0, 11]
    tm.tm_isdst = -1;
    epoch_time = mktime(&tm);

    if (epoch_time == -1) {
      fprintf(stderr, "Error converting to time_t\n");
      return -1;
    }
  } else {
    fprintf(stderr, "Error parsing the date-time string\n");
    return -1;
  }

#endif
  Candle *candle = malloc(sizeof(Candle));
  if (candle != NULL) {
    candle->timestamp = epoch_time;
    candle->status = CANDLE_FILLED;
    candle->high = r.high;
    candle->low = r.low;
    candle->open = r.open;
    candle->close = r.close;
    mtqueue_enqueue(queues->bars, candle);
    count++;
    return 0;
  }
  printf("Unable to allocate memory for candle\n");
  return 0;
}
