#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#define THREADS 12

typedef struct s_context t_context;
typedef struct s_region t_region;
typedef pthread_mutex_t t_mutex;

void* renderthread(void* arg);
void transferimg(t_context* ctx, const t_region* region);

#endif
