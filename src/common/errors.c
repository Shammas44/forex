#include "error.h"
#include <openssl/err.h>
#define GETSOCKETERRNO() (errno)

int get_error(char*msg){
  fprintf(stderr, "%s (%d)\n",msg, GETSOCKETERRNO());
  return EXIT_FAILURE;
}
