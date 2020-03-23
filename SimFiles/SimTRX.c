#include "SimConfigFiles/SimulationConfigurations.h"

#include "SimTRX.h"
#include "SimSTK.h"
#include "SimThermodynamics.h"
#include "SimRTC.h"

#include "../Helper/List.h"
#include "../Helper/GenericHelpFunctions.h"
#include "../Helper/TimeHelperFunctions.h"

#include <pthread.h>
#include <semaphore.h>
#include <math.h>
ECEF_location_t gs_locations[] = TRX_GROUND_STATION_LOCATION_ECEF;

list_t *lst_uplink_buffer = NULL;       // uplink command buffer
list_t *lst_downlink_buffer = NULL;     // downlink telemetry buffer

sem_t  sem_downlink_buff= NULL;         // signal to the thread that new packets are to be sent
pthread_mutex_t mutex_downlink = NULL;  // mutex on downlink buffer

sem_t  sem_uplink_buff = NULL;          // signal to the thread that new packets are sent to the TRX
pthread_mutex_t mutex_uplink = NULL;    // mutex on uplink buffer

#if(TRX_TRANSMIT_TO_FILE == 1)
    FILE *fp_trx_downlink = NULL;
    char filename_downlink[256];            // filepathto which the data will be written to
#endif
int SimTRX_InitSimTRX(){
    int err = 0;
    err = pthread_mutex_init(&mutex_downlink,NULL);
    TRACE_ERROR(SimTRX_InitSimTRX -> pthread_mutex_init,err);
    if(0 != err){
        return err;
    }
    err = pthread_mutex_init(&mutex_uplink,NULL);
    TRACE_ERROR(SimTRX_InitSimTRX -> pthread_mutex_init,err);
    if(0 != err){
        return err;
    }
    err = sem_init(&sem_downlink_buff,0,0);
    TRACE_ERROR(SimTRX_InitSimTRX -> sem_init,err);
    if(0 != err){
        return err;
    }
    err = sem_init(&sem_uplink_buff,0,0);
    TRACE_ERROR(SimTRX_InitSimTRX -> sem_init,err);
    if(0 != err){
        return err;
    }
#if(TRX_TRANSMIT_TO_FILE == 1)
    AddTimeStamp(filename_downlink,TRX_DOWNLINK_FILE_PATH);
    fp_trx_downlink = fopen(filename_downlink, "w");
    if(NULL == fp_trx_downlink){
        TRACE_ERROR(SimTRX_InitSimTRX -> fopen,-1);
    }
#endif
    return 0;
}

int SimTRX_AddPacketToDownlinkBuffer(unsigned char *data, unsigned int length){
    list_t *node = ListHelper_CreateList(data,length);
    if(NULL == node){
        TRACE_ERROR(SimTRX_AddPacketToDownlinkBuffer ->ListHelper_CreateList , -1);
        return -1;
    }
    int err = 0;
    pthread_mutex_lock(&mutex_downlink);
        lst_downlink_buffer = ListHelper_Add(lst_downlink_buffer,node);
        if(NULL == lst_downlink_buffer){
            TRACE_ERROR(SimTRX_AddPacketToDownlinkBuffer -> ListHelper_Add, -2);
            ListHelper_DestroyNode(node);
            err = -2;
        }else{
            sem_post(&sem_downlink_buff);
        }
    pthread_mutex_unlock(&mutex_downlink);
    return err;
}

int SimTRX_AddPacketToUplinkBuffer(unsigned char *data, unsigned int length){
    list_t *node = ListHelper_CreateList(data,length);
    if(NULL == node){
        TRACE_ERROR(SimTRX_AddPacketToUplinkBuffer ->ListHelper_CreateList , -1);
        return -1;
    }
    int err = 0;
    pthread_mutex_lock(&mutex_uplink);
    lst_downlink_buffer = ListHelper_Add(lst_downlink_buffer,node);
    if(NULL == lst_uplink_buffer){
        TRACE_ERROR(SimTRX_AddPacketToDownlinkBuffer -> ListHelper_Add, -2);
        ListHelper_DestroyNode(node);
        err = -2;
    }else{
        sem_post(&sem_uplink_buff);
    }
    pthread_mutex_unlock(&mutex_uplink);
    return 0;
}

// transmits a packet from the downlink buffer
int SimTRX_TransmitFromBuffer(){
    list_t *node;
    pthread_mutex_lock(&mutex_downlink);
    node = ListHelper_Pop(&lst_downlink_buffer);
    if(NULL == node){
        TRACE_ERROR(SimTRX_TransmitFromBuffer->ListHelper_Pop: Null list,-1);
        pthread_mutex_unlock(&mutex_downlink);
        return 0;
    }
    #if(TRX_TRANSMIT_TO_PRINT == 1)
        ListHelper_PrintNode(node,stdout);
    #endif
    #if(TRX_TRANSMIT_TO_FILE == 1)
        atomic_time_t curr_time = SimRTC_GetSimulationTime();
        fprintf(fp_trx_downlink,"\n Current simulated time = %lf\n",curr_time);
        ListHelper_PrintNode(node,fp_trx_downlink);
    #endif
    #if(TRX_TRANSMIT_TO_TCP == 1)
        //TODO: complete TCP
    #endif
    pthread_mutex_unlock(&mutex_downlink);
    return 0;
}

// reads a packet from the uplink buffer
int SimTRX_ReadFromBuffer(unsigned char *data, unsigned int *length){
    if((NULL == data) || (NULL == length)){
        TRACE_ERROR(SimTRX_ReadFromBuffer,-1);
        return -1;
    }
    list_t *node = ListHelper_Pop(&lst_uplink_buffer);
    if(NULL == node){
        TRACE_ERROR(SimTRX_ReadFromBuffer ->ListHelper_Pop,-2);
        return -2;
    }
    pthread_mutex_lock(&mutex_uplink);
        *length = node->data_length;
        memcpy(data,node->data,*length);
        ListHelper_DestroyNode(node);
    pthread_mutex_unlock(&mutex_uplink);
    return 0;
}

unsigned int SimTRX_GetNumberOfPacketsInBuffer(TrxPacketBuffer buff){
    unsigned int num = 0;
    switch(buff){
        case BUFFER_DOWNLINK:
            pthread_mutex_lock(mutex_downlink);
                num = ListHelper_GetLength(lst_downlink_buffer);
            pthread_mutex_unlock(mutex_downlink);
            break;
        case BUFFER_UPLINK:
            pthread_mutex_lock(mutex_uplink);
                num = ListHelper_GetLength(lst_uplink_buffer);
            pthread_mutex_unlock(mutex_uplink);
            break;
        default:
            return 0;
    }
    return num;
}
// ------------------------------- CHANNEL PROPERTIES

//delay between sending packet from the ground sation to the satellite and vice versa
double SimTRX_CalcTransmissionDelay(unsigned int gs_index){
    ECEF_location_t gs_loc,sat_loc;
    gps_record_t rec;
    SimSTK_GetCurrentStkSatPosition(&rec);
    memcpy(&sat_loc,&rec.position,sizeof(sat_loc));
    memcpy(&gs_loc,&gs_locations[gs_index],sizeof(gs_loc));

    double dist_km = GnrHelper_CalcDistance(*(point_t*)&sat_loc, *(point_t*)&gs_loc);
    return (1000 * dist_km/SPEED_OF_LIGHT_m_sec);
}

double SimTRX_CalcTransmissionTime(unsigned int baud_bit_sec,unsigned int num_of_bytes_in_packet){
    return ((8.0*num_of_bytes_in_packet)/baud_bit_sec);
}
// ------------------------------- LINK BUDGET
double SimTRX_CalcAtmosphereAttenuation_dB(unsigned int gs_index){
    return 0;
}
// free space loss
double SimTRX_CalcFSL_dB(unsigned int gs_index){
    ECEF_location_t gs_loc,sat_loc;
    gps_record_t rec;
    SimSTK_GetCurrentStkSatPosition(&rec);
    memcpy(&sat_loc,&rec.position,sizeof(sat_loc));
    memcpy(&gs_loc,&gs_locations[gs_index],sizeof(gs_loc));

    double dist_km = GnrHelper_CalcDistance(*(point_t*)&sat_loc, *(point_t*)&gs_loc);
    double fsl = 20*log10(4*M_PI/SPEED_OF_LIGHT_m_sec) + 20*log10(1000*dist_km) + 20 * log10(TRX_UPLINK_CENTER_FREQUANCY_Hz) ;
    return fsl;
}
// caused by the lossed of the system
double SimTRX_CalcSystemLosses_dB(){
    return 0;
}
// allows to further expanding of the simulator when ADCS simulation will be available
double SimTRX_CalcAntennaGain_dB(unsigned int gs_index){
    // needs to take the satellites attitude into consideration, and the antennas characteristics.
    return 0;
}
// calculates the noise floor power level according to Nyquist
double SimTRX_CalcNoiseFloorLevel(){
    double temp = SimTherm_GetSubsysTemperature(SUBSYS_TRX);
    double noise_dB = 0;
    noise_dB = K_BOLTZMAN_CONSTANT_dB +
               10*log10(CELSIUS_TO_KELVIN(temp)) +
               10*log10(TRX_RX_ANTENNA_BANDWIDTH_Hz);;
    return noise_dB;
}
// calculates the link budget according to Fris's formula
double SimTRX_CalcRSSI(unsigned int gs_index,double gs_transmit_power_dBm){
    ECEF_location_t gs_loc,sat_loc;
    gps_record_t rec;
    SimSTK_GetCurrentStkSatPosition(&rec);
    memcpy(&sat_loc,&rec.position,sizeof(sat_loc));
    memcpy(&gs_loc,&gs_locations[gs_index],sizeof(gs_loc));

    double dist = GnrHelper_CalcDistance(*(point_t*)&sat_loc,*(point_t*)&gs_loc);

    double attenuation_dB = 0;
    attenuation_dB += SimTRX_CalcAtmosphereAttenuation_dB(gs_index);
    attenuation_dB += SimTRX_CalcFSL_dB(gs_index);
    attenuation_dB += SimTRX_CalcSystemLosses_dB();

    double gain_dB = 0;
    gain_dB += SimTRX_CalcAntennaGain_dB(gs_index);
    double rssi_dB = gs_transmit_power_dBm - attenuation_dB + gain_dB;
    if(rssi_dB < SimTRX_CalcNoiseFloorLevel()){
        rssi_dB = SimTRX_CalcNoiseFloorLevel();
    }
    return rssi_dB;
}
// assumes GS is on earths surface and that the earth is a sphere
bool SimTRX_CheckIfLinkExists(unsigned int ground_station_index, double gs_transmit_power_dBm){
    int err = 0;
    gps_record_t rec;
    err = SimSTK_GetCurrentStkSatPosition(&rec);
    if(0 != err){
        TRACE_ERROR(SimTRX_CheckIfLinkExists ->SimSTK_GetCurrentStkSatPosition,err);
        return err;
    }

    ECEF_location_t sat_loc;
    memcpy(&sat_loc,&rec.position,sizeof(sat_loc));

    ECEF_location_t earth_center = STK_EARTH_COORDINATE_CARTESIAN;
    ECEF_location_t gs_loc;
    memcpy(&gs_loc,&gs_locations[ground_station_index],sizeof(gs_loc));

    vector_t v1 = GnrHelper_VecMinus(*(vector_t*)&earth_center,*(vector_t*)&gs_loc);
    vector_t v2 = GnrHelper_VecMinus(*(vector_t*)&sat_loc,*(vector_t*)&gs_loc);
    v1 = GnrHelper_GetUnitVector(v1);
    v2 = GnrHelper_GetUnitVector(v2);

    double deg = 0;
    deg = GnrHelper_VecMult(v1,v2);
    deg = acos(deg) * 180 / M_PI;
    double mag = GnrHelper_CalcMagnitude(*(vector_t*)&gs_loc);
    double crit_ang = asin(STK_EARTH_RADIUS_km/ mag) * 180 / M_PI;

    bool geometric_condition = false;
    if(deg >= crit_ang && deg <= 180) {
        geometric_condition = true;
    }
    bool link_condition = false;
    double rssi = SimTRX_CalcRSSI(ground_station_index,gs_transmit_power_dBm);
    if(rssi < TRX_MIN_SAT_LINK_POWER_dBm){
        link_condition = false;
    }
    return geometric_condition && link_condition;
}
