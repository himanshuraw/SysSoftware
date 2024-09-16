/*
========================================================================================================
Name : 8b
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		b. SIGINT
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void sigint_handler(int signo) {
    printf("\nCaught SIGINT (Interrupt signal)\n");
    exit(1);
}

int main() {
    signal(SIGINT, sigint_handler);
    printf("Press Ctrl+C to trigger SIGINT...\n");
    while (1);
}

/*
========================================================================================================
Output:

./a.out
Press Ctrl+C to trigger SIGINT...
^C
Caught SIGINT (Interrupt signal)

========================================================================================================
*/

