#ifndef SIMPROJECT_TCPHANDLER_H
#define SIMPROJECT_TCPHANDLER_H

#define DEFAULT_BUFLEN (1024)

typedef unsigned char thread_id;


int  GetClientPortFromThreadId(thread_id tid);

int  GetAckPortFromThreadId(thread_id tid);

int AddDataToSendQueue(thread_id tid, char *data);

thread_id StartTcp();

#endif //SIMPROJECT_TCPHANDLER_H
