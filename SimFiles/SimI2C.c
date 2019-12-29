#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "SimI2C.h"
#include "SimulationConfigurations.h"

pthread_mutex_t mutex_sda_taken = NULL;

typedef struct{
    bool exists;
    bool is_subsys_on;  // determined by EPS simulation
    unsigned char i2c_addr;
    SlaveMaster status;
    i2c_subsys_func_t pfunc;
}I2cSubsystemData_t;

bool master_exists = false;                                     //noting if there is only ONE master. only one can rule them all
I2cSubsystemData_t i2cSubsystems[I2C_SUBSYS_NUM_OF_SUBSYSTEMS]; // saves all subsystems data
unsigned char I2cSDABuffer[I2C_MAX_SDA_BUFFER_SIZE] = { 0 };            // data written on the SDA will be saved here

I2C_ERR SimI2C_AddSubSys(I2C_Subsystem subsys, SlaveMaster status, unsigned char subsys_addr, i2c_subsys_func_t pfunc){
    if(NULL == pfunc){
        return I2C_ERR_NULL_POINTER;
    }
    if (0x80 == (subsys_addr & 0x80)) {
        return I2C_ERR_INVALID_PARAM;
    }
    if (0x00 == subsys_addr || 0xFF == subsys_addr) {          // MSB should be 0 because it is reserved for Read/Write
        return I2C_ERR_INVALID_PARAM;
    }
    if(master_exists && I2C_MASTER == status){
        return I2C_ERR_MASTER_EXISTS;
    }
    if(i2cSubsystems[subsys].exists){
        return I2C_ERR_SUBSYS_EXISTS;
    }
    i2cSubsystems[subsys].status = status;
    i2cSubsystems[subsys].i2c_addr = subsys_addr;
    i2cSubsystems[subsys].exists = true;
    i2cSubsystems[subsys].is_subsys_on = false; // until decided otherwise
    i2cSubsystems[subsys].pfunc = pfunc;

    return  I2C_ERR_SUCCESS;
}

static void SimI2C_I2cThread(void* param){

    int err = 0;
    unsigned int msg_length = 0;
    while(1){
        // TODO: check if a message was written by the master on the SDA
        // TODO: Send message to all slaves

        for (int i = 0; i < I2C_SUBSYS_NUM_OF_SUBSYSTEMS; ++i) {
            if(i2cSubsystems[i].exists){
                err = i2cSubsystems[i].pfunc(I2cSDABuffer,msg_length);
                TRACE_ERROR(i2cSubsystems[i].pfunc, err);
            }
        }
    }
}

I2C_ERR SimI2C_StartI2C(){
    //TODO: start I2C thread
    int err = 0;
    err = pthread_mutex_init(&mutex_sda_taken,NULL);
    TRACE_ERROR(pthread_mutex_init,err);
    return err;
}

int SimI2C_read(I2C_Subsystem subsys_dest, unsigned char* msg, unsigned int length){
    return 0;
}

int SimI2C_write(I2C_Subsystem subsys_dest, unsigned char* msg, unsigned int length){
    return 0;
}

