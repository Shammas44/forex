#include <netdb.h>
#include "url.h"
#include "isDestroyable.h"
#include <openssl/rand.h>
#ifndef SSLWRAPPER_H
#define SSLWRAPPER_H

#define T SslWrapper

// extern SSL_CTX *ctx;
// SSL_CTX *ctx = NULL;
extern SSL_CTX*ctx;

typedef struct T T;

struct T {
    IsDestroyable __destructor;
    void (*destructor)(T*self);
    int (*connect)(T*self,SSL*ssl);
    int (*write)(T*self,SSL*ssl,char*payload,int payload_length);
    int (*read)(T*self,SSL*ssl,char**out,size_t*out_length);
    int (*get_last_error)(T*self);
    int (*cleanup)(T*self);
    SSL* (*new)(T*self,SSL_CTX*ctx);
    int (*set_context)(T*self);
    long (*set_host)(T*self,SSL*ssl,char*host);
    int (*set_fd)(T*self,SSL*ssl,int fd);
    int (*shutdown)(T*self, SSL*ssl);
    void*__private;
};

T* sslWrapper_constructor();
#undef T
#endif
