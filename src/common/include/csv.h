#ifndef CSV_PARSING_H
#define CSV_PARSING_H
#include "candle.h"
#include "date.h"
#include "mtqueue.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define MAX_RECORDS 1000 // adjust based on expected records

// Function prototype for line parsing functions
typedef int (*RowParser)(const char *, void *);
typedef int (*RowParserForeach)(const char *, Mtqueue_list *queues);

void **csv_parse(const char *filename, int *record_count, size_t record_size, RowParser parser);

int csv_parse_foreach(const char *filename, Mtqueue_list *queues, RowParserForeach parser);

void csv_add_new_line(const char *filename, const char *line); 

void csv_erase_content(const char *filename);

#endif
