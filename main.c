
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimConfigFiles/SimulationConfigurations.h"
#include "Helper/TimeHelperFunctions.h"
#include "Helper/List.h"
#include "SubsystemModules/Time.h"
#include "SubsystemModules/RTC.h"
#include "SubsystemModules/GPS_Module.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimRTC.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimI2C.h"
#include "SimFiles/SimEPS.h"
#include "SimFiles/SimThermodynamics.h"
#include "Helper/GenericHelpFunctions.h"
#include <stdbool.h>
#include <pthread_time.h>
#include <unistd.h>
#include <math.h>
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
void ListTest(){
    unsigned char c1[] = {0};
    unsigned char c2[] = {1,2};
    unsigned char c3[] = {3,4,5};
    unsigned char c4[] = {6,7,8,9};
    unsigned char c5[] = {42};

    list_t *ptr;
    list_t *lst1 = ListHelper_CreateList(c1,sizeof(c1));
    list_t *lst2 = ListHelper_CreateList(c2,sizeof(c2));
    list_t *lst3 = ListHelper_CreateList(c3,sizeof(c3));
    list_t *lst4 = ListHelper_CreateList(c4,sizeof(c4));
    list_t *lst5 = ListHelper_CreateList(c5,sizeof(c5));
    ListHelper_Add(lst1,lst2);
    printf("added new item to list\n");
    ListHelper_PrintList(lst1,NULL);
    ListHelper_Add(lst1,lst3);
    printf("added new item to list\n");
    ListHelper_PrintList(lst1,NULL);
    ListHelper_Add(lst1,lst4);
    printf("added new item to list\n");
    ListHelper_PrintList(lst1,NULL);
    unsigned int index = 0;
    printf("printing node at index %d:\n",index);
    ListHelper_PrintNode(ListHelper_GetNodeAtIndex(lst1,index),NULL);
    printf("remove item at top:\n");
    ListHelper_Pop(&lst1);
    ListHelper_PrintList(lst1,NULL);
}

int main(){
    return 0;
}