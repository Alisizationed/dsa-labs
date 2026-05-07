#include <stdio.h>
#include <stdlib.h>

#include "simple_queue.h"

// Create queue
SimpleQueue *simple_queue_create(int initial_capacity) {
    SimpleQueue *q = malloc(sizeof(SimpleQueue));
    if (q == NULL) {
        return NULL;
    }
    q->customers = malloc(initial_capacity * sizeof(uBankCustomer));
    if (q->customers == NULL) {
        free(q);
        return NULL;
    }
    q->MAX_CUSTOMERS = initial_capacity;
    q->front = -1;
    q->rear = -1;
    return q;
}

// Destroy queue
void simple_queue_destroy(SimpleQueue *q) {
    if (q == NULL) {
        return;
    }
    free(q->customers);
    free(q);
}

// Check if queue is empty
_Bool simple_queue_is_empty(SimpleQueue *q) {
    return q->front == q->rear;
}

// Check if queue is full
_Bool simple_queue_is_full(SimpleQueue *q) {
    return q->rear == q->MAX_CUSTOMERS - 1;
}

// Get queue size
int simple_queue_size(SimpleQueue *q) {
    return q->rear - q->front;
}

// Add customer to queue
void simple_queue_enqueue(SimpleQueue *q, uBankCustomer customer) {
    if (simple_queue_is_full(q)) {
        int size = simple_queue_size(q);
        // Shift elements left if there is unused space
        if (q->front >= 0) {
            for (int i = 0; i < size; i++) {
                q->customers[i] = q->customers[q->front + 1 + i];
            }
            q->front = -1;
            q->rear = size - 1;
        } else {
            // Resize queue
            q->MAX_CUSTOMERS *= 2;
            uBankCustomer *temp = realloc(
                q->customers,
                q->MAX_CUSTOMERS * sizeof(uBankCustomer)
            );
            if (temp == NULL) {
                printf("Memory reallocation failed\n");
                return;
            }
            q->customers = temp;
        }
    }
    q->customers[++q->rear] = customer;
}

// Remove customer from queue
uBankCustomer simple_queue_dequeue(SimpleQueue *q) {
    if (simple_queue_is_empty(q)) {
        printf("Queue is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    uBankCustomer customer = q->customers[q->front + 1];
    q->front++;
    // Reset queue if empty after dequeue
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    return customer;
}

// Get front customer
uBankCustomer simple_queue_peek(SimpleQueue *q) {
    if (simple_queue_is_empty(q)) {
        printf("Queue is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return q->customers[q->front + 1];
}

// Find customer by account number
uBankCustomer *simple_queue_find_by_no(SimpleQueue *q, int account_no) {
    // for (int i = q->front + 1; i <= q->rear; i++) {
    //     if ((int) q->customers[i].account_no == account_no) {
    //         return &q->customers[i];
    //     }
    // }
    return u_find_customer_by_no(q->customers + q->front + 1, simple_queue_size(q), account_no);
}

// Find customer by position
uBankCustomer *simple_queue_find_by_position(SimpleQueue *q, int pos) {
    int j = q->front + 1 + pos;
    if (pos < 0 || j > q->rear) {
        printf("Position %d out of range\n", pos);
        return NULL;
    }
    return &q->customers[j];
}

// Print queue
void simple_queue_print(SimpleQueue *q, FILE *file) {
    if (simple_queue_is_empty(q)) {
        fprintf(file, "Queue is empty\n");
        return;
    }
    for (int i = q->front + 1; i <= q->rear; i++) {
        u_display_bank_customer(&q->customers[i], file);
    }
    fprintf(file, "\n");
}

// Save queue to file
void simple_queue_save_to_file(SimpleQueue *q, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "wb" : "w");
    if (!f) {
        printf("Cannot open: %s\n", filename);
        return;
    }
    int size = simple_queue_size(q);
    if (binary) {
        fwrite(&size, sizeof(int), 1, f);
        fwrite(q->customers + q->front + 1,sizeof(uBankCustomer),size,f);
    } else {
        fprintf(f, "%d\n", size);
        for (int i = q->front + 1; i <= q->rear; i++) {
            u_display_bank_customer(&q->customers[i], f);
        }
    }
    fclose(f);
}

// Load queue from file
void simple_queue_load_from_file(SimpleQueue *q, const char *filename,  _Bool binary) {
    FILE *f = fopen(filename, binary ? "rb" : "r");
    if (!f) {
        printf("Cannot open: %s\n", filename);
        return;
    }
    int size;
    if (binary) {
        fread(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            fread(&c, sizeof(uBankCustomer), 1, f);
            simple_queue_enqueue(q, c);
        }
    } else {
        fscanf(f, "%d", &size);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            u_read_customer(&c, f);
            simple_queue_enqueue(q, c);
        }
    }
    fclose(f);
}

// Queue menu
void simple_queue_menu() {
    SimpleQueue *q = simple_queue_create(10);
    if (q == NULL) {
        printf("Queue creation failed\n");
        return;
    }
    int choice;
    do {
        printf("\nSimple queue menu:\n");
        printf("1 - Enqueue customer\n2 - Dequeue customer\n3 - Peek front\n");
        printf("4 - Display all\n5 - Find by account no\n6 - Find by position\n");
        printf("7 - Save to file\n8 - Load from file\n0 - Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                uBankCustomer c;
                u_read_customer(&c, stdin);
                simple_queue_enqueue(q, c);
                printf("Enqueued.\n");
                break;
            }
            case 2: {
                uBankCustomer c = simple_queue_dequeue(q);
                if (c.account_no) {
                    printf("Dequeued:\n");
                    u_display_bank_customer(&c, stdout);
                }
                break;
            }
            case 3: {
                uBankCustomer c = simple_queue_peek(q);
                if (c.account_no) {
                    u_display_bank_customer(&c, stdout);
                }
                break;
            }
            case 4:
                simple_queue_print(q, stdout);
                break;
            case 5: {
                int no;
                printf("Account No: ");
                scanf("%d", &no);
                uBankCustomer *c = simple_queue_find_by_no(q, no);
                if (c) {
                    u_display_bank_customer(c, stdout);
                } else {
                    printf("Not found.\n");
                }
                break;
            }
            case 6: {
                int pos;
                printf("Position (0 = front): ");
                scanf("%d", &pos);
                uBankCustomer *c = simple_queue_find_by_position(q, pos);
                if (c) {
                    u_display_bank_customer(c, stdout);
                }
                break;
            }
            case 7: {
                char fname[256];
                int mode;
                printf("Filename: ");
                scanf("%255s", fname);
                printf("Mode (0 = text, 1 = binary): ");
                scanf("%d", &mode);
                simple_queue_save_to_file(q, fname, mode);
                break;
            }
            case 8: {
                char fname[256];
                int mode;
                printf("Filename: ");
                scanf("%255s", fname);
                printf("Mode (0 = text, 1 = binary): ");
                scanf("%d", &mode);
                simple_queue_load_from_file(q, fname, mode);
                break;
            }
            case 0:
                break;
            default:
                printf("Unknown command.\n");
        }
    } while (choice != 0);
    simple_queue_destroy(q);
}
