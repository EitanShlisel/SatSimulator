#include <time.h>
#include <string.h>


#include "Time.h"
#include "boolean.h"
#include "RTC.h"
#include "../Helper/TimeHelperFunctions.h"
Boolean timeStarted = FALSE;        // has the 'Time' module been started

unsigned int sync_sec = 0;


int Time_start(Time *_time, unsigned int syncInterval){
    if(NULL == _time){
        return -1;
    }
    RTC_start();
    if(!RTC_checkTimeValid(_time)){
        RTC_setTime(_time);
    }else{
        return -2;
    }
    timeStarted = TRUE;
    return 0;
}

int Time_set(Time *_time){
    if(NULL == _time){
        return -1;
    }
    if(RTC_checkTimeValid(_time)){
        RTC_setTime(_time);
    }
    return 0;
}

int Time_setUnixEpoch(unsigned int epochTime){
    Time tm;
    UnixToTime(&tm,(time_t)epochTime);
    RTC_setTime(&tm);
    return 0;
}

int Time_get(Time *_time){
    return RTC_getTime(_time);
}

unsigned int Time_getUptimeSeconds(void){
    Time tm;
    RTC_getTime(&tm);
    return TimeToUnix(&tm);
}

int Time_getUnixEpoch(unsigned int *epochTime)
{
    if (!timeStarted) return 1;
    Time tm;
    RTC_getTime(&tm);
    *epochTime = (unsigned int)TimeToUnix(&tm);
    return 0;
}

int Time_sync(void)
{
    if (!timeStarted) return 1;
    return 0;
}

int Time_syncIfNeeded(void)
{
    return 0;
}

void Time_setSyncInterval(unsigned int seconds)
{
    sync_sec = seconds;
}

Boolean Time_isLeapYear(unsigned int year)
{
    if (year % 4 != 0) return FALSE;
    if (year % 100 != 0) return TRUE;
    if (year % 400 != 0) return FALSE;

    return TRUE;
}

unsigned int Time_diff(Time *newTime, Time *oldTime)
{
    time_t t_new = TimeToUnix(newTime);
    time_t t_old = TimeToUnix(oldTime);
    if (t_new < t_old) return -1;
    return (t_new - t_old);
}

Boolean Time_isRTCworking(void){
    return timeStarted;
}
