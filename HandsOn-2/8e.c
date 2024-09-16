/*
========================================================================================================
Name : 8e
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		e. SIGALRM (use setitimer system call)
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>

void sigalrm_handler(int signo) {
    printf("Caught SIGALRM\n");
}

int main() {
    signal(SIGALRM, sigalrm_handler);
    
    struct itimerval timer;
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    pause();
}

/*
========================================================================================================
Output:

./a.out
Caught SIGALRM

========================================================================================================
*/

