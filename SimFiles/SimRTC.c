#include "SimRTC.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

#include "../SimFiles/SimulationConfigurations.h"

pthread_t rtc_thread_id;
_sigset_t sig = SIG_DFL;
pthread_mutex_t lock;   // for synchronisations on 'clock_cycles'
#if(1 ==  RTC_USE_PRINTS)
    clock_t begin_time;  // time at start of RTC;
#endif

atomic_time_t sat_time = RTC_DEFAULT_START_TIME;
unsigned long long clock_cycles = 0;


// returns how many clock cycles have passed since start of RTC
unsigned long long SimRTC_GetClockCycleCount(){
    return clock_cycles;
}

// returns the real time of the simulation and not of the satellite
atomic_time_t  SimRTC_GetSimulationTime(){
    return (RTC_SIMULATION_START_TIME+ (atomic_time_t)(SimRTC_GetClockCycleCount())/RTC_TICK_TIME_PERIOD );
}

//returns the time the satellite thinks it is
atomic_time_t SimRtc_GetSatTime(){
    return (sat_time + (atomic_time_t)(SimRTC_GetClockCycleCount()) / RTC_TICK_TIME_PERIOD);
}

void* SimRtcThread(void* a){
#if(1 == RTC_USE_PRINTS)
    begin_time = clock();
#endif
    int err = 0;

    while(1){
    if(SIGTERM == sig){
        pthread_exit((void*)(1));
        return NULL;
    }
        pthread_mutex_lock(&lock);
            clock_cycles++;
        pthread_mutex_unlock(&lock);
        usleep(RTC_TICK_TIME_PERIOD);

#if(1 == RTC_USE_PRINTS)
        printf("runtime = %f\n", ((double)(clock() - begin_time) / CLOCKS_PER_SEC));
        printf("clock_cycles42 = %llu\n",clock_cycles);
#endif
    }
    return NULL;
}

int SimRTC_Init(){
#if(1 == RTC_USE_PRINTS)
    printf("RTC INIT\n");
#endif
    clock_cycles = 0;
    if(0 != pthread_mutex_init(&lock,NULL)){
        return -1;
    }
    pthread_create(&rtc_thread_id, NULL, SimRtcThread, NULL);
}
int SimRTC_StopRTC(){
    pthread_mutex_destroy(&lock);
    int err = pthread_cancel(rtc_thread_id);
    TRACE_ERROR(pthread_cancel,err);
    err = pthread_kill(rtc_thread_id,SIGTERM);
    TRACE_ERROR(pthread_kill,err);
    sig = SIGTERM;
    return err;

}
int SimRTC_SetSatTime(atomic_time_t time){
    pthread_mutex_lock(&lock);
        sat_time = time;
    pthread_mutex_unlock(&lock);
}