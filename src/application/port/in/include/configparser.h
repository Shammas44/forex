#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include "Parser.h"
#include "file.h"
#include "tsmetadata.h"
#include "Strategy.h"
#include "mtqueue.h"

void configparser_init(const char *path, Tsmetadata *metadata);
#endif 
