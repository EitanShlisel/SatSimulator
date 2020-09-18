#ifndef SIMPROJECT_SIMSOLAR_H
#define SIMPROJECT_SIMSOLAR_H

#include "SimConfigFiles/SimulationConfigurations.h"

// will return positive values dictating how much current is produced
double SimSolar_GetSolarCurrentProduction();

void SimSolar_EnableSolarCharge();

//turns all solar charging off
void SimSolar_DisableSolarCharge();

#endif //SIMPROJECT_SIMSOLAR_H
