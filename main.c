#include "Helper/TimeHelperFunctions.h"
#include "Helper/GenericHelpFunctions.h"
#include "Helper/TcpHandler.h"
#include "Helper/TcpPlotter.h"
#include "Helper/RunPython.h"
#include "Helper/List.h"

#include "SubsystemModules/GPS_Module.h"
#include "SubsystemModules/Time.h"
#include "SubsystemModules/RTC.h"
#include "SubsystemModules/FRAM.h"

#include "SimFiles/SimConfigFiles/SimulationConfigurations.h"
#include "SimFiles/SimThermodynamics.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimRTC.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimI2C.h"
#include "SimFiles/SimEPS.h"

#include "ConsumptionStates/ConsumptionStates.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <pthread_time.h>
#include <stdbool.h>
#include <math.h>

int python_tcp_Test(){
    thread_id tid = StartTcp();
    int i = 0;
    unsigned char buff[DEFAULT_BUFLEN]={0};
    while(1){
        sleep(1);
        if(i % 5 == 0){
            printf("sending data\n");
            AddDataToSendQueue(tid, buff);
        }

        i++;
    }
}

double costume_func0(double x){
    double r = ((double) rand() / (RAND_MAX));
    return sin(0.1*x)+exp(x/100) + r/5.0;
}
double costume_func1(double x){
    double r = ((double) rand() / (RAND_MAX));
    return sin(0.01*x) * sin(90*x);
}
double costume_func2(double x){
    double r = ((double) rand() / (RAND_MAX));
    return cos(0.1*x) * x + r/5.0;
}
#define NUM_OF_POINTS 100
void PlotFigureOverTCP_Test(){
    unsigned int i = 0;
    double start = 0;
    double end = 2.0 * M_PI;
    data_point_t points[NUM_OF_POINTS];

    figure_t fig = {.x_label = "Xlabel",
            .y_label="Ylabel",
            .title="I'm the best",
            .figure_id = 42,
            .sub_figure_id = 0,
            .dataPoints = points,
            .num_of_data_points = NUM_OF_POINTS};


    thread_id tid = StartTcp();
    char *script_path = PLOTTER_PATH_PY;
    char buff[10] = {0};
    itoa(GetClientPortFromThreadId(tid) ,buff,10);
    RunPythonScript(script_path,buff);

    while(1) {
        fig.sub_figure_id = 0;
        ApplyFunctionToRange(points, NUM_OF_POINTS, start, end, costume_func0);
        memcpy(fig.dataPoints,points, sizeof(points));
        SendFigureToPlotter(tid, &fig);

        fig.sub_figure_id = 1;
        ApplyFunctionToRange(points, NUM_OF_POINTS,start,end, costume_func1);
        memcpy(fig.dataPoints,points, sizeof(points));
        SendFigureToPlotter(tid, &fig);

        fig.sub_figure_id = 2;
        ApplyFunctionToRange(points, NUM_OF_POINTS,start,end, costume_func2);
        memcpy(fig.dataPoints,points, sizeof(points));
        SendFigureToPlotter(tid, &fig);
        usleep(1000);
        start = end;
        end += 2 * M_PI;
    }
}

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

void FRAM_Test(){
    int err = 0;
    err = SimRTC_Init();
    TRACE_ERROR(SimRTC_Init,err);
    if(0 == err){
        while(!SimRTC_RtcStarted());
    }
    err = SimEPS_StartEps();
    TRACE_ERROR(SimEPS_StartEps,err);
    ConsumptionStates_Init();
    EpsConsumptionState_t *temp = GetConsumptionStates(SUBSYS_OBC);
    err = SimEPS_AddConsumptionStates(SUBSYS_OBC,temp,OBC_CONSMP_NUM_OF_CONSUMPTION_STATES);
    TRACE_ERROR(SimEPS_AddConsumptionStates, err);
    err = FRAM_start();
    TRACE_ERROR(FRAM_start,err);

    printf("main: starting FRAM test\n\n");
    unsigned char buff[100] = {0};
    for(unsigned int i = 0; i < 10; i++) {
        err = FRAM_read(buff,i* sizeof(buff),sizeof(buff));
        TRACE_ERROR(FRAM_start, err);
        printf(buff);
    }
    for (unsigned int j = 0; j < sizeof(buff); ++j) {
        buff[j] = (j % 256);
    }
    err = FRAM_write(buff,0,sizeof(buff));
    TRACE_ERROR(FRAM_write,err);

    printf("main: print data\n");
    for (unsigned int k = 0; k < sizeof(buff); ++k) {
        printf("%02X-",buff[k]);
    }
    printf("\n");
    memset(buff,0,sizeof(buff));

    err = FRAM_read(buff,0,sizeof(buff));
    TRACE_ERROR(FRAM_read,err);

    printf("main: print written data from FRAM\n");
    for (unsigned int k = 0; k < sizeof(buff); ++k) {
        printf("%02X-",buff[k]);
    }
}

int main(){
    PlotFigureOverTCP_Test();
    return 0;
}