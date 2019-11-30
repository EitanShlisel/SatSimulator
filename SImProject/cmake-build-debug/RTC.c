
#ifndef RTC_H_
#define RTC_H_

// The only thing used from Time.h is the LocalTime structure
//#include <rpcndr.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#include <pthread.h>
#include "boolean.h"
#include "Time.h"
#include "SimulationConfigurations.h"
#include "RTC.h"

pthread_t rtc_thread_id;

struct timespec timestep = {.tv_sec = 0, .tv_nsec = RTC_TICK_TIME_PERIOD}; // this defines how long will a RTC tick will take(full cycle).
Boolean  rtc_on = FALSE;
unsigned int long clock_cycles = 0;
pthread_mutex_t lock;   // for synchronisations on 'clock_cycles'

void* RtcThread(void* a){

    while(rtc_on){
        pthread_mutex_lock(&lock);
        clock_cycles++;
        pthread_mutex_unlock(&lock);
        nanosleep(&timestep, NULL);
    }
    return NULL;
}

int RTC_start(void) {
    clock_cycles = 0;
    rtc_on = TRUE;
    if(0 != pthread_mutex_init(&lock,NULL)){
        return -1;
    }
    pthread_create(&rtc_thread_id,NULL,RtcThread,NULL);
    return 0;
}

void RTC_stop(void) {
    rtc_on = FALSE;
    pthread_mutex_destroy(&lock);
}

int RTC_setTime(Time *time) {
    if(!rtc_on){
        return -2;
    }
    if(NULL == time){
        return -1;
    }

    return 0;
}

int RTC_getTime(Time *time){
    if(!rtc_on){
        return -2;
    }
    return 0;
}

int RTC_testGetSet(void){
    if(!rtc_on){
        return -2;
    }
    return 0;
}

int RTC_testSeconds(void){
    if(!rtc_on){
        return -2;
    }
    return 0;
}

void RTC_printSeconds(void) {
    if(!rtc_on){
        printf("RTC not on\n");
    }
    printf("RTC = %lu\n",clock_cycles);
}

int RTC_checkTimeValid(Time *time){
    if(!rtc_on){
        return -2;
    }
    return 0;
}


int RTC_getTemperature(float *temperature) {
    if (!rtc_on) {
        return -2;
    }
    if (NULL == temperature){
        return -1;
    }
    *temperature = 42;
    return 0;
}
#endif /* RTC_H_ */