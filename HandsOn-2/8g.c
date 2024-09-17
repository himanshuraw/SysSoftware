/*
========================================================================================================
Name : 8g
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		g. Caught SIGPROF (Profiling timer alarm)
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/time.h>

void sigprof_handler(int signo) {
	printf("Caught SIGPROF (Profiling timer alarm)\n");
	exit(0);
}

int main() {
	signal(SIGPROF, sigprof_handler);
	
	struct itimerval timer;
	timer.it_value.tv_sec = 2;  
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	setitimer(ITIMER_PROF, &timer, NULL);
	
	while(1);
}

/*
========================================================================================================
Output:

./a.out
Caught SIGPROF (Profiling timer alarm)

========================================================================================================
*/


