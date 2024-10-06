#ifndef TRANSACTION
#define TRANSACTION

#include <time.h>

struct Transaction {
    int transaction_id;
    int account_number;
    int operation;  // 0: Deposit, 1: Withdraw, 2: Transfer
    time_t transaction_time;
};
#endif