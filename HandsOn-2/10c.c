/*
========================================================================================================
Name : 10a
Author : Himanshu Rawat
Description : Write a separate program using sigaction system call to catch the following signals.
		a. SIGSEGV
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void sigfpe_handler(int signo) {
	printf("Caught SIGFPE (Floating-point exception)\n");
	exit(1);
}

int main() {
	 struct sigaction sa;
	 sa.sa_handler = sigfpe_handler;
	 sigemptyset(&sa.sa_mask);
	 sa.sa_flags = 0;

	 sigaction(SIGFPE, &sa, NULL);

	 int x = 1 / 0;
}

/*
========================================================================================================
Output:
./a.out
Caught SIGFPE (Floating-point exception)
========================================================================================================
*/

