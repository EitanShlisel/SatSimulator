
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

void EPS_Test(){
    EpsConsumptionState_t fake_subsys_states[] = {
            {0,5000, false},
            {1,1000, false},
            {2,420, false}
    };

    int err = 0;
    err = SimRTC_Init();
    TRACE_ERROR(SimRTC_Init,err);
    err = SimEPS_AddConsumptionStates(SUBSYS_FAKE,fake_subsys_states,3);
    TRACE_ERROR(SimEPS_AddConsumptionStates,err);
    err = SimEPS_StartEps();
    TRACE_ERROR(SimEPS_StartEps,err);
    printStates(SUBSYS_FAKE);
    int i = 0;
    double volt = SimEPS_GetBatteryVoltage();
    while(true){

        printf("vbatt = %.04lf\n",volt);
        printChannels();
        volt = SimEPS_GetBatteryVoltage();
        if(i ==3){
            SimEPS_SetChannel(0,true);
            SimEPS_SetSubsysState(SUBSYS_FAKE,0,true);
            printStates(SUBSYS_FAKE);
        }
        if(i == 6){
            SimEPS_SetChannel(1,true);
            SimEPS_SetSubsysState(SUBSYS_FAKE,1,true);
            printStates(SUBSYS_FAKE);
        }
        if(i==9){
            SimEPS_SetChannel(2,true);
            SimEPS_SetSubsysState(SUBSYS_FAKE,2,true);
            printStates(SUBSYS_FAKE);
        }
        sleep(1);
        i++;
    }
}

int main(){

}