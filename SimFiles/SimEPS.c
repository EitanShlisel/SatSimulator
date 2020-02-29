#include "SimEPS.h"
#include "SimRTC.h"
#include "SimSolar.h"
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

#define mAsec_to_mAh(mAsec) (double)(((double)(mAsec))/3600.0)

typedef struct {
    bool is_consuming;                  // if this consumption state is on or not;
    double avg_curr_consumption;        // average current consumption for this state [mA]
}EpsConsumptionState_t;

typedef struct EpsMngr_t{
    double *polynom_coeff;      // the interpolation polynomial of the batteries voltage vs discharge capacity [V] vs [mAh]
    unsigned int pol_length;      // polynomial degree

    double batt_temperature;         // temperature of batteries
    double batt_charge;             // how much charge in the battery [mAh]
    double batt_volt;               // the batteroes voltage [Volt]

    bool subsys_on[SUBSYS_NUM_OF_SUBSYSTEMS];       // flag of which subsystems are on and which are off
    EpsConsumptionState_t *subsys_consumption_states[SUBSYS_NUM_OF_SUBSYSTEMS];
    unsigned int num_of_states_per_subsys[SUBSYS_NUM_OF_SUBSYSTEMS];
}EpsMngr_t;
EpsMngr_t epsMngr;

pthread_mutex_t mutex_eps_mngr = NULL;
pthread_t eps_thread_id;

// ----------------------------------   API

int SimEPS_AddCurrentConsumptionStates(SatSubsystem subsys,EpsConsumptionState_t *states, unsigned int length){
   if(subsys >=SUBSYS_NUM_OF_SUBSYSTEMS || subsys < 0){
       return -1;
   }
   if(NULL == states){
       return -2;
   }
    epsMngr.subsys_consumption_states[subsys] = malloc(length * sizeof(EpsConsumptionState_t));
    memcpy(epsMngr.subsys_consumption_states[subsys],states,length * sizeof(EpsConsumptionState_t));
    epsMngr.num_of_states_per_subsys[subsys] = length;
    return 0;
}

bool SimEPS_IsSubSystemOn(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
        bool is_on = epsMngr.subsys_on[subsys];
    pthread_mutex_lock(&mutex_eps_mngr);
    return is_on;
}

int SimEPS_SetSubsysOn(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_on[subsys]= true;
    pthread_mutex_unlock(&mutex_eps_mngr);
}

int SimEPS_SetSubsysOff(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
    epsMngr.subsys_on[subsys]= false;
    pthread_mutex_unlock(&mutex_eps_mngr);
}

int SimEPS_SetSubsysState(SatSubsystem subsys,unsigned int index,bool onOff){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_consumption_states[subsys][index].is_consuming = onOff;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}
// return positive values
double SimEPS_GetCurrentConsumption(){
    pthread_mutex_lock(&mutex_eps_mngr);
    double current_consumption = 0;
    EpsConsumptionState_t temp;

    for (unsigned int subsys = 0; subsys < SUBSYS_NUM_OF_SUBSYSTEMS; ++subsys) {
        if(!SimEPS_IsSubSystemOn(subsys)){
            continue;
        }
        for (unsigned int state = 0; state < epsMngr.num_of_states_per_subsys[subsys]; ++state) {
            temp = epsMngr.subsys_consumption_states[subsys][state];
            if(temp.is_consuming){
                current_consumption += temp.avg_curr_consumption;
            }
        }
    }
    pthread_mutex_unlock(&mutex_eps_mngr);
    return current_consumption;
}

double SimEPS_GetBatteryVoltage(){
    pthread_mutex_lock(&mutex_eps_mngr);
    double volt = 0;
    for (unsigned int i = 0; i < epsMngr.pol_length; ++i) {
        volt += epsMngr.polynom_coeff[i] * pow(epsMngr.batt_charge,i);
    }
    epsMngr.batt_volt = volt;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return volt;
}

// ----------------------------------   THREADS

void* EpsThread(void *param){
    double current_consumption_mA = 0; // [mA]
    atomic_time_t dt = 0;               //[sec]
    atomic_time_t prev_time = SimRTC_GetSimulationTime();
    atomic_time_t curr_time = 0;

    while(true){
        curr_time = SimRTC_GetSimulationTime();
        dt = curr_time - prev_time;
        prev_time = curr_time;
        current_consumption_mA = SimSolar_GetSolarCurrentProduction() - SimEPS_GetCurrentConsumption();
        if(current_consumption_mA <= 0){
            TRACE_ERROR(negative charge in eps,-666);
            current_consumption_mA = 0;
        }
        epsMngr.batt_charge = mAsec_to_mAh(dt * current_consumption_mA);
        SimThreadSleep(1000);
    }
}

int SetBatteryChargeFunction(double range_start, double range_end,
                             double *pol_coef, unsigned char length){
    pthread_mutex_lock(&mutex_eps_mngr);
    if(range_start >= range_end ){
        TRACE_ERROR(SimEps_SetBatteryChargeFunction,-2);
        return -2;
    }

    if(NULL ==  (epsMngr.polynom_coeff = malloc(length * sizeof(*pol_coef)))){
        return -1;
    }
    memcpy(epsMngr.polynom_coeff, pol_coef, sizeof(*pol_coef) * length);
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}

int SimEPS_StartEps(){
    double pol[] = EPS_BATTERY_CHARGE_POLYNOMIAL;
    unsigned char length = sizeof(pol)/sizeof(*pol);

    epsMngr.batt_volt = EPS_INITIAL_BATTERY_VOLTAGE;

    pthread_mutex_init(&mutex_eps_mngr,NULL);

    SetBatteryChargeFunction(EPS_BATTERY_CHARGE_RANGE_START
            ,EPS_BATTERY_CHARGE_RANGE_END,pol,length);

    pthread_create(&eps_thread_id,NULL,EpsThread,NULL);

    return 0;
}