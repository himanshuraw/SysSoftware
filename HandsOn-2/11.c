/*
========================================================================================================
Name : 9
Author : Himanshu Rawat
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
signal - Use sigaction system call.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

void restore_default_action(int signo) {
	printf("\nSIGINT caught, Resetting to default action.\n");
	
	struct sigaction sa_default;
	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
    	sa_default.sa_flags = 0;
    	sigaction(SIGINT, &sa_default, NULL);
    
}

int main() {
    	struct sigaction sa_ignore, sa_reset;

    	sa_ignore.sa_handler = SIG_IGN;
    	sigemptyset(&sa_ignore.sa_mask);
    	sa_ignore.sa_flags = 0;
    	sigaction(SIGINT, &sa_ignore, NULL);

   	printf("SIGINT is ignored.\n");
    	sleep(10);

    	sa_reset.sa_handler = restore_default_action;
    	sigemptyset(&sa_reset.sa_mask);
   	sa_reset.sa_flags = 0;
   	sigaction(SIGINT, &sa_reset, NULL);

   	printf("SIGINT will now be caught and reset to its default action.\n");
    
    	while (1) {
    	    pause();
   	}
}

/*
========================================================================================================
Output:

./a.out
SIGINT is ignored.
^CSIGINT will now be caught and reset to its default action.
^C
SIGINT caught, Resetting to default action.
^C

========================================================================================================
*/

