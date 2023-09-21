#include <stdbool.h>

#define FIX_VERSION ".4.4"

typedef enum Tag {
  start = 8,
  body_len = 9,
  checksum = 10,
  msg_type = 35,
} Tag;

typedef struct PairKeyValue {
  int key;
  char *value;
} PairKeyValue;

char* compose_message(const PairKeyValue pairs[], const int pairs_size,const int exceptions[], const int exceptions_size); 

int get_checksum(const char *msg);

bool in_exceptions(int value, const int exceptions[], int num_exceptions);

