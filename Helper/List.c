#include "List.h"

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

list_t* ListHelper_CreateList(void *data, unsigned int length){
    list_t *ptr = (list_t*)malloc(sizeof(list_t));
    if(NULL == ptr){
        return NULL;
    }
    ptr->next = NULL;
    ptr->data = NULL;
    if((0 != length ) && (NULL != data)){
        ptr->data = malloc(length);
        memcpy(ptr->data,data,length);
        if(NULL == ptr->data){
            free(ptr);
            return NULL;
        }
    }
    ptr->data_length = length;
    return ptr;
}

list_t* ListHelper_Add(list_t *head,list_t *node) {
    if(NULL == head){
        if(NULL!= node) {
            mempcpy(head, node, sizeof(*head));
            return head;
        }
        return NULL;
    }
    list_t *ptr = head;
    while(NULL != ptr->next){
        ptr=ptr->next;
    }
    ptr->next = node;
    return head;
}

unsigned int ListHelper_GetLength(list_t *head){
    unsigned int length = 0;
    list_t *ptr = head;
    while(NULL != ptr){
        length++;
    }
    return length;
}

list_t* ListHelper_Pop(list_t **head){
    list_t *ptr = *head;
    *head = (**head).next;
    return ptr;
}

list_t* ListHelper_GetNodeAtIndex(list_t *head,unsigned int index){
    list_t *ptr = head;
    unsigned int i = 0;
    while(NULL != ptr){
        if(i == index){
            break;
        }
        ptr=ptr->next;
        i++;
    }
    return ptr;
}

void ListHelper_DestroyNode(list_t *node){
    if(NULL != node){
        if(NULL != node->data) {
            free(node->data);
        }
        free(node);
    }
}

void ListHelper_PrintNode(list_t *node,FILE *fp){
    FILE *pfile = fp;
    if(NULL == pfile){
        pfile = stdout;
    }
    fprintf(pfile,"{");
    fprintf(pfile,"%02X",((unsigned char*)(node->data))[0]);
    for (unsigned int i = 1; i < node->data_length; ++i) {
        fprintf(pfile,"-%02X",((unsigned char*)(node->data))[i]);
    }
    fprintf(pfile,"}\n");
}

void ListHelper_PrintList(list_t *head,FILE *fp){
    list_t *ptr = head;
    FILE *filep = fp;
    if(NULL == filep){
        filep = stdout;
    }
    fprintf(fp,"printing list data:\n");
    while(NULL != ptr){
        ListHelper_PrintNode(ptr,filep);
        ptr = ptr->next;
    }
}
