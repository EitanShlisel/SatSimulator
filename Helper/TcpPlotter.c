#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "TcpPlotter.h"

typedef double(*data_function_t)(double);

typedef char* xml_file_t;

typedef struct{
    unsigned int num_of_packets_in_msg;
    xml_file_t xml_file;
}xml_msg_t;

#define NumOfPackets(num_of_bytes) (((num_of_bytes) / (DEFAULT_BUFLEN - sizeof(unsigned int)))+1)

int ApplyFunctionToRange(data_point_t *dataPoints,  unsigned int length, double start, double end, data_function_t func){
    if(NULL == dataPoints || NULL == func){
        return -1;
    }
    if(end < start){
        return -2;
    }
    double dx = (end-start)/(double)length;
    data_point_t point;
    for (unsigned int n = 0; n < length; ++n) {
        point.x = start + (float)n * dx;
        point.y = func(point.x);
        memcpy(&dataPoints[n],&point,sizeof(point));
    }
    return 0;
}

unsigned int EstimateXmlFileSize(unsigned int num_of_data_points){
    // 50 = max figure line
    // 10 = max id size
    // 9 =  </figure>
    // 8 =  # of " in figure element
    // 4 = # of " in point element
    // 8 = characters in <point/>
    // 20 = max num of digits per float (x2)
    // 6 , 7 = <data> + </data>
    // 100 = just because
    return (10 + 50) + sizeof(figure_t) + 8 +
           (2 * 20 + 8 + 4) * num_of_data_points
           +(6 + 7) + 100;
}

unsigned int FormatData(figure_t *fig, xml_msg_t *xml){
    if(NULL == fig){
        return -1;
    }
    unsigned int temp = EstimateXmlFileSize(fig->num_of_data_points);
    unsigned int num_of_packets = NumOfPackets(temp);
    xml->xml_file = calloc(num_of_packets * DEFAULT_BUFLEN, 1);
    unsigned int num_of_char_written = 0;
    num_of_char_written += sprintf(xml->xml_file + num_of_char_written, "<figure id = \"%u\" title = \"%s\" Xaxis = \"%s\" Yaxis = \"%s\">", fig->figure_id, fig->title, fig->x_label, fig->y_label);
    num_of_char_written += sprintf(xml->xml_file + num_of_char_written, "<data sub_id = \"%u\">",fig->sub_figure_id);
    for (unsigned int i = 0; i < fig->num_of_data_points; ++i) {
        num_of_char_written += sprintf(xml->xml_file + num_of_char_written, "<point X=\"%lf\" Y=\"%lf\"/>", fig->dataPoints[i].x, fig->dataPoints[i].y);
    }
    num_of_char_written += sprintf(xml->xml_file + num_of_char_written, "</data>");
    num_of_char_written += sprintf(xml->xml_file + num_of_char_written, "</figure>");
    xml->num_of_packets_in_msg = NumOfPackets(num_of_char_written);

    return num_of_char_written;
}

unsigned int SendFigureToPlotter(thread_id tid, figure_t *fig){
    unsigned int num_of_bytes;
    char buff[DEFAULT_BUFLEN] = {0};
    xml_msg_t msg;

    num_of_bytes = FormatData(fig, &msg);
    for (unsigned int j = 0; j < msg.num_of_packets_in_msg; ++j) {
        memcpy(buff, &msg.num_of_packets_in_msg, sizeof(msg.num_of_packets_in_msg));
        memcpy(buff + sizeof(msg.num_of_packets_in_msg),
               (msg.xml_file + j * (DEFAULT_BUFLEN - sizeof(msg.num_of_packets_in_msg))),
               (DEFAULT_BUFLEN - sizeof(msg.num_of_packets_in_msg)));

        AddDataToSendQueue(tid, buff);
    }
    free(msg.xml_file);
    return num_of_bytes;
}

