#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TimeHelperFunctions.h"

int UnixToTime(Time *_time,time_t tim){

    struct tm epoch_time;
    time_t seconds = tim;
    memcpy(&epoch_time, localtime(&seconds), sizeof (struct tm));
    _time->minutes = epoch_time.tm_min;

    _time->seconds = epoch_time.tm_sec; ///< Seconds of the minute.
    _time->minutes = epoch_time.tm_min; ///< Minutes in the hour.
    _time->hours = epoch_time.tm_hour; ///< Hour of the day.
    _time->day = epoch_time.tm_wday + 1;  ///< Day of the week (Sunday=1, Monday=2, ..., Saturday=7)
    _time->date = epoch_time.tm_mday; ///< Date of the month.
    _time->month = epoch_time.tm_mon + 1; ///< Month of the year (January = 1).
    _time->year = epoch_time.tm_year - 100; ///< Year since 2000 (2000 = 0).
    return 0;
}

time_t TimeToUnix(Time *_time)
{
    struct tm curr_date;
    curr_date.tm_year = _time->year + 100 ; // number of years since 2000, where year = 0,1,...
    curr_date.tm_mon = _time->month - 1;
    curr_date.tm_mday = _time->date;
    curr_date.tm_wday = _time->day - 1;
    curr_date.tm_hour = _time->hours;
    curr_date.tm_min = _time->minutes;
    curr_date.tm_sec = _time->seconds;
    curr_date.tm_isdst = -1; // no dailight saving

    return mktime(&curr_date);
}

void PrintTime(Time *_time){
    printf("seconds  = %d\n",_time->seconds);
    printf("minutes  = %d\n",_time->minutes);
    printf("hours  = %d\n",_time->hours);
    printf("day  = %d\n",_time->day);
    printf("date  = %d\n",_time->date);
    printf("month  = %d\n",_time->month);
    printf("year  = %d\n",_time->year);
}

int monthToInt(char *month){
    if(NULL == month){
        return -2;
    }
    char* month_arr[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    for (int i = 0; i < (int)(sizeof(month_arr)/ sizeof(month_arr[0])); ++i) {
        if(0 == strcmp(month,month_arr[i])){
            return i;
        }
    }
    return -1;
}
double parseTime(char *line){
    //18 Nov 2019 10:00:00.000
    double unix_time = 0;
    double milli_sec = 0;
    struct tm tim;
    char split_char[] = " ,:.";

    char *ptr = strtok(line, split_char); // day of the month(0-31)
    tim.tm_mday = atoi(ptr);

    ptr = strtok(NULL, split_char);   // month (0-11)
    tim.tm_mon = monthToInt(ptr);

    ptr = strtok(NULL, split_char);   // year
    tim.tm_year = atoi(ptr) - 1900;

    ptr = strtok(NULL, split_char);   // hour (0-23)
    tim.tm_hour = atoi(ptr) + 2;

    ptr = strtok(NULL, split_char);   // minute(0-59)
    tim.tm_min = atoi(ptr);

    ptr = strtok(NULL, split_char);   // sec(0-61)
    tim.tm_sec = atoi(ptr);

    ptr = strtok(NULL, split_char);   // millisec(0-999)
    milli_sec = (double)(atoi(ptr))/1000.0;
    //"1 Jan 2000 1:02:03.456,-1894.182666,5879.877177,3982.272531,-7.648021,-1.420840,0.192274";
    tim.tm_isdst = -1;
    unix_time = (double)mktime(&tim) + milli_sec;
    return unix_time;
}
