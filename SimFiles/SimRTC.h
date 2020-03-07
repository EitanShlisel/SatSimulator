#ifndef SIMPROJECT_SIMRTC_H
#define SIMPROJECT_SIMRTC_H

#include "../SubsystemModules/GPS_types.h"


// returns how many clock cycles have passed since start of RTC
unsigned long long SimRTC_GetClockCycleCount();

// returns the real time outside of the simulation
atomic_time_t  SimRTC_GetSimulationTime();

//returns the time the satellite thinks it is
atomic_time_t SimRTC_GetSatTime();

int SimRTC_Init();

int SimRTC_StopRTC();//    pthread_mutex_destroy(&lock);

int SimRTC_SetSatTime(atomic_time_t time);


void SimRTC_Test();
#endif //SIMPROJECT_SIMRTC_H
