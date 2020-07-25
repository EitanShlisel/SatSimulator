#ifndef SIMPROJECT_THREADS_H
#define SIMPROJECT_THREADS_H

#include "../Helper/sem.h"

typedef void* thread_mutex_t;
typedef void* thread_handle_t;

int thread_create(thread_handle_t *thread, void *attr,
                  void *(*start_routine) (void *), void *arg);
int thread_terminate(thread_handle_t thread);
void thread_exit(int *err_code);
int thread_mutex_destroy(thread_mutex_t *mutex);
int thread_mutex_init(thread_mutex_t *mutex, void *restrict attr);
int thread_mutex_lock(thread_mutex_t mutex);
int thread_mutex_unlock(thread_mutex_t mutex);

#endif //SIMPROJECT_THREADS_H
