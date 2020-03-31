#include "SimEPS.h"
#include "SimRTC.h"
#include "SimSolar.h"
#include "../Helper/GenericHelpFunctions.h"

#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

pthread_mutex_t mutex_eps_mngr = NULL;
pthread_t eps_thread_id;

typedef struct EpsMngr_t{
    double *polynom_coeff;          // the interpolation polynomial of the batteries voltage vs discharge capacity [V] vs [mAh]
    unsigned int pol_length;        // polynomial degree

    double batt_discharge;          // how much charge in the battery [mAh]
    double batt_temperature;        // temperature of batteries

    sem_t sem_channel_voltage_rise[EPS_CHANNEL_NUM_OF_CHANNELS];   // signal if a sub-system turned on

    EpsConsumptionState_t *subsys_consumption_states[SUBSYS_NUM_OF_SUBSYSTEMS];
    unsigned int num_of_states_per_subsys[SUBSYS_NUM_OF_SUBSYSTEMS];

    bool is_channel_on[EPS_CHANNEL_NUM_OF_CHANNELS];            // if this channel is on or not; (true = on; false = off)
    ChannelVoltage channel_voltage[EPS_CHANNEL_NUM_OF_CHANNELS];

}EpsMngr_t;
EpsMngr_t epsMngr;

double  EpsMngr_GetChannelVoltageFromIndex(ChannelIndex chnl){
    if(!epsMngr.is_channel_on[chnl]){//if channel is off -> output voltage = 0;
        return 0;
    }
    switch(chnl){
        case CHANNEL_VOLTAGE_3V3:
            return 3300.0;
        case CHANNEL_VOLTAGE_5V:
            return 5000.0;
        case CHANNEL_VOLTAGE_VBATT:
            return SimEPS_GetBatteryVoltage();
        default:
            return 0;
    }
}

bool EpsMngr_GetChannelState(ChannelIndex chnl){
    return epsMngr.is_channel_on[chnl];
}

EpsConsumptionState_t EpsMngr_GetConsumptionState(SatSubsystem subsys, unsigned int state){
    return epsMngr.subsys_consumption_states[subsys][state];
}

// ----------------------------------   API
int SimEPS_AddConsumptionStates(SatSubsystem subsys,EpsConsumptionState_t *states,unsigned int length){
   if(subsys >=SUBSYS_NUM_OF_SUBSYSTEMS || subsys < 0){
       return -1;
   }
   if(NULL == states){
       return -2;
   }
    epsMngr.num_of_states_per_subsys[subsys] = length;
    epsMngr.subsys_consumption_states[subsys] = malloc(length * sizeof(EpsConsumptionState_t));
    memcpy(epsMngr.subsys_consumption_states[subsys],states,length * sizeof(EpsConsumptionState_t));

    // turn off all consumption states on init of EPS simulation
    for (unsigned int subsys = 0; subsys < SUBSYS_NUM_OF_SUBSYSTEMS; ++subsys) {
        for (unsigned int state_index = 0; state_index < epsMngr.num_of_states_per_subsys[subsys] ; ++state_index) {
            epsMngr.subsys_consumption_states[subsys][state_index].is_state_on = false;
        }
    }
    return 0;
}

int SimEPS_SetSubsysState(SatSubsystem subsys,unsigned int state_index, bool onOff){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.subsys_consumption_states[subsys][state_index].is_state_on = onOff;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}
int SimEPS_GetChannel(ChannelIndex chnl){
    return epsMngr.is_channel_on[chnl];
}

int SimEPS_SetChannel(ChannelIndex chnl, bool onOff){
    pthread_mutex_lock(&mutex_eps_mngr);
        epsMngr.is_channel_on[chnl] = onOff;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}

int SimEPS_GetChannelStates(bool *channel_state){
    if(NULL == channel_state){
        TRACE_ERROR(SimEPS_GetChannelStates NULL pointer,-1);
        return -1;
    }
    memcpy(channel_state,epsMngr.is_channel_on,EPS_CHANNEL_NUM_OF_CHANNELS*sizeof(*channel_state));
    return 0;
}

double SimEPS_GetCurrentThroughChannel(ChannelIndex chnl){

    double total_current = 0;
    EpsConsumptionState_t state;
    for (SatSubsystem subsys = 0; subsys < SUBSYS_NUM_OF_SUBSYSTEMS; ++subsys) {
        for (unsigned int i = 0; i < epsMngr.num_of_states_per_subsys[subsys]; ++i) {
            state = EpsMngr_GetConsumptionState(subsys,i);
            if(state.is_state_on && (chnl == state.channel_index )){
                total_current += GnrHelper_Ohms_PowerToCurrent_mA(
                        state.avg_power_consumption_mW,
                        EpsMngr_GetChannelVoltageFromIndex(state.channel_index));
            }
        }
    }
    return total_current;
}

double SimEPS_GetCurrentConsumption(){
    double current_consumption = 0;
    for (unsigned int  chnl = 0; chnl < EPS_CHANNEL_NUM_OF_CHANNELS; ++chnl) {
        current_consumption += SimEPS_GetCurrentThroughChannel(chnl);
    }
    return current_consumption;
}

double SimEPS_GetBatteryVoltage(){
    pthread_mutex_lock(&mutex_eps_mngr);
        double batt_charge = epsMngr.batt_discharge;
        double volt = 0;
        for (unsigned int i = 0; i < epsMngr.pol_length; ++i) {
            volt += epsMngr.polynom_coeff[i] * pow(batt_charge,epsMngr.pol_length - i - 1);
        }
#if(EPS_USE_PRINTS == 1)
    printf("EPS battery voltage = %lf\n",volt);
    printf("EPS battery discharge = %lf\n",epsMngr.batt_discharge);
#endif
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

        current_consumption_mA =   SimEPS_GetCurrentConsumption() - SimSolar_GetSolarCurrentProduction() ;
        epsMngr.batt_discharge += mAsec_to_mAh(dt * current_consumption_mA);
        if(epsMngr.batt_discharge >= EPS_MAX_BATTERY_CHARGE_mAh){
            TRACE_ERROR(negative batt_charge in eps,-666);
            epsMngr.batt_discharge = 0;
        }

        SimThreadSleep(1000);//TODO: change this to somthing real
    }
}

//----------------------------------    INIT
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
        TRACE_ERROR(EpsMngr_InitMngr -> malloc error,-1);
        pthread_mutex_unlock(&mutex_eps_mngr);
        return -1;
    }
    memcpy(epsMngr.polynom_coeff, pol_coef, sizeof(*pol_coef) * length);
    epsMngr.pol_length = length;
    pthread_mutex_unlock(&mutex_eps_mngr);
    return 0;
}

bool SimEPS_IsSubSystemOn(SatSubsystem system){
    EpsConsumptionState_t state;
    state = EpsMngr_GetConsumptionState(system,0);  // state 0 is system on state
    if( epsMngr.is_channel_on[state.channel_index] &&   // is the corresponding state on?
        state.is_state_on){       // is the corresponding channel on
        return true;
    }
    return false;
}

int SimEPS_StartEps(){
    int err = 0;
    if(!SimRTC_RtcStarted()){
        TRACE_ERROR(SimEPS_StartEps RTC has not been started, -1);
        return -1;
    }
    double pol[] = EPS_BATTERY_CHARGE_POLYNOMIAL;
    unsigned char length = sizeof(pol)/sizeof(*pol);

    err =  pthread_mutex_init(&mutex_eps_mngr,NULL);
    TRACE_ERROR(pthread_mutex_init in SimEPS_StartEps,err);

    for (unsigned int i = 0; i < EPS_CHANNEL_NUM_OF_CHANNELS; ++i) {
        err = sem_init(&(epsMngr.sem_channel_voltage_rise[i]),0,0);
        TRACE_ERROR(SimEPS_StartEps sem_init,err);
    }
    err = SetBatteryChargeFunction(EPS_BATTERY_CHARGE_RANGE_START
            ,EPS_BATTERY_CHARGE_RANGE_END_mV,pol,length);
    TRACE_ERROR(SetBatteryChargeFunction in SimEPS_StartEps,err);

    epsMngr.batt_discharge = EPS_MAX_BATTERY_CHARGE_mAh -
                             EPS_INITIAL_BATTERY_CHARGE_mAh;

    ChannelVoltage temp[EPS_CHANNEL_NUM_OF_CHANNELS] = EPS_CHANNEL_VOLTAGES;
    memcpy(epsMngr.channel_voltage, temp, sizeof(temp));

    // set all channels off
    memset(epsMngr.is_channel_on,false,EPS_CHANNEL_NUM_OF_CHANNELS*sizeof(*epsMngr.is_channel_on));

    err = pthread_create(&eps_thread_id,NULL,EpsThread,NULL);
    TRACE_ERROR(pthread_create in SimEPS_StartEps,err);
    return 0;
}

//----------------------------------    EVENTS
void SimEPS_WaitForSubsysWakeup(ChannelIndex index){
    sem_wait(&epsMngr.sem_channel_voltage_rise[index]);
    sem_post(&epsMngr.sem_channel_voltage_rise[index]);    //turnstile
}

//-------------- TESTS
void printChannels(){
    bool temp[EPS_CHANNEL_NUM_OF_CHANNELS] = {0};
    SimEPS_GetChannelStates(temp);
    printf("{");
    printf("%d",temp[0]);
    for (int j = 1; j < EPS_CHANNEL_NUM_OF_CHANNELS; ++j) {
        printf(",%d",temp[j]);
    }
    printf("}\n");
}

void printStates(SatSubsystem subsys){
    printf("subsys number : %d\n",subsys);

    for (unsigned int state_index = 0; state_index < epsMngr.num_of_states_per_subsys[subsys]; ++state_index) {
        printf("{");
        printf("channel index : %d\t",EpsMngr_GetConsumptionState(subsys,state_index).channel_index);
        printf("channel on : %d\t",SimEPS_GetChannel(EpsMngr_GetConsumptionState(subsys,state_index).channel_index));
        printf("current [mA] : %.2lf\t",EpsMngr_GetConsumptionState(subsys,state_index).avg_power_consumption_mW);
        printf("voltage[mV] : %.2lf\t",
               EpsMngr_GetChannelVoltageFromIndex(EpsMngr_GetConsumptionState(subsys,state_index).channel_index));
        printf("}\n");
    }
}