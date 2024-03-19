#ifndef USECASE_H
#define USECASE_H
#include "IsDestroyable.h"
#define T Usecase

typedef struct T T;

typedef struct UsecaseReturn UsecaseReturn;
typedef struct UsecaseArgs UsecaseArgs;

typedef struct T {
    IsDestroyable __destructor;
    UsecaseReturn (*execute)(T *self,UsecaseArgs input);
    void (*destructor)(T *self);
    void*__private;
} T;

#undef T
#endif
