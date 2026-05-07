#ifndef LAB_6_DEQUE_H
#define LAB_6_DEQUE_H

#include "unionBank.h"

typedef struct {
    int MAX_CUSTOMERS;
    int front;
    int rear;
    uBankCustomer *customers;
} Deque;

void deque_menu();

#endif // LAB_6_DEQUE_H
