#include "buffer.h"
#include <stddef.h>
#include <stdlib.h>

char* buffer_init(size_t size) {
    char *response = malloc(size);
    if (!response) {
        return NULL;
    }
    return response;
}

