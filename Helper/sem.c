#include "sem.h"
#include <windows.h>
#define MAX_SEM_COUNT 1024
int semaphore_init(semaphore_t *sem, int pshared, unsigned int value){
    *sem = CreateSemaphore(
            NULL,           // default security attributes
            value,  // initial count
            MAX_SEM_COUNT,  // maximum count
            NULL);          // unnamed semaphore

    if (*sem == NULL){
        return GetLastError();
    }
    return 0;
}
int semaphore_destroy(semaphore_t *sem){
    CloseHandle(*sem);
    return GetLastError();
}
int semaphore_post(semaphore_t *sem){
    BOOL res;
    res = ReleaseSemaphore(*sem,1,NULL);
    if(!res){
        return GetLastError();
    }
    return 0;
}
int semaphore_wait(semaphore_t *sem){
    DWORD dwWaitResult;
    dwWaitResult = WaitForSingleObject(*sem,INFINITE);
    if(WAIT_OBJECT_0 == dwWaitResult){
        return 0;
    }
    return dwWaitResult;
}