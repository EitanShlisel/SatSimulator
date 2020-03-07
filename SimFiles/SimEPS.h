#ifndef SIMPROJECT_SIMEPS_H
#define SIMPROJECT_SIMEPS_H

#include <stdbool.h>
#include "SimConfigFiles/SimulationConfigurations.h"

typedef struct {
    bool is_channel_on;                 // if this consumption state is on or not;
    double channel_voltage_mV;             // the channel voltage in volts. Usually 3.3[V] or 5[V]
    double avg_curr_consumption_mA;        // average current consumption for this state [mA]
}EpsConsumptionState_t;

int SimEPS_SetSubsysOn(SatSubsystem subsys);

int SimEPS_SetSubsysOff(SatSubsystem subsys);

int SimEPS_SetSubsysState(SatSubsystem subsys,unsigned int state_index,bool onOff);

bool SimEPS_IsSubSystemOn(SatSubsystem subsys);

double SimEPS_GetCurrentConsumption();

double SimEPS_GetBatteryVoltage();

int SimEPS_AddCurrentConsumptionStates(SatSubsystem subsys,EpsConsumptionState_t *states, unsigned int length);

int SimEPS_StartEps();


//----------------TESTS

void printStates(SatSubsystem subsys);
#endif //SIMPROJECT_SIMEPS_H
