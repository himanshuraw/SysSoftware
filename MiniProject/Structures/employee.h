#include "constants.h"
#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD

struct Employee {
    int id;
    char name[NAME_SIZE];
    char gender;
    int age;

    char username[NAME_SIZE + 5];
    char password[PASSWORD_SIZE];
};

#endif