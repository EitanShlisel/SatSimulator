
#include "GenericHelpFunctions.h"

int getLine(FILE *fp, char *line){
    if(NULL == fp){
        return -1;
    }
    if(NULL == line){
        return -2;
    }
    unsigned int i = 0;
    while('\n' != (line[i] = (char)getc(fp))){
        i++;
    }
    line[i] = '\0';
    return 0;
}