
#ifndef SIMPROJECT_GPS_MODULE_H
#define SIMPROJECT_GPS_MODULE_H

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

int GPS_Init();

int GPS_getLocation(ECEF_location_t *loc);

int GPS_getSpeed(ECEF_velocity_t *vel);

int GPS_getAtomicTime(atomic_time_t *tm);


int GPS_Test(unsigned int num_of_lines_in_test_file);

#endif //SIMPROJECT_GPS_MODULE_H
