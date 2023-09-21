#include "movingaverage.h"
#include "candle.h"

double movingaverage(Slidingwindow *window) {
  double sum = 0;
  int count = 0;
  Slidingwindow_node *w = window->front;
  while (w != NULL) {
    count++;
    Candle *candle = (Candle *)w->data;
    double close = candle->close;
    sum += close;
    w = w->next;
  }
  if (count == 0) return 0;
  return sum / count;
}
