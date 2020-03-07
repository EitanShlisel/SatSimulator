
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimConfigFiles/SimulationConfigurations.h"
#include "SimFiles/TimeHelperFunctions.h"
#include "SubsystemModules/Time.h"
#include "SubsystemModules/RTC.h"
#include "SubsystemModules/GPS_Module.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimRTC.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimI2C.h"
#include "SimFiles/SimEPS.h"
#include "SimFiles/GenericHelpFunctions.h"
#include <stdbool.h>
#include <pthread_time.h>
#include <unistd.h>

EpsConsumptionState_t fake_subsys_states[] = {
        {true,5000,100},        //IDLE
        {true,5000,1000},       // full on consumption
        {true,3300,500}         // another state

};
void printPoint(point a){
    printf("{x = %lf,y = %lf, z = %lf}\n",a.x,a.y,a.z);
}
int main(){

}