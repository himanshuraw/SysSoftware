#include "constants.h"
#ifndef CUSTOMER_RECORD
#define CUSTOMER_RECORD

struct Customer {
    int id;
    char name[NAME_SIZE];
    char gender;
    int age;

    char username[NAME_SIZE + 5];
    char password[PASSWORD_SIZE];

    bool is_active;
    int account_number;
    long int balance;
    int transactions[MAX_TRANSACTIONS];
};

#endif