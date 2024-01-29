#include <netdb.h>
#include "url.h"
#include "isDestroyable.h"
#include <openssl/rand.h>
#ifndef SSLWRAPPER_H
#define SSLWRAPPER_H

#define T SslWrapper

extern SSL_CTX*ctx;

typedef struct T T;

struct T {
    IsDestroyable __destructor;
    void (*destructor)(T*self);
    SSL *(*get)(T*self);
    int (*connect)(T*self);
    int (*write)(T*self,char*payload,int payload_length);
    int (*read)(T*self,char**out,size_t*out_length);
    int (*get_last_error)(T*self);
    int (*cleanup)(T*self);
    SSL* (*new)(T*self);
    int (*set_context)(T*self);
    int (*set_host)(T*self,char*host);
    int (*set_fd)(T*self,int fd);
    int (*shutdown)(T*self);
    void*__private;
};

T* sslWrapper_constructor();
#undef T
#endif
