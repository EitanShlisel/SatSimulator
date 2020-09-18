#ifndef SIMPROJECT_SIMCONFIGEPS_H
#define SIMPROJECT_SIMCONFIGEPS_H

//--------------------------------------------------------------------------------------
//-------------------------------------------EPS----------------------------------------
//--------------------------------------------------------------------------------------
#define EPS_USE_PRINTS 0

#define EPS_BATTERY_CHARGE_RANGE_START  (0)
#define EPS_BATTERY_CHARGE_RANGE_END_mV    (8500.0)
#define EPS_BATTERY_CHARGE_POLYNOMIAL {-0.000000055691,0.000448227279,-0001.353846666812,8175.336690291735 }

#define EPS_MAX_BATTERY_CHARGE_mAh (8500.0)
#define EPS_INITIAL_BATTERY_CHARGE_mAh (8300.0)

#define EPS_DISCHARGE_AT_0VOLT_mAh      (7434.15)
#define EPS_MAX_DoD_PERCENT            (0.80)      // max percentage of battery discharge
//


// to declare what voltage will be in a specific channel change this array
#define EPS_CHANNEL_VOLTAGES {\
    CHANNEL_VOLTAGE_5V,\
    CHANNEL_VOLTAGE_3V3,\
    CHANNEL_VOLTAGE_VBATT,\
    CHANNEL_VOLTAGE_5V,\
    CHANNEL_VOLTAGE_5V,\
    CHANNEL_VOLTAGE_5V,\
    CHANNEL_VOLTAGE_5V,\
    CHANNEL_VOLTAGE_5V,\
};

#endif //SIMPROJECT_SIMCONFIGEPS_H
