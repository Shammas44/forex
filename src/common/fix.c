#include "fix.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void concat(char *str, const int param_num, ...);

char *compose_message(const PairKeyValue pairs[], const int pairs_size, const int exceptions[], const int exceptions_size) {

  int msg_size = 0;

  for (int i = 0; i < pairs_size; i++) {
    if (!in_exceptions(pairs[i].key, exceptions, exceptions_size)) {
      char key[3];
      sprintf(key, "%d", pairs[i].key);
      int value_len = strlen(pairs[i].value);
      msg_size += strlen(key) + value_len + 2; // 2 is for '=' and '\001'
    }
  }

  char body_len[5];
  int header_size = 13 + 5; // 5 is for body_len
  char *msg = malloc(sizeof(char) * msg_size + header_size);
  sprintf(body_len, "%d", msg_size);
  concat(msg, 3, "8=FIX",FIX_VERSION, "\001");
  concat(msg, 3, "9=", body_len, "\001");
  int checksum = get_checksum(msg);
  char checksum_str[20];
  sprintf(checksum_str, "%d", checksum);
  concat(msg, 3, "10=", checksum_str, "\001");

  for (int i = 0; i < pairs_size; i++) {
    char key[4];
    sprintf(key, "%d", pairs[i].key);
    concat(msg, 3, key, "=", pairs[i].value, "\001");
  }

  return msg;
}

int get_checksum(const char *msg) {
  int checksum = 0;
  for (unsigned long i = 0; i < strlen(msg); i++) {
    checksum += msg[i];
  }
  checksum %= 256;
  return checksum;
}

bool in_exceptions(int value, const int exceptions[], int num_exceptions) {
  for (int i = 0; i < num_exceptions; i++) {
    if (value == exceptions[i])  return true; 
  }
  return false;
}

void concat(char *str, const int param_num, ...) {
  va_list args;
  va_start(args, param_num);
  for (int i = 0; i < param_num; i++) {
    char *arg = va_arg(args, char *);
    strcat(str, arg);
  }
  va_end(args);
}
