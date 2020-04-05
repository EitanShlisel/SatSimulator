#ifndef SIMPROJECT_CMDTYPES_H
#define SIMPROJECT_CMDTYPES_H

#include <stdbool.h>

#define MAX_CMD_NAME_LENGTH 30
#define CMD_PARAM_LENGTH 30
#define MAX_DESCRIPTION_LENGTH 150
#define CMD_PARAM_VALUE_LENGTH 50
#define MAX_INPUT_LENGTH 300

typedef bool(*parse_func_f)(char[CMD_PARAM_VALUE_LENGTH], void*);
typedef enum{
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING
}param_type;

typedef struct{
    char param_name[CMD_PARAM_LENGTH];
    char param_value[CMD_PARAM_VALUE_LENGTH];
}cmd_input_param_t;


typedef struct{
    char command[MAX_CMD_NAME_LENGTH];                               // command name
    cmd_input_param_t *parameters;
    unsigned int num_of_parameters;
}user_input_t;

typedef struct{
    char param_name[CMD_PARAM_LENGTH];
    param_type type;
    bool mandatory;
}cmd_param_t;
typedef int(*cmd_func_f)(char input[MAX_INPUT_LENGTH]);

typedef struct{
    char opcode[MAX_CMD_NAME_LENGTH];
    cmd_param_t *args;
    unsigned int num_of_args;
    cmd_func_f func;
}cmd_t;

#endif //SIMPROJECT_CMDTYPES_H
