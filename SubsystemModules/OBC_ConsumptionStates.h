#ifndef SIMPROJECT_OBC_CONSUMPTIONSTATES_H
#define SIMPROJECT_OBC_CONSUMPTIONSTATES_H

#include "../SimFiles/SimEPS.h"

EpsConsumptionState_t fram_consumption = {.is_state_on = false,
        .channel_index = 0,
        .avg_power_consumption_mW = 50};;

EpsConsumptionState_t i2c_consumption = {.is_state_on = false,
        .channel_index = 0,
        .avg_power_consumption_mW = 50};;

typedef enum{
    OBC_CONSMP_IDLE_OBC,
    OBC_CONSMP_FRAM_CONSUMPTION,
    OBC_CONSP_I2C_BUS
}ObcConsumptionStates;


#endif //SIMPROJECT_OBC_CONSUMPTIONSTATES_H
