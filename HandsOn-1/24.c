/*
========================================================================================================

Name: 24
Author: Himanshu Rawat
Description: Write a program to create an orphan process.
Date: 28th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();
	if(!pid){
		printf("Parent PID of child (Before PID): %d\n",getppid());
		sleep(3);
		printf("Parent PID of child (After PID): %d\n",getppid());
	}else{
		sleep(1);
	}
}

/*
========================================================================================================
Output:

./a.out
Parent PID of child (Before PID): 16125
Parent PID of child (After PID): 1843

========================================================================================================
*/

