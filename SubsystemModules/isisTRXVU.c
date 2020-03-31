#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hal/errors.h"
#include "isisTRXVU.h"
#include "I2C.h"


ISIStrxvuI2CAddress *trxvu_addresses = NULL;
ISIStrxvuFrameLengths *trxvu_maxFrameLengths = NULL;
ISIStrxvuBitrate *trxvu_default_bitrates = NULL;
unsigned char number_of_trx = 0;

bool *is_initialised = NULL;        // is a trxvu subsystem initialised or not

// done
int IsisTrxvu_initialize(ISIStrxvuI2CAddress *address, ISIStrxvuFrameLengths *maxFrameLengths, ISIStrxvuBitrate* default_bitrates, unsigned char number){

    if(NULL != is_initialised){
        return E_IS_INITIALIZED;
    }

    if( (NULL == address) ||
        (NULL == maxFrameLengths) ||
        (NULL == default_bitrates )){
        return E_INPUT_POINTER_NULL;
    }

    trxvu_addresses = (ISIStrxvuI2CAddress*)malloc(number * sizeof(*trxvu_addresses));
    trxvu_maxFrameLengths = (ISIStrxvuFrameLengths*)malloc(number * sizeof(*trxvu_maxFrameLengths));
    trxvu_default_bitrates = (ISIStrxvuBitrate*)malloc(number * sizeof(*trxvu_default_bitrates));
    if( (NULL == trxvu_addresses) ||
        (NULL == trxvu_maxFrameLengths) ||
        (NULL == trxvu_default_bitrates )){
        return E_MEM_ALLOC;
    }
    memcpy(trxvu_addresses,address,number * sizeof(*trxvu_addresses));
    memcpy(trxvu_maxFrameLengths,address,number * sizeof(*trxvu_maxFrameLengths));
    memcpy(trxvu_default_bitrates,address,number * sizeof(*trxvu_default_bitrates));
    number_of_trx = number;

    int err = 0;
    for (unsigned char i = 0; i < number_of_trx; ++i) {
        //TODO: complete from documents
        err = I2C_write(trxvu_addresses[i].addressVu_rc,NULL,0);
        if(0 != err){
            return -1;
        }
        err = I2C_write(trxvu_addresses[i].addressVu_tc,NULL,0);
        if(0 != err){
            return -1;
        }
    }
    is_initialised = (bool*)malloc(number_of_trx * sizeof(*is_initialised));
    if(NULL == is_initialised){
        return E_MEM_ALLOC;
    }
    memset(is_initialised,true,number_of_trx);
    return 0;
}
//done
int IsisTrxvu_componentSoftReset(unsigned char index, ISIStrxvuComponent component){
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = 0;
    switch (component){
        case trxvu_rc:
            addr = trxvu_addresses[index].addressVu_rc;
            break;
        case trxvu_tc:
            addr = trxvu_addresses[index].addressVu_tc;
            break;
        default:
            return 0;
    }
    int err = I2C_write(addr,0xAA,1);//TODO: not corrent data
    return err;
}
//done
int IsisTrxvu_componentHardReset(unsigned char index, ISIStrxvuComponent component){
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = 0;
    switch (component){
        case trxvu_rc:
            addr = trxvu_addresses[index].addressVu_rc;
            break;
        case trxvu_tc:
            addr = trxvu_addresses[index].addressVu_tc;
            break;
        default:
            return 0;
    }
    int err = I2C_write(addr,0xAB,1);//TODO: not corrent data
    return err;
}
//done
int IsisTrxvu_softReset(unsigned char index){
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    int err = 0;
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    err = I2C_write(addr,0xAA,1);
    if(0 != err){
        return err;
    }
    addr = trxvu_addresses[index].addressVu_tc;
    err = I2C_write(addr,0xAA,1);
    if(0 != err){
        return err;
    }
    return err;
}
//done
int IsisTrxvu_hardReset(unsigned char index){
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    int err = I2C_write(addr,0xAB,1);   // make sure that in simulated reset both RX and TX will reset
    return err;
}
//done
int IsisTrxvu_tcSendAX25DefClSign(unsigned char index, unsigned char *data, unsigned char length, unsigned char *avail){
    if(0 != data){
        return E_INPUT_POINTER_NULL;
    }
    if(0 != avail){
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_tc;
    unsigned char *buff = malloc(length + 1);
    if(NULL == buff){
        return E_MEM_ALLOC;
    }
    buff[0] = 0x10;
    memcpy(buff + 1,data,length);
    int err = I2C_write(addr,buff,1 + length);
    free(buff);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,avail,sizeof(avail));
    if(0 != err){
        return err;
    }
    return err;
}
//done
int IsisTrxvu_tcSendAX25OvrClSign(unsigned char index, unsigned char *fromCallsign, unsigned char *toCallsign, unsigned char *data, unsigned char length, unsigned char *avail){
    if((0 != data) || (0 != avail) ||(0 != toCallsign) ||(0 != fromCallsign)) {
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_tc;
    unsigned char *buff = malloc(length+8+8+1);//8 for fromCallsign. 8 for toCallsign
    if(NULL == buff){
        return E_MEM_ALLOC;
    }
    buff[0] = 0x11;
    memcpy(buff + 1,fromCallsign,length);
    memcpy(buff + 1+8,toCallsign,length);
    memcpy(buff + 1+8+8,data,length);
    int err = I2C_write(addr,buff,length+8+8+1);
    free(buff);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,avail,sizeof(avail));
    if(0 != err){
        return err;
    }
    return err;
}
int IsisTrxvu_tcSetAx25BeaconDefClSign(unsigned char index, unsigned char *data, unsigned char length, unsigned short interval){
    return 0;
}
int IsisTrxvu_tcSetAx25BeaconOvrClSign(unsigned char index, unsigned char *fromCallsign, unsigned char *toCallsign, unsigned char *data, unsigned char length, unsigned short interval){
    return 0;
}
int IsisTrxvu_tcClearBeacon(unsigned char index){
    return 0;
}
int IsisTrxvu_tcSetDefToClSign(unsigned char index, unsigned char *toCallsign){
    return 0;
}
int IsisTrxvu_tcSetDefFromClSign(unsigned char index, unsigned char *fromCallsign){
    return 0;
}
int IsisTrxvu_tcSetIdlestate(unsigned char index, ISIStrxvuIdleState state){
    return 0;
}
int IsisTrxvu_tcSetAx25Bitrate(unsigned char index, ISIStrxvuBitrate bitrate){
    return 0;
}
int IsisTrxvu_tcGetUptime(unsigned char index, unsigned int *uptime){
    return 0;
}
int IsisTrxvu_tcGetState(unsigned char index, ISIStrxvuTransmitterState *currentvutcState){
    return 0;
}
int IsisTrxvu_tcGetTelemetryAll(unsigned char index, ISIStrxvuTxTelemetry *telemetry){
    return 0;
}
int IsisTrxvu_tcGetTelemetryAll_revC(unsigned char index, ISIStrxvuTxTelemetry_revC *telemetry){
    return 0;
}
int IsisTrxvu_tcGetLastTxTelemetry(unsigned char index, ISIStrxvuTxTelemetry *last_telemetry){
    return 0;
}
int IsisTrxvu_tcGetLastTxTelemetry_revC(unsigned char index, ISIStrxvuTxTelemetry_revC *last_telemetry){
    return 0;
}
unsigned short IsisTrxvu_tcEstimateTransmissionTime(unsigned char index, unsigned char length){
    return 0;
}
int IsisTrxvu_rcGetFrameCount(unsigned char index, unsigned short *frameCount){
    if(NULL == frameCount){
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    int err = I2C_write(addr,0x21,1);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,frameCount,2);
    return 0;
}
int IsisTrxvu_rcGetCommandFrame(unsigned char index, ISIStrxvuRxFrame *rx_frame){
    if(NULL == rx_frame){
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    int err = I2C_write(addr,0x22,1);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,(char*)rx_frame, 6 + 200); // 6 from ISIStrxvuRxFrame and 200 data
    if(0 != err){
        return err;
    }
    return 0;
}
int IsisTrxvu_rcGetTelemetryAll(unsigned char index, ISIStrxvuRxTelemetry *telemetry){
    return 0;
}
//done
int IsisTrxvu_rcGetTelemetryAll_revC(unsigned char index, ISIStrxvuRxTelemetry_revC *telemetry){
    if(NULL == telemetry){
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    int err = I2C_write(addr,0x1A,1);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,(unsigned char*)telemetry, sizeof(*telemetry));
    if(0 != err){
        return err;
    }
    return 0;
}
//done
int IsisTrxvu_rcGetUptime(unsigned char index, unsigned int *uptime){
    if(NULL == uptime){
        return E_INPUT_POINTER_NULL;
    }
    if(NULL != is_initialised){
        if(is_initialised[index] == false){
            return E_NOT_INITIALIZED;
        }
    }
    unsigned char addr = trxvu_addresses[index].addressVu_rc;
    int err = I2C_write(addr,0x40,1);
    if(0 != err){
        return err;
    }
    err = I2C_read(addr,(unsigned char*)uptime, sizeof(*uptime));
    if(0 != err){
        return err;
    }
    return 0;
}