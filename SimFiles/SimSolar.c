#include "SimSolar.h"
#include "SimRTC.h"
#include "SimSTK.h"
#include "GenericHelpFunctions.h"
#include <math.h>

// production of current with MPP @ 2.275[V] and 6 solar panels of 30.18[cm^2] with 26.8%
double SimSolar_GetSolarCurrentProduction(){
    double current = 0;
#if(SOLAR_TEST_USE_ETERNAL_DARKNESS == 1)
    current = 0;
#elif(SOLAR_TEST_USE_ETERNAL_SUNSHINE == 1)
    current = SOLAR_NUM_OF_SOLAR_PANELS_FACING_THE_SUN *
            SOLAR_AVERAGE_CURRENT_PRODUCTION_mA;
#elif(SOLAR_TEST_USE_SINE_FLUX_ON_PANELS == 1)
    current = (SOLAR_NUM_OF_SOLAR_PANELS_FACING_THE_SUN *
            SOLAR_AVERAGE_CURRENT_PRODUCTION_mA) * cos(2*M_PI /5400.0);//5400 = 1.5 hour in seconds
    if(current < 0){
        current = 0;
    }
#else
    point sat_loc;
    point sun_loc;
    gps_record_t rec;
    sun_vec_t sun_vec;
    int err = 0;
    err = SimSTK_GetCurrentStkSatPosition(&rec);
    TRACE_ERROR(SimSolar_GetSolarCurrentProduction,err)
    if(0!= err)
        return -1;
    err = SimSTK_GetCurrentStkSunVec(&sun_vec);
    if(0!= err){
        TRACE_ERROR(SimSolar_GetSolarCurrentProduction,err)
        return 0;
    }
    memcpy(&sat_loc ,&rec.position,sizeof(sat_loc));        //satellite location
    memcpy(&sun_loc,&sun_vec.position,sizeof(sun_loc));     //sun location
    if(GnrHelper_LineSphereIntersection(
            sat_loc,
            sun_loc,
            (point) STK_EARTH_COORDINATE_CARTESIAN,
            STK_EARTH_RADIUS_km)) {
        return 0;
    }
    current = SOLAR_NUM_OF_SOLAR_PANELS_FACING_THE_SUN *  SOLAR_AVERAGE_CURRENT_PRODUCTION_mA;
#endif
#if(SOLAR_USE_PRINTS  == 1)
    printf("Solar Current Production = %lf\n",current);
#endif
    return current;
}