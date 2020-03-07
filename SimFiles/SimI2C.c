#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "SimI2C.h"
#include "SimEPS.h"
#include "SimConfigFiles/SimulationConfigurations.h"

#define GET_7BIT_ADDR(addr) ((addr) & 0x7F)
#define IsReadCommand(addr)   ((addr) & 0x80)

pthread_t slave_threads_id[SUBSYS_NUM_OF_SUBSYSTEMS];

bool finished_reading_data = false;
pthread_mutex_t mutex_sda_taken = NULL;
pthread_mutex_t mutex_handshake = NULL;
pthread_mutex_t mutex_operation_running = NULL; // if there is already an operation running(read/write command)
sem_t sem_scl = NULL;   // SCL clock is running
sem_t sem_ack = NULL;   // ack messenger
char sda_data; // the data written on the SDA will be stored here

bool master_exists = false;                                     // noting if there is already a master. only one can rule them all
I2cSubsystemData_t i2cSubsystems[SUBSYS_NUM_OF_SUBSYSTEMS];     // saves all subsystems data

char buffer[I2C_MAX_SDA_BUFFER_SIZE] = {0};                     // local buffer for i2c commands

static void SDA_ReadByte(char *byte){
    pthread_mutex_lock(&mutex_sda_taken);
        *byte = sda_data;
#if(I2C_USE_PRINTS ==1)
    printf("r_%c\n",*byte);
#endif
    pthread_mutex_unlock(&mutex_sda_taken);
}
static void SDA_WriteByte(char byte){
    pthread_mutex_lock(&mutex_sda_taken);
        sda_data = byte;
#if(I2C_USE_PRINTS==1)
    printf("w_%c\t",byte);
#endif
    pthread_mutex_unlock(&mutex_sda_taken);
}
static int WaitForAck(){
    int err = 0;    // 0 = ACK
    //const struct timespec tm = {.tv_sec = 10,.tv_nsec = 1000000000};// TODO: timeout should be depended on RTC
    //err = sem_timedwait(&sem_ack,&tm);
    err = sem_wait(&sem_ack);
    return err;
}
static void SendAck(){
    sem_post(&sem_ack);
}

// ---------------------------------------Master Side
int SimI2C_read(SatSubsystem subsys_dest, char *msg, unsigned int length){
    pthread_mutex_lock(&mutex_operation_running);
    sem_post(&sem_scl); // raise clock signal on the SCL
    pthread_mutex_lock(&mutex_handshake);
    char byte = GET_7BIT_ADDR(i2cSubsystems[subsys_dest].i2c_addr) | 0x80; // Set MSB to 1 and write to SDA

    SDA_WriteByte(byte);    // write i2c subsys addr
    WaitForAck();
    finished_reading_data = false;
    for (unsigned int i = 0; i < length; ++i) {
        pthread_mutex_unlock(&mutex_handshake);
        WaitForAck();
        SDA_ReadByte(&msg[i]);
    }
    finished_reading_data = true;
    pthread_mutex_unlock(&mutex_operation_running);
    return 0;
}

int SimI2C_write(SatSubsystem subsys_dest, char *msg, unsigned int length){
    pthread_mutex_lock(&mutex_operation_running);
    sem_post(&sem_scl); // raise clock signal on the SCL
    pthread_mutex_lock(&mutex_handshake);
    char byte = GET_7BIT_ADDR(i2cSubsystems[subsys_dest].i2c_addr) & 0x7F;
    SDA_WriteByte(byte); // Set MSB to 0 and write to SDA
    WaitForAck();

    for (unsigned int i = 0; i < length; ++i) {
        pthread_mutex_unlock(&mutex_handshake);
        SDA_WriteByte(msg[i]);
        WaitForAck();
    }
    pthread_mutex_unlock(&mutex_handshake);
    pthread_mutex_unlock(&mutex_operation_running);
    return 0;
}
// ---------------------------------------Slave Side
static void* I2cSlaveThread(void *param){
    int index = 0;
    if(NULL == param){
        TRACE_ERROR(I2cSlaveThread,I2C_ERR_INVALID_PARAM);
        return NULL;
    }
    char byte;
    char subsys_i2c_addr;
    unsigned int length = 0;
    I2cSubsystemData_t subsys_data;
    memcpy(&subsys_data, param, sizeof(subsys_data));

    while(true) {
        sem_wait(&sem_scl); // wait for clock signal to start
        sem_post(&sem_scl); // turnstile

        if(!SimEPS_IsSubSystemOn(subsys_data.sub_system)){
            SimThreadSleep(100);
            continue;
        }
        SDA_ReadByte(&subsys_i2c_addr);                    // read subsystem I2C address
        if(GET_7BIT_ADDR(subsys_i2c_addr) == GET_7BIT_ADDR(0x00)){        // if destination address is 0x00 then the command is a broadcast
            //TODO: handle I2C broadcast
            SimThreadSleep(100);
            continue;
        }

        if(GET_7BIT_ADDR(subsys_data.i2c_addr) != GET_7BIT_ADDR(subsys_i2c_addr)) {
            SimThreadSleep(100);    // sleep for 100 simulated [usec]
            continue;
        }

        sem_wait(&sem_scl);                     // stop from other sub-systems from interrupting
        SendAck();

        pthread_mutex_lock(&mutex_handshake);
        SDA_ReadByte(&byte);                    // command opcode
        SendAck();

        if(IsReadCommand(subsys_i2c_addr)){
            while(!finished_reading_data) {
                pthread_mutex_lock(&mutex_handshake);
                SDA_WriteByte(subsys_data.i2c_data_buffer[index]);
                index++;
                SendAck();   // TODO: Check if Slaves care about timeout
            }
            finished_reading_data = false;
            index = 0;
        }else{  // Write command
            subsys_data.p_buffer_length_function(byte,&length); // get length of buffer of command with opcode 'byte'
            for (unsigned int i = 0; i < length; ++i) {
                pthread_mutex_lock(&mutex_handshake);
                SDA_ReadByte(&subsys_data.i2c_data_buffer[i]);
                SendAck();
            }
            subsys_data.p_subsys_function(byte,subsys_data.i2c_data_buffer,length);
        }
    }
}

I2C_ERR SimI2C_AddSubSys(SatSubsystem subsys,I2cSubsystemData_t *i2c_subsys_data){
    if(NULL == i2c_subsys_data){
        return I2C_ERR_NULL_POINTER;
    }
    if((NULL == i2c_subsys_data->p_subsys_function) ||
       (NULL == i2c_subsys_data->p_buffer_length_function)){
        return I2C_ERR_NULL_POINTER;
    }
    if (0x80 == (i2c_subsys_data->i2c_addr & 0x80)) {
        return I2C_ERR_INVALID_PARAM;
    }
    if ((0x00 == i2c_subsys_data->i2c_addr) ||
        (0xFF == i2c_subsys_data->i2c_addr)) {          // MSB should be 0 because it is reserved for Read/Write
        return I2C_ERR_INVALID_PARAM;
    }
    if(master_exists && (I2C_MASTER == i2c_subsys_data->status)){
        return I2C_ERR_MASTER_ALREADY_EXISTS;
    }
    if(i2cSubsystems[subsys].exists){
        return I2C_ERR_SUBSYS_ALREADY_EXISTS;
    }
    i2cSubsystems[subsys].status = i2c_subsys_data->status;
    i2cSubsystems[subsys].i2c_addr = i2c_subsys_data->i2c_addr;
    i2cSubsystems[subsys].exists = true;
    i2cSubsystems[subsys].p_subsys_function = i2c_subsys_data->p_subsys_function;
    i2cSubsystems[subsys].sub_system = subsys;
    i2cSubsystems[subsys].p_buffer_length_function = i2c_subsys_data->p_buffer_length_function;
    return  I2C_ERR_SUCCESS;
}

I2C_ERR SimI2C_StartI2C(){

    int err = 0;
    err = pthread_mutex_init(&mutex_sda_taken,NULL);
    TRACE_ERROR(pthread_mutex_init mutex_sda_taken,err);

    err = sem_init(&sem_scl,0,0);
    TRACE_ERROR(sem_init sem_scl,err);

    err = sem_init(&sem_ack,0,0);
    TRACE_ERROR(sem_init sem_ack,err)

    err = pthread_mutex_init(&mutex_operation_running,NULL);
    TRACE_ERROR(pthread_mutex_init mutex_operation_running,err)

    err = pthread_mutex_init(&mutex_handshake,NULL);
    TRACE_ERROR(pthread_mutex_init mutex_handshake,err)

    for (int i = 0; i < SUBSYS_NUM_OF_SUBSYSTEMS; ++i) {
        if(i2cSubsystems[i].exists && I2C_SLAVE == i2cSubsystems[i].status){
            err = pthread_create(&slave_threads_id[i], NULL, I2cSlaveThread, (void*)&i2cSubsystems[i]);
            TRACE_ERROR(pthread_create,err)
        }
    }
    return err;
}

#pragma clang diagnostic pop