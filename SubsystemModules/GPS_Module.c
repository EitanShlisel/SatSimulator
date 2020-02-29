#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "RTC.h"
#include "../SimFiles/SimulationConfigurations.h"
#include "../SimFiles/GenericHelpFunctions.h"
#include "../SimFiles/SimSTK.h"
#include "GPS_Module.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



bool gps_init_flag = false;                 // was the init successful

static void Gps_AddAnomalies(gps_record_t *point){
    int rnd = rand();
    double dphi = GnrHelper_GenerateUniformInRange(0,360);
    double dr = GnrHelper_GenerateGaussianNoise(GPS_RADIUS_DIST_MEAN, GPS_RADIUS_DIST_STD);
    double dtan = GnrHelper_GenerateGaussianNoise(GPS_TANGENT_DIST_MEAN, GPS_TANGENT_DIST_STD);

    point->time += GnrHelper_GenerateGaussianNoise(GPS_TIME_DIST_MEAN, GPS_TIME_DIST_STD);
    //TODO: complete calculations...
    point->position.posx = point->position.posx;
    point->position.posy = point->position.posy;
    point->position.posz = point->position.posz;
}

int GPS_Init(){
    if(gps_init_flag) {
        return GPS_ERR_ALREDY_INITIALISED;
    }
    if(0 == SimSTK_initStkRecords()){
        gps_init_flag = true;
        return GPS_ERR_SUCCESS;
    }
    else{
        TRACE_ERROR(GPS_Init,-1);
        return -1;
    }
}

int GPS_getRecords(gps_record_t *record){
    if(!gps_init_flag){
        return GPS_ERR_NOT_INITIALISED;
    }
    if(NULL == record){
        return GPS_ERR_NULL_POINTER;
    }
    int err = SimSTK_GetCurrentStkSatPosition(record);
    Gps_AddAnomalies(record);
    if(0 != err){
        return err;
    }
    return GPS_ERR_SUCCESS;

}

int GPS_getLocation(ECEF_location_t *loc){
    if(!gps_init_flag){
        return GPS_ERR_NOT_INITIALISED;
    }
    if(NULL == loc){
        return GPS_ERR_NULL_POINTER;
    }
    gps_record_t point;
    int err = SimSTK_GetCurrentStkSatPosition(&point);
    if(0 != err){
        return err;
    }
    Gps_AddAnomalies(&point);
    if(NULL == memcpy((void*)loc,(void*)&point.position,sizeof(*loc))){
        return GPS_ERR_MEM_ERROR;
    }
    return GPS_ERR_SUCCESS;
}

int GPS_getVelocity(ECEF_velocity_t *vel){
    if(!gps_init_flag){
        return GPS_ERR_NOT_INITIALISED;
    }
    if(NULL == vel){
        return GPS_ERR_NULL_POINTER;
    }
    gps_record_t point;
    int err = SimSTK_GetCurrentStkSatPosition(&point);
    if(0 != err){
        return err;
    }
    Gps_AddAnomalies(&point);
    if(NULL == memcpy((void*)vel,(void*)&point.velocity,sizeof(*vel))){
        return GPS_ERR_MEM_ERROR;
    }
    return GPS_ERR_SUCCESS;
}

int GPS_getAtomicTime(atomic_time_t *tm){
    if(!gps_init_flag){
        return GPS_ERR_NOT_INITIALISED;
    }
    if(NULL == tm){
        return GPS_ERR_NULL_POINTER;
    }
    gps_record_t point;
    int err = SimSTK_GetCurrentStkSatPosition(&point);
    if(0 != err){
        return err;
    }
    Gps_AddAnomalies(&point);
    if(NULL == memcpy((void*)tm, (void*)&point.time, sizeof(*tm))){
        return GPS_ERR_MEM_ERROR;
    }
    return GPS_ERR_SUCCESS;
}
