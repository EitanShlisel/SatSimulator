
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
#include "SimFiles/GenericHelpFunctions.h"
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
        case '-':
            *length = 0;
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
    char str[] = "abcdefghijklmnopqrstuvwxyz";
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
        case '-':
            memcpy(buffer,str,strlen(str)-1);
            printf("executed the '-' command  \n");
            break;
        case '+':
            printf("the meaning of life is: %.*s\n",length,buffer);
            break;
        default:
            return 0;
    }
    printf("\n");
    return 0;
}

void I2CTest(){
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
    TRACE_RETURN_ON_ERROR(SimI2C_StartI2C,0,err,);
}

int main(){
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
    char msg[] = {'-'};
    char msg2[] = "+0124256789";
    char buff[100] ={0};
    SimI2C_write(SUBSYS_EPS,msg2,11);
    //SimI2C_write(SUBSYS_EPS,msg,1);
    //SimI2C_read(SUBSYS_EPS,buff,26);
    printf("this is the buffer = %s\n",buff);

    return 0;
}
