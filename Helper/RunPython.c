#include "RunPython.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EXEC_PY_PATH "..\\ExecPython\\ExecPython\\obj\\Debug\\ExecPython.exe"
void RunPythonScript(char script_path[MAX_FILE_PATH], char *args){

    unsigned int length = 0;
    length += strlen(args);
    char *command = calloc(MAX_FILE_PATH + strlen(PYTHON_EXE_PATH)+length +10,1);
    unsigned int offset = 0;

    memcpy(command + offset,EXEC_PY_PATH,strlen(EXEC_PY_PATH));
    offset += strlen(EXEC_PY_PATH);
    command[offset] = ' ';
    offset++;

    memcpy(command + offset,PYTHON_EXE_PATH,strlen(PYTHON_EXE_PATH));
    offset += strlen(PYTHON_EXE_PATH);
    command[offset] = ' ';
    offset++;

    memcpy(command + offset,script_path,strlen(script_path));
    offset += strlen(script_path);
    command[offset] = ' ';
    offset++;

    memcpy(command + offset,args,strlen(args));
    offset += strlen(PYTHON_EXE_PATH);
    command[offset-1] = ' ';
    printf("command = %s\n",command);
    system(command);
    free(command);
    printf("TCP: started python script\n");
}
