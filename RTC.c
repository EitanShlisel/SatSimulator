
#ifndef RTC_H_
#define RTC_H_

// The only thing used from Time.h is the LocalTime structure
//#include <rpcndr.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "boolean.h"
#include "Time.h"
#include "SimFiles/SimulationConfigurations.h"
#include "RTC.h"

pthread_t rtc_thread_id;
pthread_mutex_t lock;   // for synchronisations on 'clock_cycles'

clock_t begin_time;  // time at start of RTC;

Boolean  rtc_on = FALSE;
unsigned long long clock_cycles = 0;

void* RtcThread(void* a){
    begin_time = clock();
    int err = 0;
    while(rtc_on){
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