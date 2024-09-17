/*
========================================================================================================
Name : 13
Author : Himanshu Rawat
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program
will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
	int pid;
	printf("Enter PID: ");
	scanf("%d", &pid);
	if( kill(pid, SIGSTOP) == -1)
		perror("Error in sending kill signal");
}
