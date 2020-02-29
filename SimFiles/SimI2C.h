#ifndef SIMPROJECT_SIMI2C_H
#define SIMPROJECT_SIMI2C_H

#include <stdbool.h>
#include "SimulationConfigurations.h"

typedef  enum {
    I2C_ERR_SUCCESS,
    I2C_ERR_SUBSYS_ALREADY_EXISTS,
    I2C_ERR_MASTER_ALREADY_EXISTS,
    I2C_ERR_NULL_POINTER,
    I2C_ERR_INVALID_PARAM,
    I2C_ERR_FAIL
}I2C_ERR;

typedef enum{
    I2C_SLAVE,
    I2C_MASTER
}SlaveMaster;

// this function will handle further analysis of the message sent from the master internally
typedef int(*i2c_subsys_func_t)(char opcode, char* buffer, unsigned int length);

//returns buffer length for a command with opcode 'opcode'. Should be a deterministic function
typedef int(*i2c_get_buff_length_func_t)(char opcode, unsigned int *length);

typedef struct{
    bool exists;                                            // notes if a subsystem has been added to the simulator
    unsigned char i2c_addr;                                 // the unique I2C address to which this sub-system belong to
    SlaveMaster status;                                     // declaring if this subsystem is a Mater or a Slave (there can be only one master)
    i2c_subsys_func_t p_subsys_function;                    // the actual subsystem working function(should depend heavily on a ( switch(opcode) statement)
    i2c_get_buff_length_func_t p_buffer_length_function;    // returns the expected buffer length for a command with a specific opcode
    SatSubsystem sub_system;                                // which subsystem are we talking about
    char i2c_data_buffer[I2C_MAX_SDA_BUFFER_SIZE];          // holds data to be read from Slave by the Master
}I2cSubsystemData_t;

// to add a subsystem to the I2C simulation use this function
// USE THIS BEFORE STARTING THE I2C SIMULATION TO ALL RELEVANT SUBSYSTEMS
I2C_ERR SimI2C_AddSubSys(SatSubsystem subsys, I2cSubsystemData_t *i2c_subsys_data);


// start the I2C communication module simulation
I2C_ERR SimI2C_StartI2C();

// sends read request to 'subsys_dest'
// expects to read 'length' bytes into 'msg' buffer
// returns 0 on success, -1 on fail, -2 on line taken
int SimI2C_read(SatSubsystem subsys_dest, char* msg, unsigned int length);

// sends write message to 'subsys_dest'
// the message 'msg' is 'length' bytes long
// returns 0 on success, -1 on fail, -2 on line taken
int SimI2C_write(SatSubsystem subsys_dest, char* msg, unsigned int length);

#endif //SIMPROJECT_SIMI2C_H
