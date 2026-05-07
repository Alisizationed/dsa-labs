#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>

// Create queue
PriorityQueue *priority_queue_create(int initial_capacity) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    if (pq == NULL) {
        return NULL;
    }
    pq->pcustomers = malloc(initial_capacity * sizeof(PriorityQueueElement));
    if (pq->pcustomers == NULL) {
        free(pq);
        return NULL;
    }
    pq->size = 0;
    pq->capacity = initial_capacity;
    return pq;
}

// Destroy queue
void priority_queue_destroy(PriorityQueue *pq) {
    if (pq == NULL) {
        return;
    }
    free(pq->pcustomers);
    free(pq);
}

// Check if empty
_Bool priority_queue_is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

// Get size
int priority_queue_size(PriorityQueue *pq) {
    return pq->size;
}

// Enqueue by priority
void priority_queue_enqueue(PriorityQueue *pq, uBankCustomer customer, int priority) {
    // Resize if full
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        PriorityQueueElement *temp = realloc(pq->pcustomers, pq->capacity * sizeof(PriorityQueueElement));
        if (temp == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        pq->pcustomers = temp;
    }
    int i = pq->size - 1;
    // Shift lower-priority elements right
    while (i >= 0 && pq->pcustomers[i].priority > priority) {
        pq->pcustomers[i + 1] = pq->pcustomers[i];
        i--;
    }
    pq->pcustomers[i + 1].customer = customer;
    pq->pcustomers[i + 1].priority = priority;
    pq->size++;
}

// Dequeue highest-priority element
uBankCustomer priority_queue_dequeue(PriorityQueue *pq) {
    if (priority_queue_is_empty(pq)) {
        printf("Priority queue is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    uBankCustomer customer = pq->pcustomers[0].customer;
    // Shift left
    for (int i = 1; i < pq->size; i++) {
        pq->pcustomers[i - 1] = pq->pcustomers[i];
    }
    pq->size--;
    return customer;
}

// Peek highest-priority element
uBankCustomer priority_queue_peek(PriorityQueue *pq) {
    if (priority_queue_is_empty(pq)) {
        printf("Priority queue is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return pq->pcustomers[0].customer;
}

// Find by account number
PriorityQueueElement *priority_queue_find_by_no(PriorityQueue *pq, int account_no) {
    for (int i = 0; i < pq->size; i++) {
        if ((int) pq->pcustomers[i].customer.account_no == account_no) {
            return &pq->pcustomers[i];
        }
    }
    return NULL;
}

// Find by position
PriorityQueueElement *priority_queue_find_by_position(PriorityQueue *pq, int pos) {
    if (pos < 0 || pos >= pq->size) {
        printf("Position %d out of range\n", pos);
        return NULL;
    }
    return &pq->pcustomers[pos];
}

// Print queue
void priority_queue_print(PriorityQueue *pq, FILE *file) {
    if (priority_queue_is_empty(pq)) {
        fprintf(file, "Priority queue is empty\n");
        return;
    }
    for (int i = 0; i < pq->size; i++) {
        u_display_bank_customer(&pq->pcustomers[i].customer, file);
    }
    fprintf(file, "\n");
}

// Save queue to file
void priority_queue_save_to_file(PriorityQueue *pq, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "wb" : "w");
    if (!f) {
        printf("Cannot open: %s\n", filename);
        return;
    }
    if (binary) {
        fwrite(&pq->size, sizeof(int), 1, f);
        fwrite(pq->pcustomers, sizeof(PriorityQueueElement), pq->size, f);
    } else {
        fprintf(f, "%d\n", pq->size);
        for (int i = 0; i < pq->size; i++) {
            fprintf(f, "%d\n", pq->pcustomers[i].priority);
            u_display_bank_customer(&pq->pcustomers[i].customer, f);
        }
    }
    fclose(f);
}

// Load queue from file
void priority_queue_load_from_file(PriorityQueue *pq, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "rb" : "r");

    if (!f) {
        printf("Cannot open: %s\n", filename);
        return;
    }
    int size;
    if (binary) {
        fread(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            PriorityQueueElement element;
            fread(&element, sizeof(PriorityQueueElement), 1, f);
            priority_queue_enqueue(pq, element.customer, element.priority);
        }
    } else {
        fscanf(f, "%d", &size);
        for (int i = 0; i < size; i++) {
            int priority;
            uBankCustomer customer;
            fscanf(f, "%d", &priority);
            u_read_customer(&customer, f);
            priority_queue_enqueue(pq, customer, priority);
        }
    }
    fclose(f);
}

// Interactive menu
void priority_queue_menu() {
    PriorityQueue *pq = priority_queue_create(10);
    if (pq == NULL) {
        printf("Queue creation failed\n");
        return;
    }
    int choice;
    do {
        printf("\nPriority queue menu:\n");
        printf("1 - Enqueue\n2 - Dequeue\n3 - Peek\n");
        printf("4 - Display\n5 - Find by account no\n6 - Find by position\n");
        printf("7 - Save\n8 - Load\n0 - Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                uBankCustomer customer;
                int priority;
                u_read_customer(&customer,stdin);
                printf("Priority (1 = highest): ");
                scanf("%d", &priority);
                priority_queue_enqueue(pq, customer, priority);
                break;
            }
            case 2: {
                uBankCustomer customer = priority_queue_dequeue(pq);
                if (customer.account_no) {
                    u_display_bank_customer(&customer,stdout);
                }
                break;
            }
            case 3: {
                uBankCustomer customer = priority_queue_peek(pq);
                if (customer.account_no) {
                    u_display_bank_customer(&customer,stdout);
                }
                break;
            }
            case 4:
                priority_queue_print(pq,stdout);
                break;
            case 5: {
                int no;
                printf("Account No: ");
                scanf("%d", &no);
                PriorityQueueElement *e = priority_queue_find_by_no(pq, no);
                if (e) {
                    u_display_bank_customer(&e->customer,stdout);
                } else {
                    printf("Not found\n");
                }
                break;
            }
            case 6: {
                int pos;
                printf("Position: ");
                scanf("%d", &pos);
                PriorityQueueElement *e = priority_queue_find_by_position(pq, pos);
                if (e) {
                    u_display_bank_customer(&e->customer,stdout);
                }
                break;
            }
            case 7: {
                char filename[256];
                int mode;
                printf("Filename: ");
                scanf("%255s", filename);
                printf("Mode (0=text, 1=binary): ");
                scanf("%d", &mode);
                priority_queue_save_to_file(pq, filename, mode);
                break;
            }
            case 8: {
                char filename[256];
                int mode;
                printf("Filename: ");
                scanf("%255s", filename);
                printf("Mode (0=text, 1=binary): ");
                scanf("%d", &mode);
                priority_queue_load_from_file(pq, filename, mode);
                break;
            }
            case 0:
                break;
            default:
                printf("Unknown command\n");
        }
    } while (choice != 0);
    priority_queue_destroy(pq);
}
