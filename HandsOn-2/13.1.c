/*
========================================================================================================
Name : 12
Author : Himanshu Rawat
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program
will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

void signal_handler(){
	printf("Received SIGSTOP");
}

int main(){
	signal(SIGSTOP, signal_handler);

	sleep(20);
}
