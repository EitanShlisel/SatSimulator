#ifndef SIMPROJECT_SIMCONFIGSOLAR_H
#define SIMPROJECT_SIMCONFIGSOLAR_H

//--------------------------------------------------------------------------------------
//-------------------------------------------SOLAR--------------------------------------
//--------------------------------------------------------------------------------------

#define SOLAR_USE_SINE_FLUX_ON_PANELS 0 // reduce the model the an independent sine.
#define SOLAR_USE_PRINTS 0
// production of current with MPP @ 2.275[V] and  solar panels of 60.18[cm^2] with 26.8% efficiency
#define SOLAR_AVERAGE_CURRENT_PRODUCTION_mA (484)    //TODO: should be calculated with ADCS simulation

#endif //SIMPROJECT_SIMCONFIGSOLAR_H
