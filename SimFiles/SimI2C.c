#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "SimI2C.h"
#include "SimEPS.h"
#include "SimulationConfigurations.h"

#define ACK_BIT 0x01
#define START_BIT 0XFF
#define STOP_BIT 0X00
#define GET_7BIT_ADDR(addr) ((addr) & 0x7F)
#define IsReadCommand(addr)   ((addr) & 0x80)
typedef struct{
    bool exists;
    char i2c_addr;
    SlaveMaster status;
    i2c_subsys_func_t p_subsys_function;
    i2c_buff_length_func_t p_buffer_length_function;
    SatSubsystem sub_system;
}I2cSubsystemData_t;

pthread_t slave_threads_id[SUBSYS_NUM_OF_SUBSYSTEMS];
pthread_t master_thread_id;

pthread_mutex_t mutex_sda_taken = NULL;
sem_t sem_data_on_sda = NULL;
sem_t sem_scl = NULL;   // SCL clock is running
sem_t sem_ack = NULL;   // ack messenger
char sda_data; // the data written on the SDA will be stored here

bool master_exists = false;                                     // noting if there is already a master. only one can rule them all
I2cSubsystemData_t i2cSubsystems[SUBSYS_NUM_OF_SUBSYSTEMS];     // saves all subsystems data
char buffer[I2C_MAX_SDA_BUFFER_SIZE] = {0};                     // local buffer for i2c commands

static void SDA_ReadByte(char *byte){
    sem_wait(&sem_data_on_sda);
    pthread_mutex_lock(&mutex_sda_taken);
        *byte = sda_data;
    pthread_mutex_unlock(&mutex_sda_taken);
    sem_post(&sem_data_on_sda);
}
static void SDA_WriteByte(char byte){
    pthread_mutex_lock(&mutex_sda_taken);
        sda_data = byte;
        sem_post(&sem_data_on_sda);
    pthread_mutex_unlock(&mutex_sda_taken);
}
static int WaitForAck(){
    int err = 0;    // 0 = ACK
    const struct timespec tm = {.tv_sec = 0,.tv_nsec = 1000000000};// TODO: timeout should be depended on RTC
    err = sem_timedwait(&sem_ack,&tm);
    return err;
}
static void SendAck(){
    sem_post(&sem_ack);
}

// ---------------------------------------Master Side
int SimI2C_read(SatSubsystem subsys_dest, char *msg, unsigned int length){

    sem_post(&sem_scl); // raise clock signal on the SCL
    SDA_WriteByte(GET_7BIT_ADDR(i2cSubsystems[subsys_dest].i2c_addr) | 0x80);   // Set MSB to 1 and write to SDA
    int err = WaitForAck();
    if(0 != err){
        TRACE_ERROR(SimI2C_read,err);
        return err;
    }
    for (unsigned int i = 0; i < length; ++i) {
        SDA_ReadByte(&msg[i]);
        SendAck();
    }
    return 0;
}

int SimI2C_write(SatSubsystem subsys_dest, char *msg, unsigned int length){
    sem_post(&sem_scl); // raise clock signal on the SCL
    SDA_WriteByte(GET_7BIT_ADDR(i2cSubsystems[subsys_dest].i2c_addr) & 0x7F);// Set MSB to 0 and write to SDA
    int err = WaitForAck();
    if(0 != err){
        TRACE_ERROR(SimI2C_write,err);
        return err;
    }
    for (unsigned int i = 0; i < length; ++i) {
        SDA_WriteByte(msg[i]);
        err = WaitForAck();
        if(0 != err){
            TRACE_ERROR(SimI2C_write,err);
            return err;
        }
    }
    return 0;
}

static void* I2cMasterThread(void *param){
    if(NULL == param){
        TRACE_ERROR(I2cMasterThread,I2C_ERR_INVALID_PARAM);
        return NULL;
    }
    return NULL;
}

// ---------------------------------------Slave Side
static void* I2cSlaveThread(void *param){
    if(NULL == param){
        TRACE_ERROR(I2cSlaveThread,I2C_ERR_INVALID_PARAM);
        return NULL;
    }
    char byte;
    unsigned int length = 0;
    I2cSubsystemData_t subsys_data;
    memcpy(&subsys_data, param, sizeof(subsys_data));
    while(true) {
        sem_wait(&sem_scl); // wait for clock signal to start
        sem_post(&sem_scl); // turnstile
        if(!SimEPS_IsSubSystemOn(subsys_data.sub_system)){
            SimThreadSleep(1000);
            continue;
        }
        SDA_ReadByte(&byte);
        //TODO: handle I2C broadcast
        if (GET_7BIT_ADDR(subsys_data.i2c_addr) != byte) {
            SimThreadSleep(1000);    // sleep for 1000 simulated usec
            continue;
        }
        sem_wait(&sem_scl);                     // stop from other sub-systems from interrupting
        sem_wait(&sem_data_on_sda);             // the data belongs to this subsystem
        SendAck();

        SDA_ReadByte(&byte); // command opcode
        SendAck();
        subsys_data.p_buffer_length_function(byte,&length); // get length of buffer of command with opcode 'byte'

        if(IsReadCommand(byte)){
            subsys_data.p_subsys_function(byte,buffer,length);  // reads 'length ' data into 'buffer' according to opcode 'byte'
            for (unsigned int i = 0; i < length; ++i) {
                SDA_WriteByte(buffer[i]);
                WaitForAck();   // TODO: Check if Slaves care about timeout
            }
        }else{  // Write command
            for (unsigned int i = 0; i < length; ++i) {
                SDA_ReadByte(&buffer[i]);
                SendAck();
            }
            subsys_data.p_subsys_function(byte,buffer,length);
        }
    }
}

I2C_ERR SimI2C_AddSubSys(SatSubsystem subsys, SlaveMaster status, unsigned char subsys_addr,
        i2c_subsys_func_t pfunc, i2c_buff_length_func_t buff_func){
    if(NULL == pfunc || NULL == buff_func){
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
    i2cSubsystems[subsys].p_subsys_function = pfunc;
    i2cSubsystems[subsys].sub_system = subsys;
    i2cSubsystems[subsys].p_buffer_length_function = buff_func;
    return  I2C_ERR_SUCCESS;
}

I2C_ERR SimI2C_StartI2C(){
    //TODO: start I2C thread
    int err = 0;
    err = pthread_mutex_init(&mutex_sda_taken,NULL);
    TRACE_ERROR(pthread_mutex_init,err);
    for (int i = 0; i < SUBSYS_NUM_OF_SUBSYSTEMS; ++i) {
        if(i2cSubsystems[i].exists && I2C_SLAVE == i2cSubsystems[i].status){
            err = pthread_create(&slave_threads_id[i], NULL, I2cSlaveThread, (void*)&i2cSubsystems[i]);
            TRACE_ERROR(pthread_create,err)
        }
    }
    err = pthread_create(&master_thread_id, NULL, I2cMasterThread, (void*)&i2cSubsystems[SUBSYS_OBC]);
    TRACE_ERROR(pthread_create,err)
    return err;
}

#pragma clang diagnostic pop