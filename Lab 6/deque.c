#include <stdio.h>
#include "deque.h"

#include <stdlib.h>

Deque *deque_create(int initial_capacity) {
    Deque *d = malloc(sizeof(Deque));
    d->customers = malloc(initial_capacity * sizeof(uBankCustomer));
    d->MAX_CUSTOMERS = initial_capacity;
    d->front = -1;
    d->rear = -1;
    return d;
}

void deque_destroy(Deque *d) {
    free(d->customers);
    free(d);
}

// Function to check if the deque is empty
_Bool deque_is_empty(Deque *d) {
    return (d->front == -1);
}

// Function to find the deque size
int deque_size(Deque *d) {
    if (deque_is_empty(d)) return 0;
    return (d->rear - d->front + d->MAX_CUSTOMERS) % d->MAX_CUSTOMERS + 1;
}

_Bool deque_is_full(Deque *d) {
    return deque_size(d) == d->MAX_CUSTOMERS;
}

// Function to insert an element at the front of the deque
void deque_push_front(Deque *d, uBankCustomer customer) {
    if (deque_is_full(d)) {
        int old_size = d->MAX_CUSTOMERS;
        uBankCustomer *temp = malloc(old_size * 2 * sizeof(uBankCustomer));
        if (temp == NULL) return;
        for (int i = 0; i < old_size; i++)
            temp[i] = d->customers[(d->front + i) % old_size];
        free(d->customers);
        d->customers = temp;
        d->front = 0;
        d->rear = old_size - 1;
        d->MAX_CUSTOMERS = old_size * 2;
    } else if (deque_is_empty(d)) {
        d->front = 0;
        d->rear = 0;
    } else {
        d->front = (d->front - 1 + d->MAX_CUSTOMERS) % d->MAX_CUSTOMERS;
    }
    d->customers[d->front] = customer;
}

// Function to insert an element at the rear of the deque
void deque_push_back(Deque *d, uBankCustomer customer) {
    if (deque_is_full(d)) {
        int old_size = d->MAX_CUSTOMERS;
        uBankCustomer *temp = malloc(old_size * 2 * sizeof(uBankCustomer));
        if (temp == NULL) return;
        for (int i = 0; i < old_size; i++)
            temp[i] = d->customers[(d->front + i) % old_size];
        free(d->customers);
        d->customers = temp;
        d->front = 0;
        d->rear = old_size - 1;
        d->MAX_CUSTOMERS = old_size * 2;
    }
    if (deque_is_empty(d)) {
        d->front = 0;
        d->rear = 0;
    } else d->rear = (d->rear + 1) % d->MAX_CUSTOMERS;
    d->customers[d->rear] = customer;
}

// Function to delete an element from the front of the deque
uBankCustomer deque_pop_front(Deque *d) {
    if (deque_is_empty(d)) {
        printf("Deque is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    uBankCustomer removed = d->customers[d->front];
    if (d->front == d->rear) {
        d->front = -1;
        d->rear = -1;
    } else {
        d->front = (d->front + 1) % d->MAX_CUSTOMERS;
    }
    return removed;
}

// Function to delete an element from the rear of the deque
uBankCustomer deque_pop_back(Deque *d) {
    if (deque_is_empty(d)) {
        printf("Deque is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    uBankCustomer removed = d->customers[d->rear];
    if (d->front == d->rear) {
        d->front = -1;
        d->rear = -1;
    } else {
        d->rear = (d->rear - 1 + d->MAX_CUSTOMERS) % d->MAX_CUSTOMERS;
    }
    return removed;
}

uBankCustomer deque_peek_front(Deque *d) {
    if (deque_is_empty(d)) {
        printf("Deque is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return d->customers[d->front];
}

uBankCustomer deque_peek_back(Deque *d) {
    if (deque_is_empty(d)) {
        printf("Deque is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return d->customers[d->rear];
}

uBankCustomer *deque_find_by_no(Deque *d, int account_no) {
    if (deque_is_empty(d)) return NULL;
    int j = d->front;
    for (int i = 0; i < deque_size(d); i++) {
        if ((int) d->customers[j].account_no == account_no)
            return &d->customers[j];
        j = (j + 1) % d->MAX_CUSTOMERS;
    }
    return NULL;
}

uBankCustomer *deque_find_by_position(Deque *d, int pos) {
    if (pos < 0 || pos >= deque_size(d)) {
        printf("Position %d out of range\n", pos);
        return NULL;
    }
    int j = (d->front + pos) % d->MAX_CUSTOMERS;
    return &d->customers[j];
}

void deque_print(Deque *d, FILE *file) {
    if (deque_is_empty(d)) {
        fprintf(file, "Deque is empty\n");
        return;
    }
    int j = d->front, size = deque_size(d);
    for (int i = 0; i < size; i++) {
        fprintf(file, "[%d] ", i);
        u_display_bank_customer(&d->customers[j], file);
        j = (j + 1) % d->MAX_CUSTOMERS;
    }
    fprintf(file, "\n");
}

void deque_save_to_file(Deque *d, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "wb" : "w");
    if (!f) {
        printf("Cannot open: %s\n", filename);
        return;
    }
    int size = deque_size(d), j = d->front;
    if (binary) {
        fwrite(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            fwrite(&d->customers[j], sizeof(uBankCustomer), 1, f);
            j = (j + 1) % d->MAX_CUSTOMERS;
        }
    } else {
        fprintf(f, "%d\n", size);
        for (int i = 0; i < size; i++) {
            u_display_bank_customer(&d->customers[j], f);
            j = (j + 1) % d->MAX_CUSTOMERS;
        }
    }
    fclose(f);
}

void deque_load_from_file(Deque *d, const char *filename, _Bool binary) {
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
            deque_push_back(d, c);
        }
    } else {
        fscanf(f, "%d", &size);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            u_read_customer(&c, f);
            deque_push_back(d, c);
        }
    }
    fclose(f);
}

void deque_menu() {
    Deque *d = deque_create(2);
    int choice;

    do {
        printf("\nDeque menu:\n");
        printf("1 - Push front\n2 - Push back\n3 - Pop front\n");
        printf("4 - Pop back\n5 - Peek front / rear\n6 - Display all\n");
        printf("7 - Find by account no\n8 - Find by position\n9 - Save to file\n");
        printf("10- Load from file\n0 - Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            case 2: {
                uBankCustomer c;
                u_read_customer(&c, stdin);
                if (choice == 1)
                    deque_push_front(d, c);
                else
                    deque_push_back(d, c);
            }
            break;
            case 3: {
                uBankCustomer c = deque_pop_front(d);
                if (c.account_no)
                    u_display_bank_customer(&c, stdout);
            }
            break;
            case 4: {
                uBankCustomer c = deque_pop_back(d);
                if (c.account_no) u_display_bank_customer(&c, stdout);
                break;
            }
            case 5: {
                printf("Front: ");
                uBankCustomer f = deque_peek_front(d);
                if (f.account_no) u_display_bank_customer(&f, stdout);
                printf("Rear:  ");
                uBankCustomer r = deque_peek_back(d);
                if (r.account_no) u_display_bank_customer(&r, stdout);
                break;
            }
            case 6: deque_print(d, stdout);
                break;
            case 7: {
                int no;
                printf("Account No: ");
                scanf("%d", &no);
                uBankCustomer *c = deque_find_by_no(d, no);
                if (c) u_display_bank_customer(c, stdout);
                else printf("Not found.\n");
                break;
            }
            case 8: {
                int pos;
                printf("Position: ");
                scanf("%d", &pos);
                uBankCustomer *c = deque_find_by_position(d, pos);
                if (c)
                    u_display_bank_customer(c, stdout);
            }
            break;
            case 9: {
                char f[256];
                int m;
                printf("Filename: ");
                scanf("%255s", f);
                printf("Mode(0=text,1=bin): ");
                scanf("%d", &m);
                deque_save_to_file(d, f, m);
            }
            break;
            case 10: {
                char f[256];
                int m;
                printf("Filename: ");
                scanf("%255s", f);
                printf("Mode(0=text,1=bin): ");
                scanf("%d", &m);
                deque_load_from_file(d, f, m);
            }
            break;
            case 0: break;
            default: printf("Unknown command.\n");
        }
    } while (choice != 0);

    deque_destroy(d);
}
