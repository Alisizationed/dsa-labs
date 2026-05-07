#ifndef LAB_6_CIRCULAR_QUEUE_H
#define LAB_6_CIRCULAR_QUEUE_H

#include "unionBank.h"

typedef struct {
    int front;
    int rear;
    int MAX_CUSTOMERS;
    uBankCustomer *customers;
} CircularQueue;

void circular_queue_menu();

#endif // LAB_6_CIRCULAR_QUEUE_H
