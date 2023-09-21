#include "date.h"

time_t iso8601_to_epoch(const char *timestamp) {
  struct tm t;
  if (sscanf(timestamp, "%d-%d-%dT%d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday,
             &t.tm_hour, &t.tm_min, &t.tm_sec) != 6) {
    return -1;
  }
  // Adjustments for struct tm expectations
  // t.tm_year -= 1900; // tm_year is since 1900
  // t.tm_mon -= 1;     // January is 0 in struct tm
  // t.tm_isdst = -1;   // No Daylight Saving Time information provided
  return mktime(&t);
}

char *timestamp_to_string(time_t timestamp) {
  struct tm *local_time;
  static char str[20]; // "YYYY-MM-DD HH:MM:SS" + '\0'
  local_time = localtime(&timestamp);
  // Format it as: "YYYY-MM-DD HH:MM:SS"
  strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", local_time);
  return str;
}

time_t date_to_timestamp(const char *dateString) {
  struct tm tm = {0};
  // Format it as: "MM/DD/YYYY"
#ifdef __APPLE__
  if (strptime(dateString, "%m/%d/%Y", &tm) == NULL) {
    fprintf(stderr, "Failed to parse date: %s\n", dateString);
    return -1;
  }
#else
  int ret = sscanf(dateString, "%d/%d/%d", 
                   &(tm.tm_mon), &(tm.tm_mday), &(tm.tm_year));

  if (ret != 3) {
    fprintf(stderr, "Failed to parse date: %s\n", dateString);
    return -1;
  }
#endif

  // tm.tm_mon -= 1; // Adjust month (0-based)
  //   tm.tm_year -= 1900; // Adjust year (1900-based)

  time_t timestamp = mktime(&tm);
  return timestamp;
}

int time_to_timestamp(const char *timeString) {
  struct tm tm = {0}; // Initialize all fields to zero
  int hour, min, sec;

#ifdef __APPLE__
  if (strptime(timeString, "%H:%M:%S", &tm) == NULL) {
    fprintf(stderr, "Failed to parse time: %s\n", timeString);
    return (time_t)-1; // Return an error value
  }
  hour = tm.tm_hour * 3600;
  min = tm.tm_min * 60;
  sec = tm.tm_sec;
  return hour + min + sec;
#else
  int ret = sscanf(timeString, "%d:%d:%d", &hour, &min, &sec);

  if (ret != 3) {
    fprintf(stderr, "Failed to parse time: %s\n", timeString);
    return -1; // Return an error value
  }
  int total_seconds = hour * 3600 + min * 60 + sec;
  return total_seconds;
#endif
}

struct tm *get_time() {
  time_t rawtime;
  struct tm *current_time;
  char buffer[80];
  time(&rawtime);
  current_time = localtime(&rawtime);
  return current_time;
}

int get_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int milliseconds_of_current_second = tv.tv_usec / 1000;
  return milliseconds_of_current_second;
}
