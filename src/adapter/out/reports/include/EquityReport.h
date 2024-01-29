#ifndef EQUITYREPORT_H
#define EQUITYREPORT_H
#include "Report.h"
#include "Metadata.h"
#include <time.h>

#define T EquityReport

typedef struct Report T;

struct Report_entry {
  double equity;
  time_t timestamp;
};

T *equityReport_constructor(Metadata *metadata, char*report_name);

#undef T
#endif
