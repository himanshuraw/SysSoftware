#include "constants.h"
#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD

struct Employee {
    int id;  // 0 1 2 3 ...
    char name[NAME_SIZE];
    char gender;
    int age;

    int loans[LOAN_SIZE];
    int ptr;
    int role;  // 0: Employee, 1: Manager

    char username[NAME_SIZE + 5];
    char password[PASSWORD_SIZE];
};

#endif