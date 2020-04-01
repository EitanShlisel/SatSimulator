#ifndef SIMPROJECT_TCPPLOTTER_H
#define SIMPROJECT_TCPPLOTTER_H
#include "TcpHandler.h"

typedef double(*data_function_t)(double);

typedef struct{
    double x;
    double y;
}data_point_t;

#define MAX_ATTR_LENGTH 20
typedef struct{
    unsigned int figure_id;
    unsigned int sub_figure_id;
    char title[MAX_ATTR_LENGTH];
    char x_label[MAX_ATTR_LENGTH];
    char y_label[MAX_ATTR_LENGTH];
    data_point_t *dataPoints;
    unsigned int num_of_data_points;
}figure_t;

int ApplyFunctionToRange(data_point_t *dataPoints,  unsigned int length, double start, double end, data_function_t func);

unsigned int EstimateXmlFileSize(unsigned int num_of_data_points);

unsigned int SendFigureToPlotter(thread_id tid, figure_t *fig);

#endif //SIMPROJECT_TCPPLOTTER_H
