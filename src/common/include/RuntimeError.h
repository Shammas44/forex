#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

void runtimeError(const char*message, int parameter,const char*file,int line);

#define RUNTIME_ERROR(description,parameter)\
  runtimeError(description,parameter,__FILE__,__LINE__)

#endif 
