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

int SimEPS_GetChannel(ChannelIndex chnl);

int SimEPS_SetChannel(ChannelIndex chnl, bool onOff);

int SimEPS_GetChannelStates(bool *channel_states);

double SimEPS_GetCurrentConsumption();

double SimEPS_GetBatteryVoltage();

//return percentage of discharge in the battery
double SimEPS_GetDOD();
// state at index 0 MUST be the idle system on state, where the system is just on.
// For example, a transmitter NOT transmitting
int SimEPS_AddConsumptionStates(SatSubsystem subsys,
                                EpsConsumptionState_t *states,
                                unsigned int length);

int SimEPS_SetSubsysState(SatSubsystem subsys,unsigned int state_index, bool onOff);

int SimEPS_SetStateConsumptionPwr(SatSubsystem subsys,unsigned int state_index, double pwr);

void SimEPS_WaitForSubsysWakeup(ChannelIndex index);

bool SimEPS_IsSubSystemOn(SatSubsystem system);

int SimEPS_StartEps();


//----------------TESTS
void printChannels();
void printStates(SatSubsystem subsys);
#endif //SIMPROJECT_SIMEPS_H
