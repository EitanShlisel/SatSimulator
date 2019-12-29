#ifndef SIMPROJECT_SIMI2C_H
#define SIMPROJECT_SIMI2C_H


// to add a subsystem to the simulation add an element to this enum!
typedef enum {
    I2C_SUBSYS_OBC = 0,
    I2C_SUBSYS_EPS ,
    I2C_SUBSYS_GPS,
    I2C_SUBSYS_PAYLOAD,

    I2C_SUBSYS_NUM_OF_SUBSYSTEMS
}I2C_Subsystem;

typedef  enum {
    I2C_ERR_SUCCESS,
    I2C_ERR_SUBSYS_EXISTS,
    I2C_ERR_MASTER_EXISTS,
    I2C_ERR_NULL_POINTER,
    I2C_ERR_INVALID_PARAM,
    I2C_ERR_FAIL
}I2C_ERR;

typedef enum{
    I2C_SLAVE,
    I2C_MASTER
}SlaveMaster;


// this function will handle further analysis of the message sent from the master internally
typedef int(*i2c_subsys_func_t)(unsigned char*, unsigned int);

// to add a subsystem to the I2C simulation use this function
// USE THIS BEFORE STARTING THE I2C SIMULATION TO ALL RELEVANT SUBSYSTEMS
I2C_ERR SimI2C_AddSubSys(I2C_Subsystem subsys, SlaveMaster status, unsigned char subsys_addr, i2c_subsys_func_t pfunc);

// start the I2C communication module simulation
I2C_ERR SimI2C_StartI2C();

// sends read request to 'subsys_dest'
// expects to read 'length' bytes into 'msg' buffer
// returns 0 on success, -1 on fail, -2 on line taken
int SimI2C_read(I2C_Subsystem subsys_dest, unsigned char* msg, unsigned int length);

// sends write message to 'subsys_dest'
// the message 'msg' is 'length' bytes long
// returns 0 on success, -1 on fail, -2 on line taken
int SimI2C_write(I2C_Subsystem subsys_dest, unsigned char* msg, unsigned int length);

#endif //SIMPROJECT_SIMI2C_H
