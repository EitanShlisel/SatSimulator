#ifndef SIMPROJECT_OBC_CONSUMPTIONSTATES_H
#define SIMPROJECT_OBC_CONSUMPTIONSTATES_H

#include "../SimFiles/SimConfigFiles/SimulationConfigurations.h"
#include "../SimFiles/SimEPS.h"

typedef enum{
    OBC_CONSMP_IDLE_OBC,
    OBC_CONSMP_FRAM_CONSUMPTION,
    OBC_CONSMP_I2C_BUS,

    OBC_CONSMP_NUM_OF_CONSUMPTION_STATES
}ObcConsumptionStates;

typedef enum{
    TRX_CONSMP_IDLE,
    TRX_CONSMP_TRANSMITTING,

    TRX_CONSMP_NUM_OF_CONSUMPTION_STATES
}TrxConsumptionStates;

typedef enum{
    EPS_CONSMP_IDLE,
    EPS_CONSMP_BATTARY_HEATERS_ON,

    EPS_CONSMP_NUM_OF_CONSUMPTION_STATES
}EpsConsumptionStates;

typedef enum{
    GPS_CONSMP_IDLE,
    GPS_CONSMP_CONVERGING_ON_SOL,
    GPS_CONSMP_STEADY_STATE,

    GPS_CONSMP_NUM_OF_CONSUMPTION_STATES
}GpsConsumptionStates;
int ConsumptionStates_Init();

EpsConsumptionState_t* GetConsumptionStates(SatSubsystem subsys);


#endif //SIMPROJECT_OBC_CONSUMPTIONSTATES_H
