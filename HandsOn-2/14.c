/*
========================================================================================================
Name : 14
Author : Himanshu Rawat
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date: 17 Sept, 2024.
========================================================================================================
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main() {
	int pipefd[2];
	int pid;
	char buffer[100];

	if(pipe(pipefd) == -1) {
		perror("pipe");
		exit(1);
	}

	pid = fork();
	if(pid == -1) {
		perror("fork");
		exit(1);
	}

	if(pid == 0) {
		close(pipefd[0]);
		const char *message = "Child process!";
		write(pipefd[1], message, strlen(message) + 1);
		close(pipefd[1]);
		exit(0);
	} else {
		close(pipefd[1]);
		read(pipefd[0], buffer, sizeof(buffer));
		printf("Received from child: %s\n", buffer);
		close(pipefd[0]);
	}
}

/*
========================================================================================================
Output:

./a.out
Received from child: Child process!

========================================================================================================
*/

