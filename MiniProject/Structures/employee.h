#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD

struct Employee {
    int id;
    char name[30];
    char gender;
    int age;
    int type;  // 0: Bank employee, 1: Manager, 2: Admisitrator

    char username[35];
    char password[35];
};

#endif