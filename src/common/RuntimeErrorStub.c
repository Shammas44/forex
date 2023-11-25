#include "RuntimeErrorStub.h"
#include <stdio.h>
static const char *message = "No Error";
static int parameter = -1;
static const char *file = 0;
static int line = -1;

void runtimeErrorStub_reset(void) {
  message = "No Error";
  parameter = -1;
}

const char *runtimeErrorStub_get_last_error(void) { return message; }

void runtimeError(const char *m, int p, const char *f, int l) {
  static char buffer[256]; 
  sprintf(buffer, "Error: %s, parameter: %d, file: %s, line: %d", m, p, f, l);
  message = buffer;
  parameter = p;
  file = f;
  line = l;
}

int runtimeErrorStub_get_last_parameter(void) { return parameter; }
