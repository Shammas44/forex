#ifndef REPORT_H
#define REPORT_H
#define T Report

typedef struct T T;

typedef struct Report_entry Report_entry;

typedef void (Report_destructor)(T *self);
typedef void (Report_add_entry)(T *self, Report_entry *entry);
typedef void (Report_erase)(T *self);
typedef void (Report_add_header)(T *self);

typedef struct T {
  Report_destructor *destructor;
  Report_add_entry *add_entry;
  Report_erase *erase;
  Report_add_header *add_header;
  void *__private;
} T;

#undef T
#endif
