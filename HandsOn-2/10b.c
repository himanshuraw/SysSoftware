/*
========================================================================================================
Name : 10b
Author : Himanshu Rawat
Description : Write a separate program using sigaction system call to catch the following signals.
		b. SIGINT
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void sigint_handler(int signo) {
	printf("Caught SIGINT (Interrupt signal)\n");
	exit(1);
}

int main() {
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);

	printf("Press Ctrl+C to trigger SIGINT...\n");
	while (1) {
		pause();
	}
}

/*
========================================================================================================
Output:

./a.out
Press Ctrl+C to trigger SIGINT...
^CCaught SIGINT (Interrupt signal)

========================================================================================================
*/

