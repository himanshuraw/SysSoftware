/*
========================================================================================================
Name : 8a
Author : Himanshu Rawat
Description : Write a separate program using signal system call to catch the following signals.
		a. SIGSEGV
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void sigsegv_handler(int signo) {
	printf("Caught SIGSEGV (Segmentation fault)\n");
	exit(1);
}

void trigger_segfault() {
	int *ptr = NULL;
	*ptr = 42; 
}

int main() {
    	signal(SIGSEGV, sigsegv_handler);
    	trigger_segfault();
}


/*
========================================================================================================
Output:

./a.out
Caught SIGSEGV (Segmentation fault)

========================================================================================================
*/


