#include "threads.h"

#include <windows.h>
#include <synchapi.h>
#include <stdbool.h>

 int thread_mutex_init(thread_mutex_t *mutex, void *restrict attr){

    *mutex = CreateMutex(
            attr,
            FALSE,
            NULL);

    if(NULL == mutex){
        return GetLastError();
    }
    return 0;
}
int thread_mutex_destroy(thread_mutex_t *mutex){
   return CloseHandle(*mutex);
}
int thread_terminate(thread_handle_t thread){
    return TerminateThread(thread,0);
}
void thread_exit(int *err_code){
    ExitThread(*err_code);
}
int thread_create(thread_handle_t *thread, void *attr,
                                           void *(*start_routine) (void *), void *arg){
    DWORD dwThreadIdArray;
    *thread = CreateThread(
            attr,                  // default security attributes
            0,         // use default stack size
            (LPTHREAD_START_ROUTINE)start_routine,         // thread function name
            arg,                   // argument to thread function
            0,      // use default creation flags
            &dwThreadIdArray);;
    return GetLastError();

}
int thread_mutex_lock(thread_mutex_t mutex){
   return WaitForSingleObject(mutex,0xFFFF);
}
int mutex_lock_timed(thread_mutex_t mutex, unsigned int millisec){
    return WaitForSingleObject(mutex,millisec);
}
int thread_mutex_unlock(thread_mutex_t mutex){
   return ReleaseMutex(mutex);
}
