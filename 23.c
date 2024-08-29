/*
========================================================================================================

Name: 23
Author: Himanshu Rawat
Description: Write a program to create a Zombie state of the running program.
Date: 28th August, 2024

========================================================================================================
*/


#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();

	if(!pid){
		printf("Child Process [PID: %d]", getpid());
	}
	else{
		printf("Press a key to escape\n");
		getchar();

	}
}

/*
========================================================================================================
Output:

./a.out
Press a key to escape
Child Process [PID: 19099]

========================================================================================================
*/

