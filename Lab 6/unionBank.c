#include "unionBank.h"
#include <stdlib.h>
#include <time.h>

// Display the passed date
void u_display_date(uDate *d, FILE* file) {
    int day = d->day >> 26;
    int month = (d->month >> 22) & 0xF;
    int year = d->year & ((1 << 22) - 1);
    fprintf(file, "%.2d/%.2d/%.2d ", day, month, year);
}

// Save current date from the machine
void u_save_current_date(uDate *d) {
    time_t t = time(0);
    struct tm *tm = localtime(&t);
    d->day = 0;
    d->day += tm->tm_mday << 26;
    d->month += (tm->tm_mon + 1) << 22;
    d->year += tm->tm_year + 1900;
}

// Read bank customer members
void u_read_customer(uBankCustomer *customer, FILE* file) {
    unsigned int acc_no;
    int day, month, year;

    if (file == stdin) {
        printf("Account No:");
        scanf("%u", &acc_no);
        customer->account_no = acc_no;
        printf("Name:");
        scanf("%49s", customer->name);
        printf("Surname:");
        scanf("%49s", customer->surname);
        printf("Account Type (0 - deposit, 1 - salary account, "
            "2 - credit account, 3 - bonus account for pensioners): ");
        scanf("%d", &customer->type);
        printf("Opening Date (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &day, &month, &year);
        printf("Balance: ");
        scanf("%lf", &customer->balance);
    } else {
        fscanf(file, "%u", &acc_no);
        customer->account_no = acc_no;
        int t2;
        fscanf(file, "%49s %49s %d", customer->name, customer->surname, &t2);
        customer->type = (enum uAccountType)t2;
        fscanf(file, "%d/%d/%d", &day, &month, &year);
        fscanf(file, "%lf", &customer->balance);
    }
    customer->opening_date.day = 0;
    customer->opening_date.day += day<<26;
    customer->opening_date.month += month<<22;
    customer->opening_date.year += year;
    u_save_current_date(&(customer->last_account_access_date));
}

// Read an array of bank customers
void u_read_customers(uBankCustomer *customers, int n, FILE* file) {
    for (int i = 0; i < n; i++) {
        u_read_customer(customers + i, file);
    }
}

// Find bank customer by account no
uBankCustomer *u_find_customer_by_no(uBankCustomer *customers, int n, int j) {
    for (int i = 0; i < n; i++) {
        if ((customers + i)->account_no == (unsigned)j) {
            return customers + i;
        }
    }
    return NULL;
}

// Print the menu for u_find_customer_by_no
_Bool u_print_find_customer_by_no(uBankCustomer *customers, int n,uBankCustomer **customer) {
    printf("Enter Account No. of the Customer: ");
    int i;
    scanf("%d", &i);
    *customer = u_find_customer_by_no(customers, n, i);
    if (*customer == NULL) {
        printf("No such customer found!");
        return 1;
    }
    return 0;
}

// Display the account type
void u_display_account_type(uBankCustomer* customer, FILE* file) {
    if (file == stdout) {
        printf("Account Type: ");
    }
    switch (customer->type) {
        case U_CREDIT_ACCOUNT: fprintf(file, "credit_account"); break;
        case U_SALARY_ACCOUNT: fprintf(file, "salary_account"); break;
        case U_DEPOSIT: fprintf(file, "deposit"); break;
        case U_BONUS_PROGRAM_FOR_PENSIONERS: fprintf(file, "bonus_program_for_pensioners"); break;
    }
    fprintf(file, "\n");
}

// Display bank customer members
void u_display_bank_customer(uBankCustomer *customer, FILE* file) {
    if (file == stdout) {
        printf("Account No: %d\n", customer->account_no);
        printf("Name: %s\n", customer->name);
        printf("Surname: %s\n", customer->surname);
        u_display_account_type(customer, file);
        printf("Opening Date: ");
        u_display_date(&(customer->opening_date), file);
        printf("\n");
        printf("Balance: %.2lf\n", customer->balance);
        printf("Last Account Access Date: ");
    } else {
        fprintf(file,"%d %s %s %d ", customer->account_no, customer->name, customer->surname, customer->account_no);
        u_display_date(&(customer->opening_date), file);
        fprintf(file,"%.2lf ", customer->balance);
    }
    u_display_date(&(customer->last_account_access_date), file);
    fprintf(file, "\n");
}

// Display all bank customers
void u_display_all_bank_customers(uBankCustomer *customer, int n, FILE* file) {
    for (int i = 0; i < n; i++) {
        u_display_bank_customer(&(customer[i]), file);
    }
}