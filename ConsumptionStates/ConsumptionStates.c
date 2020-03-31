#include "ConsumptionStates.h"
//----------------------------------- OBC
static EpsConsumptionState_t ObcConsumption[OBC_CONSMP_NUM_OF_CONSUMPTION_STATES];

static EpsConsumptionState_t obc_idle_consumption = {
        .is_state_on = false,
         .channel_index = EPS_CHANNEL_0,
         .avg_power_consumption_mW = 100};

static EpsConsumptionState_t fram_consumption = {
        .is_state_on = false,
        .channel_index = EPS_CHANNEL_0,
        .avg_power_consumption_mW = 50};

static EpsConsumptionState_t i2c_consumption = {
        .is_state_on = false,
        .channel_index = EPS_CHANNEL_0,
        .avg_power_consumption_mW = 50};

//----------------------------------- EPS
static EpsConsumptionState_t EpsConsumption[TRX_CONSMP_NUM_OF_CONSUMPTION_STATES];
//----------------------------------- TRX
static EpsConsumptionState_t TrxConsumption[TRX_CONSMP_NUM_OF_CONSUMPTION_STATES];
//----------------------------------- GPS
static EpsConsumptionState_t GpsConsumption[TRX_CONSMP_NUM_OF_CONSUMPTION_STATES];

EpsConsumptionState_t* GetConsumptionStates(SatSubsystem subsys){
    switch(subsys){
        case SUBSYS_OBC:
            return ObcConsumption;
        case SUBSYS_TRX:
            return TrxConsumption;
        case SUBSYS_EPS:
            return EpsConsumption;
        case SUBSYS_GPS:
            return GpsConsumption;
        default:
            return NULL;
    }
}

int ConsumptionStates_Init(){
    ObcConsumption[OBC_CONSMP_IDLE_OBC] = obc_idle_consumption;
    ObcConsumption[OBC_CONSMP_FRAM_CONSUMPTION] = fram_consumption;
    ObcConsumption[OBC_CONSMP_I2C_BUS] = i2c_consumption;
    //TrxConsumption[...] = ...
    //EpsConsumption[...] = ...
    //GpsConsumption[...] = ...
    return 0;
}