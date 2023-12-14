#ifndef ERROR_H
#define ERROR_H

int get_error(char*msg);

#define FREE(ptr) do { \
    if (ptr != NULL) { \
        free(ptr); \
        ptr = NULL; \
    } \
} while (0)

#endif
