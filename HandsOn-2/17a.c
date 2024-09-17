/*
========================================================================================================
Name : 17.a
Author : Himanshu Rawat
Description : Write a program to execute ls -l | wc.
		a. Use dup
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main() {
	int pipefd[2];
	if(pipe(pipefd) == -1) {
		perror("pipe");
		exit(1);
	}
	int pid = fork();
	if(pid == -1) {
		perror("fork");
		exit(1);
	}
	if(pid == 0) {
		close(0);
		dup(pipefd[0]);
		close(pipefd[1]);

		execl("/usr/bin/wc", "wc", NULL);		
		perror("execl");
		exit(1);
	} else {
		close(1);
		dup(pipefd[1]);
		close(pipefd[0]);

		execl("/usr/bin/ls", "ls", "-l", NULL);
		perror("execl");
	//	wait(NULL);
		exit(1);
	}
}

/*
========================================================================================================
Output:
./a.out
     32     281    2089
 
========================================================================================================
*/
