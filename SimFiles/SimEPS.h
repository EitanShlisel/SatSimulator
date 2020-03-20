#ifndef SIMPROJECT_SIMEPS_H
#define SIMPROJECT_SIMEPS_H

#include <stdbool.h>
#include "SimConfigFiles/SimulationConfigurations.h"
typedef enum{
    EPS_CHANNEL_0,
    EPS_CHANNEL_1,
    EPS_CHANNEL_2,
    EPS_CHANNEL_3,
    EPS_CHANNEL_4,
    EPS_CHANNEL_5,
    EPS_CHANNEL_6,
    EPS_CHANNEL_7,

    EPS_CHANNEL_NUM_OF_CHANNELS
}ChannelIndex;

typedef enum{       // this enum contains the possibilities of supply voltages from the EPS
    CHANNEL_VOLTAGE_5V,
    CHANNEL_VOLTAGE_3V3,
    CHANNEL_VOLTAGE_VBATT
}ChannelVoltage;


typedef struct {
    ChannelIndex channel_index;             // the channel through which the subsystem consumes the current.
    double avg_power_consumption_mW;        // average power consumption for this state [mW]
    bool is_state_on;                       // states whether the consumption state os active or not (true = on; false = off)
}EpsConsumptionState_t;

int SimEPS_SetSubsysOn(SatSubsystem subsys);

int SimEPS_SetSubsysOff(SatSubsystem subsys);

int SimEPS_SetSubsysState(SatSubsystem subsys, unsigned int state_index, bool onOff);

int SimEPS_SetChannel(ChannelIndex chnl, bool onOff);

bool SimEPS_IsSubSystemOn(SatSubsystem subsys);

double SimEPS_GetCurrentConsumption();

double SimEPS_GetBatteryVoltage();

int SimEPS_AddConsumptionStates(SatSubsystem subsys,
                                EpsConsumptionState_t *states,
                                unsigned int length);

int SimEPS_StartEps();


//----------------TESTS

void printStates(SatSubsystem subsys);
#endif //SIMPROJECT_SIMEPS_H
