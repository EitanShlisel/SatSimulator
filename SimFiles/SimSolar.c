#include "SimSolar.h"
#include "SimRTC.h"
#include "SimSTK.h"
#include <math.h>

// production of current with MPP @ 2.275[V] and 6 solar panels of 30.18[cm^2] with 26.8%
double SimSolar_GetSolarCurrentProduction(){
    atomic_time_t curr_time = SimRTC_GetSimulationTime();
    double current = 6 *  SOLAR_AVERAGE_CURRENT_PRODUCTION_mA *
            (0.5 + 0.5 * sin(2 * M_PI * curr_time / SimSTK_GetOrbitTime()));   //mA
    return 0;
}