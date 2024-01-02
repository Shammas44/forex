#include "EquityReport.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#define T EquityReport

typedef struct {
  char* report_name;
  Metadata*metadata;
} Private;

static void __destructor(T*self);
static void __add_entry(T*self, Report_entry*entry);
static void __erase(T*self);
static void __add_header(T*self);

static void _$add_new_line(char*filename,char*line);
static void _$erase_content(const char *filename);

T* equityReport_constructor(Metadata*metadata,char*report_name){
  T*self=malloc(sizeof(T));
  Private * private = malloc(sizeof(Private));
  private->metadata = metadata;
  char *copy = malloc(strlen(report_name)+1);
  strcpy(copy, report_name);
  private->report_name = copy;
  self->__private = private;
  self->destructor = __destructor;
  self->add_entry = __add_entry;
  self->erase = __erase;
  self->add_header = __add_header;
  return self;
}

static void __destructor(T*self){
  if(self==NULL){
    RUNTIME_ERROR("self is NULL",1);
    return;
  }
  Private * private = self->__private;
  char* report_name = private->report_name;
  if(private == NULL){
    RUNTIME_ERROR("private is NULL",1);
    free(self);
    return;
  } 
  free(private);
  free(report_name);
  free(self);
}

static void __add_entry(T*self, Report_entry*entry){
  Private * private = self->__private;
  Metadata *metadata = private->metadata;
  char* report_name = private->report_name;
  double equity = metadata->get_equity(metadata);
  char line[50];
  sprintf(line, "%f,%ld", equity, entry->timestamp);
  _$add_new_line(report_name, line);
}

static void __erase(T*self){
  Private * private = self->__private;
  char* report_name = private->report_name;
  _$erase_content(report_name);
}

static void __add_header(T*self){
  Private * private = self->__private;
  char* report_name = private->report_name;
  char line[50];
  sprintf(line, "equity,timestamp");
  _$add_new_line(report_name, line);
}

static void _$add_new_line(char*filename,char*line){
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        char* msg = malloc(strlen(filename)+30);
        sprintf(msg,"Failed to open report file '%s'",filename);
        RUNTIME_ERROR(msg,1);
        free(msg);
        return;
    }
    fprintf(file, "%s\n", line); 
    fclose(file);
}

static void _$erase_content(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        char* msg = malloc(strlen(filename)+30);
        sprintf(msg,"Failed to open report file '%s'",filename);
        RUNTIME_ERROR(msg,1);
        free(msg);
        return;
    }
    fclose(file);
}
#undef T

