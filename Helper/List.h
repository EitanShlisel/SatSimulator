#ifndef SIMPROJECT_LIST_H
#define SIMPROJECT_LIST_H

#include<stdbool.h>
#include <stdio.h>

typedef struct list_t{
    void *data;
    unsigned int data_length;
    struct list_t *next;
}list_t;

list_t* ListHelper_CreateList(void *data, unsigned int length);
list_t* ListHelper_Add(list_t *head,list_t *node);
unsigned int ListHelper_GetLength(list_t *head);
list_t* ListHelper_Pop(list_t **head);
list_t* ListHelper_GetNodeAtIndex(list_t *head,unsigned int index);
void ListHelper_DestroyNode(list_t *node);

void ListHelper_PrintList(list_t *head,FILE *fp);
void ListHelper_PrintNode(list_t *node,FILE *fp);
#endif //SIMPROJECT_LIST_H
