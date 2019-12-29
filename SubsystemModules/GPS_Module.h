#ifndef SIMPROJECT_GPS_MODULE_H
#define SIMPROJECT_GPS_MODULE_H

#include "GPS_types.h"



int GPS_Init();

int GPS_getLocation(ECEF_location_t *loc);

int GPS_getVelocity(ECEF_velocity_t *vel);

int GPS_getAtomicTime(atomic_time_t *tm);


int SimSTK_Test(unsigned int num_of_lines_in_test_file);

#endif //SIMPROJECT_GPS_MODULE_H
