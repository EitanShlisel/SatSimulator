#ifndef SIMPROJECT_TIMEHELPERFUNCTIONS_H
#define SIMPROJECT_TIMEHELPERFUNCTIONS_H

#include <time.h>
#include "../SubsystemModules/Time.h"

int UnixToTime(Time *_time,time_t tim);

time_t TimeToUnix(Time *_time);

//"Jan" = 0,"Feb" = 1,"Mar" = 2,"Apr" = 3,"May" = 4,"Jun" = 5,
// "Jul" = 6,"Aug" = 7,"Sep" = 8,"Oct" = 9,"Nov" = 10,"Dec" = 11
int monthToInt(char *month);

//18 Nov 2019 10:00:00.000
// format: "dd Mon yyyy hh:mm:ss:lll "
// dd = day of month
// Mon = Month abbreviation
// yyyy = numeric year
// hh = hour in 24h format
// mm = minuets
// ss = seconds
// lll = milliseconds
double parseTime(char *line);
#endif //SIMPROJECT_TIMEHELPERFUNCTIONS_H
