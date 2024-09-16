/*
========================================================================================================
Name : 8d
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		d. SIGALRM(use alarm system call)
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

void sigalrm_handler(int signo) {
    printf("Caught SIGALRM (Alarm signal)\n");
}

int main() {
    signal(SIGALRM, sigalrm_handler);
    alarm(2);
    pause();
}

/*
========================================================================================================
Output:

./a.out
Caught SIGALRM (Alarm signal)

========================================================================================================
*/

