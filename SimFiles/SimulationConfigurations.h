
#ifndef SIMPROJECT_SIMULATIONCONFIGURATIONS_H
#define SIMPROJECT_SIMULATIONCONFIGURATIONS_H


//------------------------------------------- MACROS
#define TRACE_USE_TRACE 1

#if(1 == TRACE_USE_TRACE)
    #define TRACE_ERROR(name,err)    if(0 != (err)){ printf("Error in %s = %d\n",#name,(err));}
#else
    #define TRACE_ERROR(name,err) ()
#endif

//-------------------------------------------Real Time Clock
#define RTC_USE_PRINTS   0                            // print how long was the RTC running

#define RTC_TICK_TIME_PERIOD  ((double)(1000000))   // Time measure for a single cycle[uSec]

#define RTC_DEFAULT_START_TIME (1577836800.0)       // Default time after reset (1577836800 = 1/1/2020 00:00:00) for the satellite
#define RTC_SIMULATION_START_TIME ((1577836800.0))  // Time at which the simulation starts-
                                                    // THIS IS ALSO THE START TIME OF THE STK SIMULATION
//-------------------------------------------GPS Propagator
#define GPS_USE_PRINTS 0
#define GPS_MIN_TIME_STEP   1   // [sec]
#define GPS_DATA_CSV_PATH ("C:\\Users\\97250\\Downloads\\Satellite1_J2000_Position_Velocity.csv")

#define GPS_USE_ANOMALLIEY 0
#define  GPS_USE_TIME_DIALATION 0

#define GPS_TIME_DIST_VAR   1
#define GPS_TIME_DIST_MEAN  0

#define GPS_X_POS_DIST_VAR  1
#define GPS_X_POS_DIST_MEAN 0

#define GPS_Y_POS_DIST_VAR  1
#define GPS_Y_POS_DIST_MEAN 0

#define GPS_Z_POS_DIST_VAR  1
#define GPS_Z_POS_DIST_MEAN 0

#define GPS_X_VEL_DIST_VAR  1
#define GPS_X_VEL_DIST_MEAN 0

#define GPS_Y_VEL_DIST_VAR  1
#define GPS_Y_VEL_DIST_MEAN 0

#define GPS_Z_VEL_DIST_VAR  1
#define GPS_Z_VEL_DIST_MEAN 0


#endif //SIMPROJECT_SIMULATIONCONFIGURATIONS_H
