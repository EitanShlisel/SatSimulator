
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "SimFiles/SimulationConfigurations.h"
#include "SimFiles/TimeHelperFunctions.h"
#include "SubsystemModules/Time.h"
#include "SubsystemModules/RTC.h"
#include "SubsystemModules/GPS_Module.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimRTC.h"
#include "SimFiles/SimSTK.h"
#include "SimFiles/SimI2C.h"
#include <stdbool.h>
#include <pthread_time.h>
#include <unistd.h>

int TestSubsysGetBuff(char opcode, unsigned int *length){
    switch (opcode)
    {
        case 0x00:
            *length = 10;
            break;
        case 0x01:
            *length = 20;
            break;
        case 0x02:
            *length = 30;
            break;
        case 0x03:
            *length = 40;
            break;
        case 0x42:
            *length = 10;
            break;
        case '+':
            *length = 10;
            break;
        default:
            return 0;
    }
}

int TestSubSysFunc(char opcode, char *buffer, unsigned int length) {
    if(NULL == buffer){
        return I2C_ERR_NULL_POINTER;
    }
    printf("opcode :%d\n",opcode);
    switch(opcode){
        case 0x00:
            printf("zero'th message: %.*s\n",length,buffer);
            break;
        case 0x01:
            printf("first message: %.*s\n",length,buffer);
            break;
        case 0x02:
            printf("second message: %.*s\n",length,buffer);
            break;
        case 0x03:
            printf("third message: %.*s\n",length,buffer);
            break;
        case 0x04:
            printf("forth message: %.*s\n",length,buffer);
            break;
        case '+':
            printf("the meaning of life is: %.*s\n",length,buffer);
            break;
        default:
            return 0;
    }
    return 0;
}

int main() {
    int err = 0;
    Time t;

    UnixToTime(&t,RTC_DEFAULT_START_TIME);
    err = Time_start(&t,0);
    TRACE_ERROR(RTC_start,err);

    I2cSubsystemData_t i2c_data = {.p_buffer_length_function = TestSubsysGetBuff,
                                   .p_subsys_function = TestSubSysFunc,
                                   .status=I2C_SLAVE,
                                   .i2c_addr = 0x42};
    I2cSubsystemData_t i2c_gps_data = {.p_buffer_length_function = TestSubsysGetBuff,
            .p_subsys_function = TestSubSysFunc,
            .status=I2C_SLAVE,
            .i2c_addr = 0x60};
    err = SimI2C_AddSubSys(SUBSYS_EPS,&i2c_data);
    TRACE_ERROR(SimI2C_AddSubSys,err);
    err = SimI2C_AddSubSys(SUBSYS_GPS,&i2c_gps_data);
    TRACE_ERROR(SimI2C_AddSubSys,err);

    err = SimI2C_StartI2C();
    TRACE_ERROR(SimI2C_StartI2C,err);
    TRACE_RETURN_ON_ERROR(SimI2C_StartI2C,0,err,-666);

    char msg[40] ="+1234567890";
    char msg2[40] ="+0987654321";
    unsigned int length = strlen(msg);
    while(true){
        SimI2C_write(SUBSYS_EPS,msg,length);
        SimI2C_write(SUBSYS_GPS,msg2,length);
        printf("Master: wrote message\n");
        sleep(10);
    }
//    err = GPS_Init();
//    TRACE_ERROR(GPS_Init,err);
//    sun_vec_t vec;
//    gps_record_t rec;
//
//    while(1){
//        printf("Sat Time = %lf\n",SimRtc_GetSatTime());
//        err = SimStk_GetCurrentStkSunVec(&vec);
//        TRACE_ERROR(SimStk_GetCurrentStkSunVec,err);
//        //SimSTK_PrintSunVec(&vec);
//        err = SimSTK_GetCurrentStkSatPosition(&rec);
//        TRACE_ERROR(SimSTK_GetCurrentStkSatPosition,err);
//        SimSTK_PrintfSatPos(&rec);
//        sleep(1);
//    }
    return 0;
}
