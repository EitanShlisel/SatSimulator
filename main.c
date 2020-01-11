
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
    SimSTK_Test(1442);
    return 0;
}
