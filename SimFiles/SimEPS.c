#include "SimEPS.h"
#include "SimRTC.h"
#include "SimSolar.h"
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

#define mAsec_to_mAh(mAsec) (double)(((double)(mAsec))/3600.0)



typedef struct EpsMngr_t{
    double *polynom_coeff;      // the interpolation polynomial of the batteries voltage vs discharge capacity [V] vs [mAh]
    unsigned int pol_length;      // polynomial degree

    double batt_temperature;         // temperature of batteries
    double batt_discharge;             // how much charge in the battery [mAh]
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

    // turn off all consumption states on init of EPS simulation
    for (unsigned int i = 0; i < SUBSYS_NUM_OF_SUBSYSTEMS; ++i) {
        for (unsigned int j = 0; j < epsMngr.num_of_states_per_subsys[i] ; ++j) {
            epsMngr.subsys_consumption_states[i][j].is_channel_on = false;
        }
    }
    return 0;
}

bool SimEPS_IsSubSystemOn(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
        bool is_on = epsMngr.subsys_on[subsys];
    pthread_mutex_unlock(&mutex_eps_mngr);
    return is_on;
}

int SimEPS_SetSubsysOn(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_on[subsys]= true;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return true;
}

int SimEPS_SetSubsysOff(SatSubsystem subsys){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_on[subsys]= false;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return false;
}

int SimEPS_SetSubsysState(SatSubsystem subsys,unsigned int state_index, bool onOff){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_consumption_states[subsys][state_index].is_channel_on = onOff;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}

// return positive values
double SimEPS_GetCurrentConsumption(){
    pthread_mutex_lock(&mutex_eps_mngr);

    double current_consumption = 0;
    EpsConsumptionState_t temp;

    for (unsigned int subsys = 0; subsys < SUBSYS_NUM_OF_SUBSYSTEMS; ++subsys) {
        if(epsMngr.subsys_on[subsys]){
            for (unsigned int state = 0; state < epsMngr.num_of_states_per_subsys[subsys]; ++state) {
                temp = epsMngr.subsys_consumption_states[subsys][state];
                if(temp.is_channel_on){
                    current_consumption += temp.avg_curr_consumption_mA;
                }
            }
        }
    }
    pthread_mutex_unlock(&mutex_eps_mngr);
    return current_consumption;
}

double SimEPS_GetBatteryVoltage(){
    int err = 0;
    err = pthread_mutex_lock(&mutex_eps_mngr);
    double batt_charge = epsMngr.batt_discharge;
    double volt = 0;
    for (unsigned int i = 0; i < epsMngr.pol_length; ++i) {
        volt += epsMngr.polynom_coeff[i] * pow(batt_charge,epsMngr.pol_length - i - 1);
    }
#if(EPS_USE_PRINTS == 1)
    printf("EPS battery voltage = %lf\n",volt);
    printf("EPS battery discharge = %lf\n",epsMngr.batt_discharge);
#endif
    epsMngr.batt_volt = volt;
    err = pthread_mutex_unlock(&mutex_eps_mngr);
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

        current_consumption_mA =  SimEPS_GetCurrentConsumption() - SimSolar_GetSolarCurrentProduction();
        epsMngr.batt_discharge += mAsec_to_mAh(dt * current_consumption_mA);
        if(epsMngr.batt_discharge >= EPS_MAX_BATTERY_CHARGE_mAh){
            TRACE_ERROR(negative batt_charge in eps,-666);
            epsMngr.batt_discharge = 0;
        }
        SimThreadSleep(1000);
    }
}

int SetBatteryChargeFunction(double range_start, double range_end,
                             double *pol_coef, unsigned char length){
    if(range_start >= range_end ){
        TRACE_ERROR(SimEps_SetBatteryChargeFunction,-2);
        return -2;
    }
    if(NULL == pol_coef){
        TRACE_ERROR(SimEps_SetBatteryChargeFunction,-4);
        return -4;
    }
    pthread_mutex_lock(&mutex_eps_mngr);
    epsMngr.polynom_coeff = malloc(length * sizeof(*pol_coef));
    if(NULL ==  epsMngr.polynom_coeff){
        pthread_mutex_unlock(&mutex_eps_mngr);
        return -1;
    }
    memcpy(epsMngr.polynom_coeff, pol_coef, sizeof(*pol_coef) * length);
    epsMngr.pol_length = length;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}

int SimEPS_StartEps(){
    int err = 0;
    double pol[] = EPS_BATTERY_CHARGE_POLYNOMIAL;
    unsigned char length = sizeof(pol)/sizeof(*pol);


    err =  pthread_mutex_init(&mutex_eps_mngr,NULL);
    TRACE_ERROR(pthread_mutex_init in SimEPS_StartEps,err);

    err = SetBatteryChargeFunction(EPS_BATTERY_CHARGE_RANGE_START
            ,EPS_BATTERY_CHARGE_RANGE_END_mV,pol,length);
    TRACE_ERROR(SetBatteryChargeFunction in SimEPS_StartEps,err);

    epsMngr.batt_discharge = EPS_MAX_BATTERY_CHARGE_mAh -
                             EPS_INITIAL_BATTERY_CHARGE_mAh;
    epsMngr.batt_volt = SimEPS_GetBatteryVoltage();

    err = pthread_create(&eps_thread_id,NULL,EpsThread,NULL);
    TRACE_ERROR(pthread_create in SimEPS_StartEps,err);
    return 0;
}

void printStates(SatSubsystem subsys){
    printf("subsys number : %d\n",subsys);

    for (unsigned int i = 0; i < epsMngr.num_of_states_per_subsys[subsys]; ++i) {
        printf("{");
        printf("channel on : %X\t",epsMngr.subsys_consumption_states[subsys][i].is_channel_on);
        printf("current consumption : %.2lf\t",epsMngr.subsys_consumption_states[subsys][i].avg_curr_consumption_mA);
        printf("channel voltage : %.2lf\t",epsMngr.subsys_consumption_states[subsys][i].channel_voltage_mV);
        printf("}\n");
    }
}