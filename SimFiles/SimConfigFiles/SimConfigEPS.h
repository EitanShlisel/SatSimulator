#ifndef SIMPROJECT_SIMCONFIGEPS_H
#define SIMPROJECT_SIMCONFIGEPS_H

//--------------------------------------------------------------------------------------
//-------------------------------------------EPS----------------------------------------
//--------------------------------------------------------------------------------------
#define EPS_USE_PRINTS 1

#define EPS_BATTERY_CHARGE_RANGE_START  (0)
#define EPS_BATTERY_CHARGE_RANGE_END_mV    (8500.0)
#define EPS_BATTERY_CHARGE_POLYNOMIAL {-0.000000055691,0.000448227279,-0001.353846666812,8175.336690291735 }

#define EPS_MAX_BATTERY_CHARGE_mAh (8500.0)
#define EPS_INITIAL_BATTERY_CHARGE_mAh (8500.0)


#endif //SIMPROJECT_SIMCONFIGEPS_H
