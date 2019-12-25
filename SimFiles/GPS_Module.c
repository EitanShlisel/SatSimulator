
#include "../RTC.h"
#include "SimulationConfigurations.h"
#include "GPS_Module.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    double sat_time;
    double x_position;
    double y_position;
    double z_position;
    double x_velocity;
    double y_velocity;
    double z_velocity;
}gps_point_t;

bool gps_sim_input_data_error = false;      // were there an error while initiating the simulation
bool gps_init_flag = false;                 // was the init successful
FILE *fp_gps_data = NULL;                   // file pointer for the csv file where the data lies
gps_point_t *gps_data_points = NULL;          // records all data points for the gps
typedef enum {
    GPS_IO_ERR_FOPEN = 1,
    GPS_IO_ERR_FREAD,
    GPS_IO_ERR_FWRITE,
    GPS_IO_ERR_FCLOSE,
    GPS_NOT_INITIALISED,
    GPS_ALREDY_INITIALISED

}GPS_ERR;

#define SIZE 1024
int GetNumberOfItemsInFile(FILE *fp){
    if(NULL == fp) {
        return -1;
    }
    char buffer[SIZE + 1];
    char last_char = '\n';
    size_t bytes;
    int lines = 0;

    while ((bytes = fread(buffer, 1, sizeof(buffer) - 1, fp))) {
        last_char = buffer[bytes - 1];
        for (char *c = buffer; (c = memchr(c, '\n', bytes - (c - buffer))); c++) {
            lines++;
        }
    }
    if (last_char != '\n') {
        lines++;  /* Count the last line even if it lacks a newline */
    }
    fseek(fp,0,SEEK_SET);
    return lines;
}
void GpsAddAnomalies(gps_point_t *point)
{
    #if(1 == GPS_USE_ANOMALLIEY)
        #if(1 == GPS_USE_TIME_DIALATION)
        //TODO: add time dialations...
        #endif
    #endif
}

int GpsParseCsv(gps_point_t *point, char *line)
{
    //TODO: parse line into 'point'
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
    int num_of_lines = GetNumberOfItemsInFile(fp_gps_data);

    gps_data_points = malloc((num_of_lines-1) * sizeof(*gps_data_points));
    char line[SIZE];
    for (int i = 1; i < num_of_lines; ++i) {
        //TODO: read the file line by line and parse into 'gpa_data_points'
        GpsParseCsv(&gps_data_points[i],line);
        GpsAddAnomalies(&gps_data_points[i]);
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
