//
// Created by Eitan on 26/12/2019.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "SimSTK.h"
#include "SimRTC.h"
#include "SimulationConfigurations.h"
#include "TimeHelperFunctions.h"


#define BUFFER_SIZE 1024

pthread_mutex_t record_mutex_lock = NULL;

FILE *fp_gps_data = NULL;                   // file pointer for the csv file where the data lies
gps_record_t *stk_data_records = NULL;        // records all data points for the gps
unsigned long stk_num_of_data_records = 0;   // amount of records in record array
unsigned long stk_current_record_index = 0; // index of the current element in the record array
bool stk_sim_input_data_error = false;      // were there an error while initiating the simulation

static int SimSTK_GetNumberOfItemsInFile(FILE *fp){
    if(NULL == fp) {
        return -1;
    }
    int lines = 0;
    char ch;
    while(!feof(fp)){
        ch = (char)fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fseek(fp,0,SEEK_SET);
    return lines - 1;
}
static int SimSTK_ParseCsv(gps_record_t *point, char *line)
{
    char split_char[] = ",";
    char temp[BUFFER_SIZE];
    memcpy(temp,line,BUFFER_SIZE);

    char *ptr = strtok(temp, split_char);
    point->time = parseTime(temp);   //time

    ptr = strtok(line, split_char);
    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->position.posx);
    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->position.posy);
    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->position.posz);

    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->velocity.velx);
    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->velocity.vely);
    ptr = strtok(NULL, split_char);
    sscanf(ptr, "%lf", &point->velocity.velz);

    return 0;
}

int SimSTK_initStkRecords(){

    pthread_mutex_init(&record_mutex_lock, NULL);

    fp_gps_data = fopen(GPS_DATA_CSV_PATH,"r");
    TRACE_ERROR(fopen in GPS SIM,(NULL == fp_gps_data));
    if(NULL == fp_gps_data){
        stk_sim_input_data_error = true;
        return GPS_ERR_IO_ERR_FOPEN;
    }
    int num_of_lines = SimSTK_GetNumberOfItemsInFile(fp_gps_data);

    stk_num_of_data_records = (num_of_lines - 1);
    stk_data_records = (gps_record_t*)malloc( stk_num_of_data_records * sizeof(*stk_data_records));

    char line[BUFFER_SIZE];
    for (int i = 1; i < num_of_lines; ++i) {    // first line is a header and will not be used
        SimSTK_ParseCsv(&stk_data_records[i], line);
    }
    return GPS_ERR_SUCCESS;
}

int SimSTK_GetStkDataRecordAtIndex(gps_record_t *record, unsigned long index){
    if(NULL == stk_data_records){
        return GPS_ERR_NOT_INITIALISED;
    }
    if(NULL == record){
        return GPS_ERR_NULL_POINTER;
    }
    if(index > stk_num_of_data_records){
        return GPS_ERR_PARAMETER_ERR;
    }
    pthread_mutex_lock(&record_mutex_lock);
    if(NULL == memcpy((void*)record,(void*)&stk_data_records[index],sizeof(*record))){
        pthread_mutex_unlock(&record_mutex_lock);
        return GPS_ERR_MEM_ERROR;
    }
    pthread_mutex_unlock(&record_mutex_lock);
    return GPS_ERR_SUCCESS;
}

// returns the most updated record from the STK
int SimSTK_GetCurrentStkDataRecord(gps_record_t *record){
    int err = SimSTK_GetStkDataRecordRange(record,1);
    TRACE_ERROR(reached end of data points in 'SimSTK_GetCurrentStkDataRecord' , -1);
    return -1;
}

// returns a range from the current time
int SimSTK_GetStkDataRecordRange(gps_record_t *records, unsigned int num_of_records){
    if(0 == num_of_records){
        return 0;
    }
    if(NULL == records){
        return GPS_ERR_NULL_POINTER;
    }
    int err = 0;
    atomic_time_t current_time = 0;
    current_time = SimRTC_GetSimulationTime(); // TODO: change to real time from SimRTC
    if(0 != err){
        return err;
    }
    if(num_of_records - 1> stk_current_record_index){
        num_of_records = stk_current_record_index;
    }
    pthread_mutex_lock(&record_mutex_lock);
    for (unsigned int i = stk_current_record_index; i < stk_num_of_data_records; ++i) {
        if(stk_data_records[i].time > current_time){
            stk_current_record_index = i;
            if(NULL == memcpy(records, &stk_data_records[i - (num_of_records - 1) ], sizeof(*records) * num_of_records)){

                err =  GPS_ERR_MEM_ERROR;
            }
            err =  GPS_ERR_SUCCESS;
        }
    }
    pthread_mutex_unlock(&record_mutex_lock);
    return err;
}

// ------------------------------- TESTS
static int SimSTK_ParseLineTest(){
    int err = 0;
    gps_record_t point, test_point;
    char line[BUFFER_SIZE] = "1 Jan 2000 0:00:00.456,-1894.182666,5879.877177,3982.272531,-7.648021,-1.420840,0.192274";
    point.time = 946684800.456;
    point.position.posx = -1894.182666;
    point.position.posy = 5879.877177;
    point.position.posz = 3982.272531;
    point.velocity.velx = -7.648021;
    point.velocity.vely = -1.420840;
    point.velocity.velz = 0.192274;
    SimSTK_ParseCsv(&test_point, line);
#if( 1 == GPS_USE_PRINTS)
    printf("test_point.time = %lf\n",test_point.sat_time);
    printf("posx = %lf\n",test_point.position.posx);
    printf("posy = %lf\n",test_point.position.posx);
    printf("posz = %lf\n",test_point.position.posx);
    printf("velx = %lf\n",test_point.velocity.velx);
    printf("vely = %lf\n",test_point.velocity.vely);
    printf("velz = %lf\n",test_point.velocity.velz);
#endif
    if(0 != memcmp((void*)&point,(void*)&test_point, sizeof(gps_record_t))){
        err = 1;
    }
    TRACE_ERROR(SimSTK_ParseCsv, err);
    return err;
}
static int SimSTK_Lines_Test(unsigned int num_of_lines_in_test_file){
    int err = 0;
    FILE *fp = fopen(GPS_DATA_CSV_PATH,"r");
    int num_of_lines = SimSTK_GetNumberOfItemsInFile(fp);
    if(num_of_lines == 0){
        printf("Gps_Test: Empty test file\n");
    }
    if(num_of_lines != num_of_lines_in_test_file){
        err = 1;
    }
    TRACE_ERROR(SimSTK_GetNumberOfItemsInFile, num_of_lines_in_test_file - num_of_lines);
    return err;
}
int SimSTK_Test(unsigned int num_of_lines_in_test_file){
    int err = 0;
    err += SimSTK_Lines_Test(num_of_lines_in_test_file);
    err += SimSTK_ParseLineTest();

    TRACE_ERROR(SimSTK_Test, err);
    printf("Finished test\n");
    return err;
}
