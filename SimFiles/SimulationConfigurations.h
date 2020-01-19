
#ifndef SIMPROJECT_SIMULATIONCONFIGURATIONS_H
#define SIMPROJECT_SIMULATIONCONFIGURATIONS_H

#include <stdio.h>
#include <unistd.h>

//------------------------------------------- SATELLITE
typedef enum {
    SUBSYS_OBC = 0,
    SUBSYS_EPS ,
    SUBSYS_GPS,
    SUBSYS_PAYLOAD,

    SUBSYS_NUM_OF_SUBSYSTEMS
}SatSubsystem;

//------------------------------------------- MACROS
#define TRACE_USE_TRACE 1

#if(1 == TRACE_USE_TRACE)
    #define TRACE_ERROR(name,err)    if(0 != (err)){ printf(" %s ; error code =  %d\n",#name,(err));}
#else
    #define TRACE_ERROR(name,err) ()
#endif

//-------------------------------------------Real Time Clock
#define RTC_USE_PRINTS   0                              // print how long was the RTC running


#define RTC_TICK_TIME_PERIOD  ((double)(1))             // Time measure for a single cycle[uSec]
#define RTC_TICKS_PER_SECOND  (1000)                 // How many ticks does the simulated RTC have in a simulated second

#define RTC_DEFAULT_START_TIME (1574071200)// (1577836800.0)           // Default time after reset (1577836800 = 1/1/2020 00:00:00) for the satellite
// Time at which the simulation starts
#define RTC_SIMULATION_START_TIME (1574071200)//((1577836800.0))      // THIS IS ALSO THE START TIME OF THE STK SIMULATION

//TODO: translate real usec to simulated usec
#define SimThreadSleep(sim_time_us)  (usleep(sim_time_us))    // send thread to sleep for micro-seconds (simulation time) //TODO: fix to be correct
//-------------------------------------------GPS Propagator
#define GPS_USE_PRINTS 0
#define GPS_MIN_TIME_STEP   1   // [sec]

#define GPS_SAT_POS_VEC_CSV_PATH   ("C:\\Users\\97250\\Downloads\\Satellite1_J2000_Position_Velocity.csv") // path of STK file- satellite position over time
#define GPS_SAT_POS_CSV_FILE_LINE_OFFSET (1)
#define GPS_SAT_POS_SAMPLE_TIME (60.0)    // sample time period between STK records [sec]


#define GPS_SUN_VEC_PATH    ("C:\\Users\\97250\\Downloads\\Equl_Sun_Vector_J2000.txt")// path of STK file- sun position over time
#define GPS_SUN_VEC_CSV_FILE_LINE_OFFSET (5)
#define GPS_SUN_VEC_SAMPLE_TIME (10.0)    // sample time period between STK records [sec]


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

//-------------------------------------------I2C
#define I2C_USE_PRINTS 0
#define I2C_MAX_SDA_BUFFER_SIZE 1024    // maximum size of data transferred in a single transmission on the I2C

// 100kBit/sec in Standard Mode; 400kBit/sec in Fast-Speed Mode;
// 1MBit/sec in Fast-Mode Plus; 3.4MBit/sec in High-Speed Mode;
#define I2C_DATA_SPEED_KBIT_SEC      (100000)

#define I2C_BIT_TRANSMITTION_TIME
#endif //SIMPROJECT_SIMULATIONCONFIGURATIONS_H
