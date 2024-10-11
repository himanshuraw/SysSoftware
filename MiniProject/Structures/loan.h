#include <stdbool.h>

#ifndef LOAN
#define LOAN

struct Loan {
    int id;
    long int amount;
    int customer_account_number;
    bool approved;
    bool assigned;
};

#endif