/*
========================================================================================================
Name : 8c
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		c. SIGFPE
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void sigfpe_handler(int signo) {
    printf("Caught SIGFPE (Floating-point exception)\n");
    exit(1);
}

int main() {
    signal(SIGFPE, sigfpe_handler);
    int x = 1 / 0; 
}

/*
========================================================================================================
Output:

./a.out
Caught SIGFPE (Floating-point exception)

========================================================================================================
*/

