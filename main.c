
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimulationConfigurations.h"
#include "Time.h"
#include "RTC.h"

#include <pthread_time.h>

int main() {
    int err = 20;
    err = RTC_start();
    TRACE_ERROR(RTC_start,err)
    //pthread_delay_np()
    while(TRUE){

    }
    return 0;
}
