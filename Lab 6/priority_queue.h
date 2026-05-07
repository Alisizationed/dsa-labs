#ifndef LAB_6_PRIORITY_QUEUE_H
#define LAB_6_PRIORITY_QUEUE_H

#include "unionBank.h"

typedef struct {
    uBankCustomer customer;
    int priority;
} PriorityQueueElement;

typedef struct {
    PriorityQueueElement *elements;
    int size;
    int capacity;
} PriorityQueue;

void priority_queue_menu();

#endif
