
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimulationConfigurations.h"
#include "Time.h"
#include "RTC.h"
#define TRACE_ERROR(name,err)    if(0 != (err)){ printf("Error in %s = %d",#name,(err));}

int main() {
    int err = 20;
    Time time1;
    time_t curr_unix = 1573310314;
    struct tm * ptm;
    ptm = gmtime(&curr_unix);
    time1.seconds = ptm->tm_sec;
    time1.year =  ptm->tm_year;
    time1.month= ptm->tm_mon;
    time1.day = ptm->tm_mday;
    time1.hours = ptm->tm_hour;
    time1.minutes = ptm->tm_min;


    err = RTC_start();
    TRACE_ERROR(RTC_start,err)
    //pthread_delay_np()

    return 0;
}