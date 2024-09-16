/*
========================================================================================================
Name : 10a
Author : Himanshu Rawat
Description : Write a separate program using sigaction system call to catch the following signals.
		a. SIGSEGV
Date: 16 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigsegv_handler(int signo) {
    printf("Caught SIGSEGV (Segmentation fault)\n");
    exit(1);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGSEGV, &sa, NULL);

    int *ptr = NULL;
    *ptr = 42; 

}

/*
========================================================================================================
Output:
./a.out
Caught SIGSEGV (Segmentation fault)

========================================================================================================
*/

