#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H
#include <time.h>

typedef enum {
  RuntimeErrors_mode_default,
  RuntimeErrors_mode_silent,
} RuntimeErrors_mode;

extern RuntimeErrors_mode runtimeerrors_mode;

int runtimeError(const char *message, int error_code, const char *file, int line);

typedef struct {
  time_t timestamp;
  int code;
  const char *message;
  const char *file;
  int line;
} Error;

#define RUNTIME_ERROR(description, error_code)                                 \
  runtimeError(description, error_code, __FILE__, __LINE__)

#endif
