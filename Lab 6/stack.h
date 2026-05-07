#ifndef LAB_6_STACK_H
#include "unionBank.h"

typedef struct {
    int MAX_CUSTOMERS;
    int top;
    uBankCustomer *customers;
} Stack;

void stack_menu();

#define LAB_6_STACK_H

#endif //LAB_6_STACK_H
