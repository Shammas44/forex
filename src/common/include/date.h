#ifndef DATE_H
#define DATE_H
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

time_t iso8601_to_epoch(const char *timestamp);

struct tm *get_time();
int get_ms();
char* timestamp_to_string(time_t timestamp);
time_t date_to_timestamp(const char *dateString);
int time_to_timestamp(const char *dateString);
#endif 

