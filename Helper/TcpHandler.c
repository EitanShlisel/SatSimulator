

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "TcpHandler.h"




#define MAX_NUM_OF_TCP_THREADS 255
thread_id thread_ids[MAX_NUM_OF_TCP_THREADS] = {0};
pthread_t thread_handle[MAX_NUM_OF_TCP_THREADS];
sem_t sem_data_in_buffer[MAX_NUM_OF_TCP_THREADS];
sem_t sem_done_sending[MAX_NUM_OF_TCP_THREADS];
unsigned char *thread_buffers[MAX_NUM_OF_TCP_THREADS];

int  GetClientPortFromThreadId(thread_id tid){
    return (40000 + 2 * (tid));
}
int  GetAckPortFromThreadId(thread_id tid){
    return (40000 + 2 * (tid) + 1);
}
int GetNextAvailableThreadId(thread_id *id){
    if(NULL == id){
        return -1;
    }
    for (unsigned int i = 0; i < MAX_NUM_OF_TCP_THREADS; ++i) {
        if(0 == thread_ids[i]){
            *id = i;
            return 0;
        }
    }
    return -2;
}

int InitSocket(SOCKET *sock,char *port){
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;

    int iResult = 0;
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return iResult;
    }

    memset(&hints,0,sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return iResult;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        iResult = WSAGetLastError();
        printf("socket failed with error: %ld\n", iResult);
        freeaddrinfo(result);
        WSACleanup();
        return iResult;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        iResult = WSAGetLastError();
        printf("bind failed with error: %d\n", iResult);
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return iResult;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        iResult = WSAGetLastError();
        printf("listen failed with error: %d\n", iResult);
        closesocket(ListenSocket);
        WSACleanup();
        return iResult;
    }

    // Accept a client socket
    *sock = accept(ListenSocket, NULL, NULL);
    if (*sock == INVALID_SOCKET) {
        iResult = WSAGetLastError();
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return iResult;
    }
    closesocket(ListenSocket);
}

int AddDataToSendQueue(thread_id tid, char *data){
    if(NULL == data){
        return -1;
    }
    memcpy(thread_buffers[tid], data, DEFAULT_BUFLEN);
    sem_post(&sem_data_in_buffer[tid]);
    sem_wait(&sem_done_sending[tid]);
}

void* tcpThread(void *param){
    thread_id tid;
    memcpy(&tid,param,sizeof(tid));
    free(param);

    int err = 0;
    int iResult = 0;

    char client_port[6] = {0};
    char ack_port[6] = {0};
    itoa(GetClientPortFromThreadId(tid), client_port, 10);
    itoa(GetAckPortFromThreadId(tid),ack_port,10);
    printf("tid = %d;\tClient port = %s\n",tid,client_port);
    printf("tid = %d;\tack port = %s\n",tid,ack_port);

    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET AckSocket = INVALID_SOCKET;
    unsigned char data_buffer[DEFAULT_BUFLEN] = {0};
    thread_buffers[tid] = data_buffer;
    unsigned char ack_buffer[10] = {0};

    err = InitSocket(&ClientSocket, client_port);
    if(0 != err){
        thread_ids[tid] = 0;
        pthread_exit(&err);
        return NULL;
    }
    err = InitSocket(&AckSocket,ack_port);
    if(0 != err){
        thread_ids[tid] = 0;
        pthread_exit(&err);
        return NULL;
    }
    printf("TCP: initialised sockets\n");
    unsigned int counter = 0;
    do {
        sem_wait(&sem_data_in_buffer[tid]);
        printf("TCP: got data to send\n");
        iResult = send( ClientSocket, data_buffer, DEFAULT_BUFLEN, 0 );
        sem_post(&sem_done_sending[tid]);
        if(iResult < 0){
            goto CLEANUP;
        }
        printf("TCP: sent %d bytes\n",iResult);
        if(iResult != DEFAULT_BUFLEN){
            counter++;
            if(counter > 10){
                goto CLEANUP;
            }
            usleep(10);
            continue;
        }else{
            counter = 0;
        }
        iResult = recv( AckSocket, ack_buffer, 10, 0 );
        printf("TCP: got ack\n");
    } while (1);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        goto CLEANUP;
    }
    goto CLEANUP;

    CLEANUP:
    iResult = WSAGetLastError();
    closesocket(ClientSocket);
    closesocket(AckSocket);
    WSACleanup();
    thread_ids[tid] = 0;
    pthread_exit(&iResult);
    return NULL;
}

thread_id StartTcp(){
    thread_id *tid = malloc(sizeof(thread_id));
    unsigned short port;
    int err = GetNextAvailableThreadId(tid);
    if(0 != err){
        return MAX_NUM_OF_TCP_THREADS;
    }
    thread_ids[*tid] = 0xFF; // id is taken
    err = sem_init(&sem_data_in_buffer[*tid],0,0);
    if(0 != err){
        thread_ids[*tid] = 0;
        return MAX_NUM_OF_TCP_THREADS;
    }
    err = sem_init(&sem_done_sending[*tid],0,0);
    if(0 != err){
        thread_ids[*tid] = 0;
        return MAX_NUM_OF_TCP_THREADS;
    }
    err = pthread_create(&thread_handle[*tid],NULL, tcpThread, tid);
    if(0 != err){
        thread_ids[*tid] = 0;
        return MAX_NUM_OF_TCP_THREADS;
    }

    //TODO: start python script
    return *tid;
}