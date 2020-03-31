#include "I2C.h"
#include "../SimFiles/SimI2C.h"
#include "../SimFiles/SimEPS.h"
#include "OBC_ConsumptionStates.h"

int I2C_start(unsigned int i2cBusSpeed_Hz, unsigned int i2cTransferTimeout){
    SimI2C_StartI2C();
    SimEPS_SetSubsysState(SUBSYS_OBC,OBC_CONSP_I2C_BUS,true);
    return 0;
}
void I2C_setTransferTimeout(unsigned int i2cTransferTimeout){
    //TODO: in future revisions
}
void I2C_stop(void){
    SimI2C_StopI2C();
    SimEPS_SetSubsysState(SUBSYS_OBC,OBC_CONSP_I2C_BUS,false);

}
//int I2C_blockBus(portTickType timeout);

void I2C_releaseBus(void){
    //TODO: do this
}
int I2C_write(unsigned int slaveAddress, unsigned char *data, unsigned int size){
    if(!SimI2C_GetBusInitialised()){
        return -2;
    }
    if(NULL == data){
        return -2;
    }
    if(SUBSYS_VOID == SimI2C_GetSubsystemFromAddress(slaveAddress)){
        return 4;
    }
    int err = SimI2C_write(slaveAddress,(char*)data,size);
    if(0 != err){
        return 4;
    }
    return 0;
}

int I2C_read(unsigned int slaveAddress, unsigned char *data, unsigned int size){
    if(!SimI2C_GetBusInitialised()){
        return -3;
    }
    if(SUBSYS_VOID == SimI2C_GetSubsystemFromAddress(slaveAddress)){
        return 4;
    }
    int err = SimI2C_read(slaveAddress,(char*)data,size);
    if(0 != err){
        return 5;
    }
    return 0;
}
int I2C_writeRead(I2Ctransfer *tx){
    if(NULL == tx){
        return -2;
    }
    int err = 0;
    SatSubsystem subsys = SimI2C_GetSubsystemFromAddress(tx->slaveAddress);
    if((NULL != tx->writeData) && (0 != tx->writeSize)){
        if(SUBSYS_VOID == subsys){
            return 4;
        }
        err = SimI2C_write(subsys,(char*)tx->readData,tx->readSize);
        if(0 != err){
            return 4;
        }
    }
    if((NULL != tx->readData) && (0 != tx->readSize)){
        if(SUBSYS_VOID == subsys){
            return 5;
        }
        err = SimI2C_write(subsys,(char*)tx->readData,tx->readSize);
        if(0 != err){
            return 5;
        }
    }
    return 0;
}
int I2C_queueTransfer(I2CgenericTransfer *tx){
    return 0;
}

I2CdriverState I2C_getDriverState(void){
    return 0;
}
I2CtransferStatus I2C_getCurrentTransferStatus(void){
    return 0;
}

