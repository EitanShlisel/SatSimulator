#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "../RTC.h"
#include "SimulationConfigurations.h"
#include "GPS_Module.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef struct {
    double sat_time;
    ECEF_location_t position;
    ECEF_velocity_t velocity;
}gps_point_t;

typedef enum {
    GPS_IO_ERR_FOPEN = 1,
    GPS_IO_ERR_FREAD,
    GPS_IO_ERR_FWRITE,
    GPS_IO_ERR_FCLOSE,
    GPS_NOT_INITIALISED,
    GPS_ALREDY_INITIALISED

}GPS_ERR;

bool gps_sim_input_data_error = false;      // were there an error while initiating the simulation
bool gps_init_flag = false;                 // was the init successful
FILE *fp_gps_data = NULL;                   // file pointer for the csv file where the data lies
gps_point_t *gps_data_points = NULL;        // records all data points for the gps

int Gps_GetNumberOfItemsInFile(FILE *fp){
    if(NULL == fp) {
        return -1;
    }
    int lines = 0;
    char ch;
    while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fseek(fp,0,SEEK_SET);
    return lines - 1;
}

void Gps_AddAnomalies(gps_point_t *point)
{
    #if(1 == GPS_USE_ANOMALLIEY)
        #if(1 == GPS_USE_TIME_DIALATION)
        //TODO: add time dialations...
        #endif
    #endif
}

int monthToInt(char *month){
    char* month_arr[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    for (int i = 0; i < (int)(sizeof(month_arr)/ sizeof(month_arr[0])); ++i) {
        if(0 == strcmp(month,month_arr[i])){
            return i;
        }
    }
    return -1;
}
double parseTime(char *line){
    //18 Nov 2019 10:00:00.000
  double unix_time = 0;
  double milli_sec = 0;
  struct tm tim;
  char split_char[] = " ,:.";

  char *ptr = strtok(line, split_char); // day of the month(0-31)
  tim.tm_mday = atoi(ptr);

  ptr = strtok(NULL, split_char);   // month (0-11)
  tim.tm_mon = monthToInt(ptr);

  ptr = strtok(NULL, split_char);   // year
  tim.tm_year = atoi(ptr) - 1900;

  ptr = strtok(NULL, split_char);   // hour (0-23)
  tim.tm_hour = atoi(ptr) + 2;

  ptr = strtok(NULL, split_char);   // minute(0-59)
  tim.tm_min = atoi(ptr);

  ptr = strtok(NULL, split_char);   // sec(0-61)
  tim.tm_sec = atoi(ptr);

  ptr = strtok(NULL, split_char);   // millisec(0-999)
  milli_sec = (double)(atoi(ptr))/1000.0;
    //"1 Jan 2000 1:02:03.456,-1894.182666,5879.877177,3982.272531,-7.648021,-1.420840,0.192274";
  tim.tm_isdst = -1;
  unix_time = (double)mktime(&tim) + milli_sec;
  return unix_time;
}
int Gps_ParseCsv(gps_point_t *point, char *line)
{
    char split_char[] = ",";
    char temp[BUFFER_SIZE];
    memcpy(temp,line,BUFFER_SIZE);

    char *ptr = strtok(temp, split_char);
    point->sat_time = parseTime(temp);   //time

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

int GPS_Init(){
    if(gps_init_flag) {
        return GPS_ALREDY_INITIALISED;
    }
    fp_gps_data = fopen(GPS_DATA_CSV_PATH,"r");
    TRACE_ERROR(fopen in GPS SIM,(NULL == fp_gps_data));
    if(NULL == fp_gps_data){
        gps_sim_input_data_error = true;
        return GPS_IO_ERR_FOPEN;
    }
    int num_of_lines = Gps_GetNumberOfItemsInFile(fp_gps_data);

    gps_data_points = malloc((num_of_lines-1) * sizeof(*gps_data_points));
    char line[BUFFER_SIZE];
    for (int i = 1; i < num_of_lines; ++i) {
        //TODO: read the file line by line and parse into 'gpa_data_points'
        Gps_ParseCsv(&gps_data_points[i], line);
        Gps_AddAnomalies(&gps_data_points[i]);
    }
    gps_init_flag = true;
    return 0;
}

int GPS_getLocation(ECEF_location_t *loc){
    if(!gps_init_flag){
        return GPS_NOT_INITIALISED;
    }
    return 0;
}

int GPS_getSpeed(ECEF_velocity_t *vel){
    if(!gps_init_flag){
        return GPS_NOT_INITIALISED;
    }
    return 0;
}

int GPS_getAtomicTime(atomic_time_t *tm){
    if(!gps_init_flag){
        return GPS_NOT_INITIALISED;
    }
    return 0;
}


int GPS_ParseLineTest(){
    int err = 0;
    gps_point_t point, test_point;
    char line[BUFFER_SIZE] = "1 Jan 2000 0:00:00.456,-1894.182666,5879.877177,3982.272531,-7.648021,-1.420840,0.192274";
    point.sat_time = 946684800.456;
    point.position.posx = -1894.182666;
    point.position.posy = 5879.877177;
    point.position.posz = 3982.272531;
    point.velocity.velx = -7.648021;
    point.velocity.vely = -1.420840;
    point.velocity.velz = 0.192274;
    Gps_ParseCsv(&test_point,line);
#if( 1 == GPS_USE_PRINTS)
    printf("test_point.time = %lf\n",test_point.sat_time);
    printf("posx = %lf\n",test_point.position.posx);
    printf("posy = %lf\n",test_point.position.posx);
    printf("posz = %lf\n",test_point.position.posx);
    printf("velx = %lf\n",test_point.velocity.velx);
    printf("vely = %lf\n",test_point.velocity.vely);
    printf("velz = %lf\n",test_point.velocity.velz);
#endif
    if(0 != memcmp((void*)&point,(void*)&test_point, sizeof(gps_point_t))){
        err = 1;
    }
    TRACE_ERROR(Gps_ParseCsv,err);
    return err;
}
int GPS_Lines_Test(unsigned int num_of_lines_in_test_file){
    int err = 0;
    FILE *fp = fopen(GPS_DATA_CSV_PATH,"r");
    int num_of_lines = Gps_GetNumberOfItemsInFile(fp);
    if(num_of_lines == 0){
        printf("Gps_Test: Empty test file\n");
    }
    if(num_of_lines != num_of_lines_in_test_file){
        err = 1;
    }
    TRACE_ERROR(Gps_GetNumberOfItemsInFile, num_of_lines_in_test_file - num_of_lines);
    return err;
}

int GPS_Test(unsigned int num_of_lines_in_test_file){
    int err = 0;
    err += GPS_Lines_Test(num_of_lines_in_test_file);
    err += GPS_ParseLineTest();

    TRACE_ERROR(GPS_Test,err);
    printf("Finished test\n");
    return err;
}