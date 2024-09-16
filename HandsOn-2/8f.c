/*
========================================================================================================
Name : 8f
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		f. SIGVTALRM (use setitimer system call)
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/time.h>

void sigvtalrm_handler(int signo) {
    printf("Caught SIGVTALRM (Virtual timer alarm)\n");
    exit(1);
}

int main() {
    signal(SIGVTALRM, sigvtalrm_handler);
    
    struct itimerval timer;
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    while(1);
}

/*
========================================================================================================
Output:

./a.out
Caught SIGVTALRM (Virtual timer alarm)

========================================================================================================
*/

