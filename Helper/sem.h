#ifndef SIMPROJECT_SEM_H
#define SIMPROJECT_SEM_H

typedef void* semaphore_t;

int semaphore_init(semaphore_t *sem, int pshared, unsigned int value);
int semaphore_destroy(semaphore_t *sem);
int semaphore_post(semaphore_t *sem);
int semaphore_wait(semaphore_t *sem);

#endif //SIMPROJECT_SEM_H
