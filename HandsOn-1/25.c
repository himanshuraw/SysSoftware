/*
========================================================================================================

Name: 25
Author: Himanshu Rawat
Description: Write a program to create three child processes. The parent should wait for a particular child (use
waitpid system call).
Date: 29th August, 2024

========================================================================================================
*/


#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
	int pid1 = fork();
	if(!pid1){
		sleep(2);
		printf("First child\n");
	}
	else{
		int pid2 = fork();
		if(!pid2){
			sleep(3);
			printf("Second child\n");
		}
		else{
			int pid3 = fork();
			if(!pid3){
				sleep(4);
				printf("Third child\n");
			}
			else{
				printf("Waiting for 2nd Child\n");
				waitpid(pid2, NULL, 0);
			}
		}
	}
}

/*
========================================================================================================
Output:

./a.out
Waiting for 2nd Child
First child
Second child

========================================================================================================
*/

