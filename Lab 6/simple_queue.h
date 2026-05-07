#ifndef LAB_6_SIMPLE_QUEUE_H
#include "unionBank.h"

typedef struct SimpleQueue {
    int MAX_CUSTOMERS;
    int rear;
    int front;
    uBankCustomer *customers;
} SimpleQueue;

void simple_queue_menu();

#define LAB_6_SIMPLE_QUEUE_H

#endif //LAB_6_SIMPLE_QUEUE_H
