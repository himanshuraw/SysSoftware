#include <stdbool.h>

#include "constants.h"
#ifndef CUSTOMER_RECORD
#define CUSTOMER_RECORD

struct Customer {
    char name[NAME_SIZE];
    char gender;
    int age;

    char username[NAME_SIZE + 5];
    char password[PASSWORD_SIZE];

    int account_number;  // 1 2 3 4 ....
    bool is_active;
    float balance;
    int loan;
    int transactions[MAX_TRANSACTIONS];
    int ptr;
};

#endif