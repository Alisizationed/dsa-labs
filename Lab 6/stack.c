#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack *stack_create(int initial_capacity) {
    Stack *s = malloc(sizeof(Stack));
    s->customers = malloc(initial_capacity * sizeof(uBankCustomer));
    s->MAX_CUSTOMERS = initial_capacity;
    s->top = -1;
    return s;
}

void stack_destroy(Stack *stack) {
    free(stack->customers);
    free(stack);
}

_Bool stack_is_empty(Stack *stack) {
    return stack->top == -1;
}

_Bool stack_is_full(Stack *stack) {
    return stack->top >= stack->MAX_CUSTOMERS - 1;
}

int stack_size(Stack *stack) {
    return stack->top + 1;
}

// Push: add element on top. Doubles capacity if full (dynamic growth).
void stack_push(Stack *stack, uBankCustomer value) {
    if (stack_is_full(stack)) {
        stack->MAX_CUSTOMERS *= 2;
        stack->customers = realloc(stack->customers,stack->MAX_CUSTOMERS * sizeof(uBankCustomer));
    }
    stack->customers[++stack->top] = value;
}

// Pop: remove and return the top element.
uBankCustomer stack_pop(Stack *stack) {
    if (stack_is_empty(stack)) {
        printf("Stack is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return stack->customers[stack->top--];
}

// Peek: look at top without removing.
uBankCustomer stack_peek(Stack *stack) {
    if (stack_is_empty(stack)) {
        printf("Stack is empty\n");
        uBankCustomer empty = {0};
        return empty;
    }
    return stack->customers[stack->top];
}

// Search by account number (scans from top downward)
uBankCustomer *stack_find_by_no(Stack *stack, int account_no) {
    for (int i = stack->top; i >= 0; i--) {
        if ((int)stack->customers[i].account_no == account_no)
            return &stack->customers[i];
    }
    return NULL;
}

// Search by position: pos=0 is top, pos=1 is one below top, etc.
uBankCustomer *stack_find_by_position(Stack *stack, int pos) {
    int j = stack->top - pos;
    if (j < 0 || j > stack->top) {
        printf("Position %d out of range\n", pos);
        return NULL;
    }
    return &stack->customers[j];
}

void stack_print(Stack *stack, FILE *file) {
    if (stack_is_empty(stack)) {
        fprintf(file, "Stack is empty\n");
        return;
    }
    for (int i = stack->top; i >= 0; i--) {
        fprintf(file, "[%d] ", stack->top - i);  // 0 = top
        u_display_bank_customer(&stack->customers[i], file);
    }
    fprintf(file, "\n");
}

void stack_save_to_file(Stack *stack, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "wb" : "w");
    if (!f) { printf("Cannot open file: %s\n", filename); return; }

    int size = stack_size(stack);
    if (binary) {
        fwrite(&size, sizeof(int), 1, f);
        fwrite(stack->customers, sizeof(uBankCustomer), size, f);
    } else {
        fprintf(f, "%d\n", size);
        for (int i = 0; i <= stack->top; i++)
            u_display_bank_customer(&stack->customers[i], f);
    }
    fclose(f);
}

void stack_load_from_file(Stack *stack, const char *filename, _Bool binary) {
    FILE *f = fopen(filename, binary ? "rb" : "r");
    if (!f) { printf("Cannot open file: %s\n", filename); return; }

    int size;
    if (binary) {
        fread(&size, sizeof(int), 1, f);
        for (int i = 0; i < size; i++) {
            uBankCustomer c;
            fread(&c, sizeof(uBankCustomer), 1, f);
            stack_push(stack, c);
        }
    } else {
        fscanf(f, "%d", &size);
        u_read_customers(stack->customers, size, f);
        stack->top = size - 1;
    }
    fclose(f);
}

void stack_menu() {
    Stack *stack = stack_create(10);
    int choice;

    do {
        printf("\nStack menu:\n");
        printf("1 - Push customer\n2 - Pop customer\n3 - Peek top\n");
        printf("4 - Display all\n5 - Find by account no\n6 - Find by position\n");
        printf("7 - Save to file\n8 - Load from file\n0 - Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                uBankCustomer c;
                u_read_customer(&c, stdin);
                stack_push(stack, c);
                printf("Customer pushed.\n");
                break;
            }
            case 2: {
                uBankCustomer c = stack_pop(stack);
                if (c.account_no) {
                    printf("Popped: ");
                    u_display_bank_customer(&c, stdout);
                }
                break;
            }
            case 3: {
                uBankCustomer c = stack_peek(stack);
                if (c.account_no)
                    u_display_bank_customer(&c, stdout);
                break;
            }
            case 4:
                stack_print(stack, stdout);
                break;
            case 5: {
                int no; printf("Account No: ");
                scanf("%d", &no);
                uBankCustomer *c = stack_find_by_no(stack, no);
                if (c)
                    u_display_bank_customer(c, stdout);
                else
                    printf("Not found.\n");
                break;
            }
            case 6: {
                int pos; printf("Position (0=top): "); scanf("%d", &pos);
                uBankCustomer *c = stack_find_by_position(stack, pos);
                if (c) u_display_bank_customer(c, stdout);
                break;
            }
            case 7: {
                char fname[256]; int mode;
                printf("Filename: ");
                scanf("%255s", fname);
                printf("Mode (0=text, 1=binary): ");
                scanf("%d", &mode);
                stack_save_to_file(stack, fname, mode);
                break;
            }
            case 8: {
                char fname[256]; int mode;
                printf("Filename: ");
                scanf("%255s", fname);
                printf("Mode (0=text, 1=binary): ");
                scanf("%d", &mode);
                stack_load_from_file(stack, fname, mode);
                break;
            }
            case 0: break;
            default: printf("Unknown command.\n");
        }
    } while (choice != 0);

    stack_destroy(stack);
}