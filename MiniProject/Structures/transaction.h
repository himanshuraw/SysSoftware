#ifndef TRANSACTION
#define TRANSACTION

#include <time.h>

struct Transaction {
    int id;
    int account_number;
    float amount;
    int operation;  // 0: Deposit, 1: Withdraw, 2: Transfer_in, 3: Transfer_out
    time_t time;
};
#endif