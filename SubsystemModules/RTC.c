#include <stdio.h>
#include <pthread.h>

#include "boolean.h"
#include "Time.h"

#include "RTC.h"
#include "../SimFiles/SimRTC.h"
#include "../SimFiles/TimeHelperFunctions.h"

Boolean  rtc_on = FALSE;

int RTC_start(void) {
    int err = SimRTC_Init();
    if (0 == err){
        rtc_on = TRUE;
    }
    return err;
}

void RTC_stop(void) {
#if(1 == RTC_USE_PRINTS)
    printf("STOPPING RTC");
#endif
    rtc_on = FALSE;
    SimRTC_StopRTC();
}

int RTC_setTime(Time *time) {
    if(!rtc_on){
        return -2;
    }
    if(NULL == time){
        return -1;
    }//
    atomic_time_t tm;
    tm = (atomic_time_t)TimeToUnix(time);
    SimRTC_SetSatTime(tm);
    return 0;
}

int RTC_getTime(Time *time){
    if(!rtc_on){
        return -2;
    }
    if(NULL == time){
        return -1;
    }
    atomic_time_t tm  = SimRtc_GetSatTime();
    UnixToTime(time,tm);
    SimRTC_SetSatTime(tm);
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
    printf("RTC = %llu\n",SimRTC_GetClockCycleCount());
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
