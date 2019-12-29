
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimulationConfigurations.h"
#include "SubsystemModules/Time.h"
#include "SubsystemModules/RTC.h"
#include "SubsystemModules/GPS_Module.h"
#include "SimFiles/SimRTC.h"
#include <pthread_time.h>
#include <unistd.h>
int main() {
    SimRTC_Init();
    atomic_time_t prev,curr;
    prev = SimRTC_GetSimulationTime();
    while(1){
        curr = SimRTC_GetSimulationTime();
        printf("Current Simulation Time = %lf\t dt = %lf\n",curr,curr-prev);
        prev = curr;

        sleep(1);
    }
    return 0;
}
