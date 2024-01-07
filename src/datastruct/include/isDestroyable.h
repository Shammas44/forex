#ifndef ISDESTROYABLE_H
#define ISDESTROYABLE_H
#define T IsDestroyable

typedef struct T {
  void * destructor;
} T;

#undef T
#endif
