
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimulationConfigurations.h"
#include "Time.h"
#include "RTC.h"
#include "SimFiles/GPS_Module.h"
#include <pthread_time.h>

int main() {
    int err = 0;
    GPS_Test(1442);
    return 0;
}
