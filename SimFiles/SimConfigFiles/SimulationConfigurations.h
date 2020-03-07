
#ifndef SIMPROJECT_SIMULATIONCONFIGURATIONS_H
#define SIMPROJECT_SIMULATIONCONFIGURATIONS_H

#include <stdio.h>
#include <unistd.h>

#include "SimConfigGPS.h"
#include "SimConfigEPS.h"
#include "SimConfigI2C.h"

//--------------------------------------------------------------------------------------
//------------------------------------------- SATELLITE---------------------------------
//--------------------------------------------------------------------------------------

typedef enum { // to add a subsystem to the simulation add an element to this enum!
    SUBSYS_OBC = 0,
    SUBSYS_EPS ,
    SUBSYS_GPS,
    SUBSYS_PAYLOAD,
    SUBSYS_FAKE,
    SUBSYS_NUM_OF_SUBSYSTEMS
}SatSubsystem;

//--------------------------------------------------------------------------------------
//------------------------------------------- MACROS------------------------------------
//--------------------------------------------------------------------------------------
#define TRACE_USE_TRACE 1

#if(1 == TRACE_USE_TRACE)
    #define TRACE_ERROR(name,err)    if(0 != (err)){ printf(" %s ; error code =  %d\n",#name,(err));}
#define TRACE_RETURN_ON_ERROR(msg, expected, err,retval) if((expected)!=(err)){printf("%s",#msg);return retval;}
#else
    #define TRACE_ERROR(name,err) ()
    #define TRACE_RETURN_ON_ERROR(msg, expected, err,retval) ()
#endif

//--------------------------------------------------------------------------------------
//-------------------------------------------Real Time Clock----------------------------
//--------------------------------------------------------------------------------------

#define RTC_USE_PRINTS   0                              // print how long was the RTC running


#define RTC_TICK_TIME_PERIOD  ((double)(1))                         // Time measure for a clock single cycle[uSec]
#define RTC_TICKS_PER_SECOND  (2000000)                              // How many ticks does the simulated RTC have in a simulated second

#define RTC_DEFAULT_START_TIME (1574071200)// (1577836800.0)        // Default time after reset (1577836800 = 1/1/2020 00:00:00) for the satellite
// Time at which the simulation starts
#define RTC_SIMULATION_START_TIME (1574071200)//((1577836800.0))    // THIS IS ALSO THE START TIME OF THE STK SIMULATION

//TODO: translate real usec to simulated usec
#define SimThreadSleep(sim_time_us)  (usleep(sim_time_us))    // send thread to sleep for micro-seconds (simulation time) //TODO: fix to be correct



//--------------------------------------------------------------------------------------
//-------------------------------------------SOLAR--------------------------------------
//--------------------------------------------------------------------------------------

// production of current with MPP @ 2.275[V] and  solar panels of 60.18[cm^2] with 26.8% efficiency
#define SOLAR_AVERAGE_CURRENT_PRODUCTION_mA (484)    //TODO: should be calculated with ADCS simulation

#endif //SIMPROJECT_SIMULATIONCONFIGURATIONS_H
