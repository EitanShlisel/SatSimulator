#include <stdio.h>
#include "CommandDictionary.h"

#define MAX_PLOT_PARAM_LENGTH 30


char plot_params[][MAX_PLOT_PARAM_LENGTH]= {
        "sim_time",
        "eps_vbatt",
        "eps_total_output_current",
        "eps_charging_current",
        "eps_channel_volt",
        "eps_channel_current"
};
cmd_t command_dictionary[] = {
//        {.opcode = "plot_graph",.args =(cmd_param_t*){.};}
};
int GetCmdByName(cmd_t cmd, user_input_t *input){
    return 0;
}