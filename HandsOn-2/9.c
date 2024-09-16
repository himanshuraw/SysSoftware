/*
========================================================================================================
Name : 9
Author : Himanshu Rawat
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
signal - Use signal system call.
Date: 16 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void restore_default_action(int signo) {
    printf("SIGINT caught. Resetting to default action.\n");
    signal(SIGINT, SIG_DFL);
}

int main() {
    signal(SIGINT, SIG_IGN);
    printf("SIGINT is ignore.\n");
    sleep(10);

    printf("\nResetting SIGINT to default action.\n");
    signal(SIGINT, restore_default_action); 

    sleep(10);
    printf("Program is ending normally. If you pressed Ctrl+C before, it was caught.\n");

}


/*
========================================================================================================
Output:

./a.out
SIGINT is ignore.
^C^CResetting SIGINT to default action.
^CSIGINT caught. Resetting to default action.
Program is ending normally. If you pressed Ctrl+C before, it was caught.

========================================================================================================
*/

