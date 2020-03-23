#include "FRAM.h"
#include "OBC_ConsumptionStates.h"

#include "../SimFiles/SimConfigFiles/SimulationConfigurations.h"
#include "../SimFiles/SimEPS.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAM_DEVICE_ID 0x42
unsigned char fram_buffer[FRAM_MAX_ADDRESS] = {0};
FRAMblockProtect protected_blocks;

bool has_fram_started = false;
pthread_mutex_t mutex_fram_lock;

int FRAM_start(void){
    if(has_fram_started){
       return 0;
    }
    int err = pthread_mutex_init(&mutex_fram_lock,NULL);
    if(0 != err){
        return -1;
    }
    SimEPS_SetSubsysState(SUBSYS_OBC,OBC_CONSMP_FRAM_CONSUMPTION, true);
    has_fram_started = true;
    return 0;
}

void FRAM_stop(void){
    if(!has_fram_started){
        return;
    }
    SimEPS_SetSubsysState(SUBSYS_OBC,OBC_CONSMP_FRAM_CONSUMPTION, false);
    pthread_mutex_destroy(&mutex_fram_lock);
    has_fram_started = false;
}

int FRAM_write(unsigned char *data, unsigned int address, unsigned int size){
    if(NULL == data){
        return 0;
    }
    if(address + size > FRAM_MAX_ADDRESS){
        return -2;
    }
    unsigned int querter_fram_address = FRAM_MAX_ADDRESS / 4;
   switch (protected_blocks.fields.blockProtect){
       case 0:                                      // No restrictions
           break;
       case 1:
           if(address > 3 * querter_fram_address){  // top 1/4 addresses
               return -1;
           }
           break;
       case 2:
           if(address > 2 * querter_fram_address){  // top 1/2 addresses
               return -1;
           }
           break;
       case 3:                                      // no writing
           return -1;

   }

    pthread_mutex_lock(&mutex_fram_lock);
        memcpy(fram_buffer+address,data,size);
    pthread_mutex_unlock(&mutex_fram_lock);
    return 0;
}

int FRAM_read(unsigned char *data, unsigned int address, unsigned int size){
    if(NULL == data){
        return 0;
    }
    if(address + size > FRAM_MAX_ADDRESS){
        return -2;
    }
    pthread_mutex_lock(&mutex_fram_lock);
        memcpy(data,fram_buffer + address,size);
    pthread_mutex_unlock(&mutex_fram_lock);
    return 0;
}

int FRAM_writeAndVerify(unsigned char *data, unsigned int address, unsigned int size){

    int err = FRAM_write(data,address,size);
    if(0!= err){
        return err;
    }
    unsigned char *buff = malloc(size);
    err = FRAM_read(buff,address,size);
    if(0!= err){
        return err;
    }
    err = memcmp(buff,data,size);
    if(0 != err){
        return  -3;
    }
    return 0;
}

int FRAM_protectBlocks(FRAMblockProtect blocks){
    pthread_mutex_lock(&mutex_fram_lock);
        protected_blocks = blocks;
    pthread_mutex_unlock(&mutex_fram_lock);
    return 0;
}

int FRAM_getProtectedBlocks(FRAMblockProtect* blocks){
    pthread_mutex_lock(&mutex_fram_lock);
        *blocks = protected_blocks;
    pthread_mutex_unlock(&mutex_fram_lock);
    return 0;
}

int FRAM_getDeviceID(unsigned char *deviceID){
    return FRAM_DEVICE_ID;
}
