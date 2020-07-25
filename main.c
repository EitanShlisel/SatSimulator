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
#include "SimFiles/SimI2C.h"
#include "SimFiles/SimEPS.h"
#include "SimFiles/SimTRX.h"

#include "ConsumptionStates/ConsumptionStates.h"

#include "SimulatorCommandLine/SimCmdLine.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "Helper/threads.h"
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
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
    return x/10*sin(0.1*x) * sin(90*x);
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
    sleep(3);
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
            {0,10000, false},
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
        if(i==20){
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

void Plot_EPS_Vbatt_Test(){
    unsigned int i = 0;
    data_point_t points[1];

    figure_t fig = {.x_label = "Time[Sec]",
            .y_label="Voltage[mV]",
            .title="EPS Battery Voltage Over Time",
            .figure_id = 42,
            .sub_figure_id = 0,
            .dataPoints = points,
            .num_of_data_points = 1};


    thread_id tid = StartTcp();
    char *script_path = PLOTTER_PATH_PY;
    char buff[10] = {0};
    itoa(GetClientPortFromThreadId(tid) ,buff,10);
    RunPythonScript(script_path,buff);
    //sleep(5);
    EpsConsumptionState_t fake_subsys_states[] = {
            {0,10000, false},
            {0,-10000, false},
            {0,1000, false},
    };

    int err = 0;
    err = SimRTC_Init();
    TRACE_ERROR(SimRTC_Init,err);
    err = SimEPS_AddConsumptionStates(SUBSYS_FAKE,fake_subsys_states,3);
    TRACE_ERROR(SimEPS_AddConsumptionStates,err);
    err = SimEPS_StartEps();
    TRACE_ERROR(SimEPS_StartEps,err);
    SimEPS_SetChannel(0,true);
    SimEPS_SetSubsysState(SUBSYS_FAKE,0,true);


    atomic_time_t start_time = SimRTC_GetSimulationTime();
    while(1) {
        fig.sub_figure_id = 0;

        points->x = SimRTC_GetSimulationTime();
        points->y = SimEPS_GetBatteryVoltage() + GnrHelper_GenerateGaussianNoise(0,20);
        memcpy(fig.dataPoints,points, sizeof(points));
        SendFigureToPlotter(tid, &fig);
        if(points->y  < 3000){
            printf("SIM: DoD = %f\n",SimEPS_GetDOD());
            break;
        }
//        if(SimRTC_GetSimulationTime() - start_time > 5000){
//            SimEPS_SetSubsysState(SUBSYS_FAKE,0,false);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,1,true);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,2,true);
//        }
//        if(SimRTC_GetSimulationTime() - start_time > 10000){
//            SimEPS_SetSubsysState(SUBSYS_FAKE,0,true);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,1,false);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,2,false);
//        }
//        if(SimRTC_GetSimulationTime() - start_time > 15000){
//            SimEPS_SetSubsysState(SUBSYS_FAKE,0,false);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,1,true);
//            SimEPS_SetSubsysState(SUBSYS_FAKE,2,false);
//        }
        usleep(100);
    }
    while(1){
        usleep(100);
    }
}

void Plot_RSSI(){
    unsigned int i = 0;
    data_point_t points[1];

    figure_t fig = {.x_label = "Time[Sec]",
            .y_label="RSSI[dBm]",
            .title="Simulated TRX RSSI",
            .figure_id = 42,
            .sub_figure_id = 0,
            .dataPoints = points,
            .num_of_data_points = 1};


    thread_id tid = StartTcp();
    char *script_path = PLOTTER_PATH_PY;
    char buff[10] = {0};
    itoa(GetClientPortFromThreadId(tid) ,buff,10);
    RunPythonScript(script_path,buff);

    int err = 0;
    err = SimRTC_Init();
    TRACE_ERROR(SimRTC_Init,err);
    err = SimSTK_initStkRecords();
    TRACE_ERROR(SimRTC_Init,err);
    atomic_time_t start_time = SimRTC_GetSimulationTime();
    atomic_time_t current_time=0,last=SimRTC_GetSimulationTime();
    gps_record_t rec;
    double rssi = 0;
    double dist = 0;
    gps_record_t ground_stations[]  = TRX_GROUND_STATION_LOCATION_ECEF;
    while(1) {
        fig.sub_figure_id = 0;

        points->x = SimRTC_GetSimulationTime();
        current_time = SimRTC_GetSimulationTime();

        rssi = SimTRX_CalcRSSI(2,30);

        dist = GnrHelper_CalcDistance(*(point_t*)&rec.position,*(point_t*)&ground_stations[1].position);
        points->y = dist;
        fig.sub_figure_id = 0;
        SendFigureToPlotter(tid, &fig);

        dist = GnrHelper_CalcDistance(*(point_t*)&rec.position,*(point_t*)&ground_stations[2].position);
        points->y = dist;
        fig.sub_figure_id = 1;
        SendFigureToPlotter(tid, &fig);
        memcpy(fig.dataPoints,points, sizeof(points));

        if(current_time-last >= 60){
//            printf("FSL: %lf\n", SimTRX_CalcFSPL_dB(2));
            printf("sim time = %lf\n",SimRTC_GetSimulationTime());
            SimSTK_GetCurrentStkSatPosition(&rec);
            SimSTK_PrintSatPos(&rec);
            last = current_time;
            printf("gs distance  = %lf\n",dist);
       }
        usleep(100000);
    }
}

void StkTest(){
    int err = 0;
    err = SimRTC_Init();
    TRACE_ERROR(SimRTC_Init,err);
    err = SimSTK_initStkRecords();
    TRACE_ERROR(SimSTK_initStkRecords,err);
    gps_record_t rec;
    atomic_time_t last = SimRTC_GetSimulationTime();
    atomic_time_t current_time = 0;
    SimSTK_GetStkDataRecordAtIndex(&rec,0);
    SimSTK_PrintSatPos(&rec);

    while(1){
        SimSTK_GetCurrentStkSatPosition(&rec);
        current_time = SimRTC_GetSimulationTime();
        printf("Sim time = %lf\n",current_time);
        printf("time diff = %lf\n",current_time-last);
        last = current_time;
        SimSTK_PrintSatPos(&rec);
        SimThreadSleep(5500000);
    }


}

int main(){
    //Plot_RSSI();
    //Plot_EPS_Vbatt_Test();
    //StkTest();
    PlotFigureOverTCP_Test();
    return 0;
}