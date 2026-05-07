#include <stdio.h>
#include <stdlib.h>

#include "circular_queue.h"

CircularQueue *circular_queue_create(int capacity) {
    CircularQueue *q = malloc(sizeof(CircularQueue));
    q->customers = malloc(capacity * sizeof(uBankCustomer));
    q->MAX_CUSTOMERS = capacity;
    q->front = -1;
    q->rear = -1;
    return q;
}

void circular_queue_destroy(CircularQueue *q) {
    free(q->customers);
    free(q);
}

// Check if circular queue is empty
_Bool circular_queue_is_empty(CircularQueue *q) {
    return q->front == -1;
}

// Check if circular queue is full
_Bool circular_queue_is_full(CircularQueue *q) {
    return (q->rear + 1) % q->MAX_CUSTOMERS == q->front;
}

// Get current size of circular queue
int circular_queue_size(CircularQueue *q) {

    if (circular_queue_is_empty(q))
        return 0;

    if (q->rear >= q->front)
        return q->rear - q->front + 1;

    return q->MAX_CUSTOMERS - q->front + q->rear + 1;
}

void circular_queue_enqueue(CircularQueue *q, uBankCustomer customer) {
    if (circular_queue_is_full(q)) {
        printf("Queue is full\n");
        return;
    }
    if (q->front == -1) q->front = 0; // First element

    q->rear = (q->rear + 1) % q->MAX_CUSTOMERS;
    q->customers[q->rear] = customer;
}

uBankCustomer circular_queue_dequeue(CircularQueue *q) {
    if (circular_queue_is_empty(q)) {
        printf("Queue is empty\n");
        uBankCustomer temp = {0};
        return temp;
    }

    uBankCustomer removed = q->customers[q->front];

    if (q->front == q->rear) {
        q->front = q->rear = -1; // Reset queue
    } else {
        q->front = (q->front + 1) % q->MAX_CUSTOMERS;
    }

    return removed;
}

uBankCustomer circular_queue_peek(CircularQueue *q) {
    if (circular_queue_is_empty(q)) {
        printf("Queue is empty\n");
        uBankCustomer temp = {0};
        return temp;
    }
    return q->customers[q->front];
}

uBankCustomer *circular_queue_find_by_no(CircularQueue *q, int account_no) {
    int size = circular_queue_size(q);
    for (int i = 0; i < size; i++) {
        int j = (q->front + i) % q->MAX_CUSTOMERS;
        if ((int)q->customers[j].account_no == account_no)
            return &q->customers[j];
    }
    return NULL;
}

uBankCustomer *circular_queue_find_by_position(CircularQueue *q, int pos) {
    if (pos < 0 || pos >= circular_queue_size(q)) {
        printf("Position %d out of range\n", pos);
        return NULL;
    }
    int j = (q->front + pos) % q->MAX_CUSTOMERS;
    return &q->customers[j];
}

void circular_queue_print(CircularQueue *q, FILE *file) {
    if (circular_queue_is_empty(q)) {
        fprintf(file, "Circular queue is empty\n");
        return;
    }
    int size = circular_queue_size(q);
    for (int i = 0; i < size; i++) {
        int idx = (q->front + i) % q->MAX_CUSTOMERS;
        u_display_bank_customer(&q->customers[idx], file);
    }
}

void circular_queue_save_to_file(CircularQueue *q, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "wb" : "w");
    if (!f) { printf("Cannot open: %s\n", filename); return; }
    int size = circular_queue_size(q);
    if (binary) {
        fwrite(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            int idx = (q->front + i) % q->MAX_CUSTOMERS;
            fwrite(&q->customers[idx], sizeof(uBankCustomer), 1, f);
        }
    } else {
        fprintf(f, "%d\n", size);
        for (int i = 0; i < size; i++) {
            int idx = (q->front + i) % q->MAX_CUSTOMERS;
            u_display_bank_customer(&q->customers[idx], f);
        }
    }
    fclose(f);
}

void circular_queue_load_from_file(CircularQueue *q, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "rb" : "r");
    if (!f) { printf("Cannot open: %s\n", filename); return; }
    int size;
    if (binary) {
        fread(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            fread(&c, sizeof(uBankCustomer), 1, f);
            circular_queue_enqueue(q, c);
        }
    } else {
        fscanf(f, "%d", &size);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            u_read_customer(&c, f);
            circular_queue_enqueue(q, c);
        }
    }
    fclose(f);
}

void circular_queue_menu() {
    int cap; printf("Circular queue capacity: "); scanf("%d", &cap);
    CircularQueue *q = circular_queue_create(cap);
    int choice;

    do {
        printf("\nCircular queue menu:\n");
        printf("1 - Enqueue\n2 - Dequeue\n3 - Peek\n4 - Display\n");
        printf("5 - Find by no\n6 - Find by position\n7 - Save\n8 - Load\n0 - Exit\n");
        printf("Choice: "); scanf("%d", &choice);
        switch (choice) {
            case 1: {
                    uBankCustomer c;
                    u_read_customer(&c, stdin);
                    circular_queue_enqueue(q, c);
                }
                break;
            case 2: {
                    uBankCustomer c = circular_queue_dequeue(q);
                    if (c.account_no)
                        u_display_bank_customer(&c, stdout);
                }
                break;
            case 3: {
                    uBankCustomer c = circular_queue_peek(q);
                    if (c.account_no)
                        u_display_bank_customer(&c, stdout);
                }
                break;
            case 4:
                circular_queue_print(q, stdout);
                break;
            case 5: {
                    int no;
                    printf("Account No: ");
                    scanf("%d", &no);
                    uBankCustomer *c = circular_queue_find_by_no(q, no);
                    if (c)
                        u_display_bank_customer(c, stdout);
                    else
                        printf("Not found.\n");
                }
                break;
            case 6: {
                    int p;
                    printf("Position: ");
                    scanf("%d", &p);
                    uBankCustomer *c = circular_queue_find_by_position(q, p);
                    if (c)
                        u_display_bank_customer(c, stdout);
                }
                break;
            case 7: {
                    char fn[256];
                    int m;
                    printf("File: ");
                    scanf("%255s", fn);
                    printf("Mode (0=text,1=bin): ");
                    scanf("%d", &m);
                    circular_queue_save_to_file(q, fn, m);
                }
                break;
            case 8: {
                    char fn[256];
                    int m; printf("File: ");
                    scanf("%255s", fn);
                    printf("Mode (0=text,1=bin): ");
                    scanf("%d", &m);
                    circular_queue_load_from_file(q, fn, m);
                }
                break;
            case 0: break;
            default: printf("Unknown.\n");
        }
    } while (choice != 0);
    circular_queue_destroy(q);
}