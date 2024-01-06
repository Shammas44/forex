#include "RuntimeErrorImp.h"
#include <stdio.h>
#include <time.h>

char *filename = "log.txt";
static Error error = (Error){.timestamp = 0, .message = "No error", .code = 0, .file = 0, .line = 0};
char* start = "\x1b[31m";
char* end = "\x1b[0m";
RuntimeErrors_mode runtimeerrors_mode = RuntimeErrors_mode_default;

void runtimeError_reset(void) {
  error = (Error){.timestamp = 0, .message = "No error", .code = 0, .file = 0, .line = 0};
}

Error runtimeError_get_last_error(void) { return error; }

int runtimeError(const char *m, int p, const char *f, int l) {
  time_t t;
  time(&t);
  static char buffer[256];

  sprintf(buffer, "%s:%d; %s; %ld; %d", f,l, m, t, p );
  if(runtimeerrors_mode == RuntimeErrors_mode_default){
    printf("%s%s%s\n",start,buffer,end);
  }
  error = (Error){.timestamp = t,.message = m, .code = p, .file = f, .line = l};
  
  FILE *file = fopen(filename, "a");
  if (file == NULL) {
    perror("Failed to open log file");
    return p;
  }
  fprintf(file, "%s\n", buffer); 
  fclose(file);
  return p;
}

int runtimeError_get_last_parameter(void) { return error.code; }
