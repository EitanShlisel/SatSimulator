#ifndef SIMPROJECT_GPS_TYPES_H
#define SIMPROJECT_GPS_TYPES_H

typedef struct __attribute__ ((__packed__)){
    double posx;
    double posy;
    double posz;
}ECEF_location_t;

typedef struct __attribute__ ((__packed__)){
    double velx;
    double vely;
    double velz;
}ECEF_velocity_t;

typedef double atomic_time_t;

typedef struct {
    atomic_time_t time;
    ECEF_location_t position;
    ECEF_velocity_t velocity;
}gps_record_t;

typedef struct{
    atomic_time_t time;
    ECEF_location_t position;
}sun_vec_t;


typedef enum {
    GPS_ERR_SUCCESS = 0,
    GPS_ERR_IO_ERR_FOPEN = 1,
    GPS_ERR_IO_ERR_FREAD,
    GPS_ERR_IO_ERR_FWRITE,
    GPS_ERR_IO_ERR_FCLOSE,
    GPS_ERR_NOT_INITIALISED,
    GPS_ERR_ALREDY_INITIALISED,
    GPS_ERR_MEM_ERROR,
    GPS_ERR_NULL_POINTER,
    GPS_ERR_PARAMETER_ERR

}GPS_ERR;

#endif //SIMPROJECT_GPS_TYPES_H
