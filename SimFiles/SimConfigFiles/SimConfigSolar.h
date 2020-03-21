#ifndef SIMPROJECT_SIMCONFIGSOLAR_H
#define SIMPROJECT_SIMCONFIGSOLAR_H

//--------------------------------------------------------------------------------------
//-------------------------------------------SOLAR--------------------------------------
//--------------------------------------------------------------------------------------
#define SOLAR_TEST_USE_ETERNAL_SUNSHINE 0       // stationary satellite only in sunlight
#define SOLAR_TEST_USE_ETERNAL_DARKNESS 1       // stationary satellite only in darkness
#define SOLAR_TEST_USE_SINE_FLUX_ON_PANELS 0    // reduce the model the an independent sine.
#if(SOLAR_TEST_USE_SINE_FLUX_ON_PANELS + SOLAR_TEST_USE_ETERNAL_DARKNESS + SOLAR_TEST_USE_ETERNAL_SUNSHINE > 1)
#error SimConfigSolar.h: choose only one ye bastard!!
#endif
#define SOLAR_USE_PRINTS 0
// production of current with MPP @ 2.275[V] and  solar panels of 60.18[cm^2] with 26.8% efficiency
#define SOLAR_NUM_OF_SOLAR_PANELS_FACING_THE_SUN 6
#define SOLAR_AVERAGE_CURRENT_PRODUCTION_mA (484)    //TODO: should be calculated with ADCS simulation

#endif //SIMPROJECT_SIMCONFIGSOLAR_H
