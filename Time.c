#include "Time.h"
#include "boolean.h"
#include "RTC.h"
#include <time.h>

Boolean timeStatred = FALSE;        // has the 'Time' module been started
time_t sat_time = 0;                // unix time of start(given by user)
//time_t real_start_time = 0;
unsigned int sync_sec = 0;

int UnixToTime(Time *_time){

    return 0;
}

time_t TimeToUnix(Time *_time)
{
    struct tm curr_date;
    curr_date.tm_year = _time->year + 100 ; // number of years since 2000, where year = 0,1,...
    curr_date.tm_mon = _time->month - 1;
    curr_date.tm_mday = _time->date;
    curr_date.tm_wday = _time->day - 1;
    curr_date.tm_hour = _time->hours + 2;
    curr_date.tm_min = _time->minutes;
    curr_date.tm_sec = _time->seconds;
    curr_date.tm_isdst = -1; // no dailight saving

    return mktime(&curr_date);
}

int Time_start(Time *_time, unsigned int syncInterval){
    if(NULL == _time){
        return -1;
    }
    RTC_start();
    if(RTC_checkTimeValid(_time)){
        RTC_setTime(_time);
    }else{
        return -2;
    }
    sat_time = TimeToUnix(_time);
    timeStatred = TRUE;
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

    sat_time = epochTime;
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
    if (!timeStatred) return 1;
    time_t uptime = sat_time + Time_getUptimeSeconds();
    return 0;
}

int Time_sync(void)
{
    if (!timeStatred) return 1;
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

Boolean Time_isRTCworking(void)
{
    return timeStatred;
}
