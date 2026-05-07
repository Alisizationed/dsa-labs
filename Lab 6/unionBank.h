#ifndef SDA_LABS_UNIONBANK_H
#define SDA_LABS_UNIONBANK_H
#include <stdio.h>

// enum for account type of bank customer
enum uAccountType {
    U_DEPOSIT, U_SALARY_ACCOUNT, U_CREDIT_ACCOUNT,
    U_BONUS_PROGRAM_FOR_PENSIONERS
};

// union for date
typedef union {
    int day;
    int month;
    int year;
    int rawDate;
} uDate;

// struct for bank customer
typedef struct {
    unsigned int account_no : 10;
    char name[50];
    char surname[50];
    enum uAccountType type;
    uDate opening_date;
    double balance;
    uDate last_account_access_date;
} uBankCustomer;

// Display a decoded date
void u_display_date(uDate *d, FILE* file);

// Save current system date
void u_save_current_date(uDate *d);

// Read a single customer
void u_read_customer(uBankCustomer *customer, FILE* file);

// Read multiple customers
void u_read_customers(uBankCustomer *customers, int n, FILE* file);

// Find customer by account number
uBankCustomer *u_find_customer_by_no(uBankCustomer *customers, int n, int account_no);

// CLI helper for finding customer
_Bool u_print_find_customer_by_no(uBankCustomer *customers, int n, uBankCustomer **customer);

// Display account type
void u_display_account_type(uBankCustomer *customer, FILE* file);

// Display one customer
void u_display_bank_customer(uBankCustomer *customer, FILE* file);

// Display all customers
void u_display_all_bank_customers(uBankCustomer *customer, int n, FILE* file);

#endif //SDA_LABS_UNIONBANK_H
