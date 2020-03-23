#ifndef SIMPROJECT_SIMTHERMODYNAMICS_H
#define SIMPROJECT_SIMTHERMODYNAMICS_H

#include "SimConfigFiles/SimulationConfigurations.h"
#define CELSIUS_TO_KELVIN(c)    ((c)+273.15)
#define KELVIN_TO_CELSIUS(k)    ((k)-273.15)

// values in celsius
double SimTherm_GetSubsysTemperature(SatSubsystem subsys);

#endif //SIMPROJECT_SIMTHERMODYNAMICS_H
