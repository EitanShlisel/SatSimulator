#include "SimCmdLine.h"
#include "../SimFiles/SimConfigFiles/threads.h"
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool StrToInt8(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i]) && ('-' != param[0])){
            return false;
        }
    }

    char val;
    long long int res = atoll(param);
    if(res > CHAR_MAX || res < CHAR_MIN){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToInt16(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i]) && ('-' != param[0])){
            return false;
        }
    }
    short val;
    long long int res = atoll(param);
    if(res > SHRT_MAX || res < SHRT_MIN){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToInt32(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i]) && ('-' != param[0])){
            return false;
        }
    }
    int val;
    long long int res = atoll(param);
    if(res > INT_MAX || res < INT_MIN){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToInt64(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i]) && ('-' != param[0])){
            return false;
        }
    }
    long long int val;
    long long int res = atoll(param);
    if(res > LONG_LONG_MAX || res < LONG_LONG_MIN){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
}
bool StrToUInt8(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i])){
            return false;
        }
    }
    unsigned char val;
    long long int res = atoll(param);
    if(res > UCHAR_MAX || res < 0){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToUInt16(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i])){
            return false;
        }
    }
    unsigned short val;
    long long int res = atoll(param);
    if(res > USHRT_MAX || res < 0){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToUInt32(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i])){
            return false;
        }
    }
    unsigned int val;
    long long int res = atoll(param);
    if(res > UINT_MAX || res < 0){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToUInt64(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    for (unsigned int i = 0; (i < strlen(param)) && (i<CMD_PARAM_VALUE_LENGTH); ++i) {
        if(!isdigit(param[i])){
            return false;
        }
    }
    unsigned long long int val;
    long long int res = atoll(param);
    if(res > ULONG_LONG_MAX || res < 0){
        return false;
    }
    memcpy(value,(void*)&res,sizeof(val));
    return true;
}
bool StrToDouble(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    char* endptr = 0;
    strtod(param, &endptr);
    if(*endptr != '\0' || endptr == param)
        return false;
    double val = atof(param);
    memcpy(value,(void*)&val,sizeof(val));
    return true;
}
bool StrToStr(char param[CMD_PARAM_VALUE_LENGTH], void *value){
    if(strlen(param)>CMD_PARAM_VALUE_LENGTH){
        return false;
    }
    memcpy(value,(void*)param,strlen(param));
    return true;
}
parse_func_f CmdGetParseFuncByType(param_type type){
    switch(type){
        case TYPE_INT8:
            return StrToInt8;
        case TYPE_INT16:
            return StrToInt16;
        case TYPE_INT32:
            return StrToInt32;
        case TYPE_INT64:
            return StrToInt64;
        case TYPE_UINT8:
            return StrToUInt8;
        case TYPE_UINT16:
            return StrToUInt16;
        case TYPE_UINT32:
            return StrToUInt32;
        case TYPE_UINT64:
            return StrToUInt64;
        case TYPE_FLOAT:
            return StrToDouble;
        case TYPE_DOUBLE:
            return StrToDouble;
        case TYPE_STRING:
            return (parse_func_f)StrToStr;
        default:
            return NULL;
    }
}

void remove_spaces(char *s, char **out) {
    unsigned int len = strlen(s);
    *out = calloc(strlen(s),sizeof(char));
    unsigned int j = 0;
    for (unsigned int i = 0; i < len; ++i) {
        if (s[i] != ' '){
            (*out)[j] = s[i];
            j++;
        }
    }
}
void ParseParam(char *str_param, cmd_input_param_t *param){
    char *buff;
    remove_spaces(str_param, &buff);
    sscanf(buff,"'%[^=']'", param->param_name);
    sscanf(str_param,"%*[^=]=%*[ ]'%[^']'",param->param_value);
    free(buff);
}
void ParseArgsIntoParams(char input[MAX_INPUT_LENGTH], cmd_input_param_t **params, unsigned int *num_of_params){
    if(strlen(input) == 0){
        *num_of_params = 0;
        return;
    }
    *num_of_params = 1;
    for (unsigned int i = 0; i < strlen(input); ++i) {
        if(input[i] == ','){
            (*num_of_params)++;
        }
    }
    *params = (cmd_input_param_t*)calloc(*num_of_params , sizeof(cmd_input_param_t));
    char inp[MAX_INPUT_LENGTH];
    strcpy(inp,input);

    char *pt = inp;
    pt = strtok (inp,",");
    unsigned int i = 0;
    while (pt != NULL) {
        ParseParam(pt,&(*params)[i]);
        pt = strtok (NULL, ",");
        i++;
    }
}
void ParseLine(char input[MAX_INPUT_LENGTH], user_input_t *cmd){
    char args[MAX_INPUT_LENGTH] = {0};
    sscanf(input,"%[^(](%[^)]",cmd->command, args);
    ParseArgsIntoParams(args,&cmd->parameters,&cmd->num_of_parameters);
}

void* cmdLineThread(void* args){
    while(1){


    }
    return NULL;
}
void CmdLineStart(){
    thread_handle_t thread_cmd;
    thread_create(&thread_cmd,NULL,cmdLineThread,NULL);
}

void CmdTest(){
    char line[] = "command_name('param1' =    'val1', 'param2' = 'val2','param4' = 'stringo star')";
    user_input_t cmd;
    ParseLine(line,&cmd);
    for (unsigned int i = 0; i < cmd.num_of_parameters; ++i) {
        printf("name = %s\n",cmd.parameters[i].param_name);
        printf("value = %s\n",cmd.parameters[i].param_value);
    }
    free(cmd.parameters);
    printf("done\n");
}