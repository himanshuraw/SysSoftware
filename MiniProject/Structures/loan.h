#include <stdbool.h>

#ifndef LOAN
#define LOAN

struct Loan {
    int id;  // 1, 2, 3 , 4
    long int amount;
    int customer_account_number;
    int status;  // -1: pending, 0 rejected, 1 approved
    bool assigned;
};

#endif