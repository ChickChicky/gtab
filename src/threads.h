#ifndef THREADS_H
#define THREADS_H

#include "common.h"

#ifdef _WIN32
    typedef struct {
        HANDLE thread;
    } threads_thread;
#else
    typedef struct {
        pthread_t thread;
    } threads_thread;
#endif

int threads_create(threads_thread *thread, void *(*target)(void *), void *args);
int threads_cancel(threads_thread *thread);
int threads_join(threads_thread *thread, void **result);

#endif
