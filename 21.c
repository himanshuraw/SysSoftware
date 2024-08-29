/*
========================================================================================================

Name: 21
Author: Himanshu Rawat
Description: Write a program, call fork and print the parent and child process id.
Date: 28th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();
	if(pid == -1){
		perror("Error in for");
		return -1;
	}
	if(pid == 0){
		printf("This is child process\n- child PID : %d\n- parent PID %d\n\n", getpid(), getppid());
	}	
	printf("This is parent process\n- child PID : %d\n- parent PID %d\n\n", pid, getpid());
}

/*
========================================================================================================
Output:

./a.out
This is parent process
- child PID : 20076
- parent PID 20075

This is child process
- child PID : 20076
- parent PID 20075

This is parent process
- child PID : 0
- parent PID 20076

========================================================================================================
*/
