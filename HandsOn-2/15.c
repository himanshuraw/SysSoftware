/*
========================================================================================================
Name : 15
Author : Himanshu Rawat
Description : Write a simple program to send some data from parent to the child process.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main() {
	int pipefd[2];
	char buffer[100];

	if(pipe(pipefd) == -1){
		perror("Pipe");
		exit(1);
	}

	int pid = fork();

	if(pid == -1){
		perror("Fork");
		exit(1);
	}

	if(pid == 0){
		close(pipefd[1]);
		read(pipefd[0], buffer, sizeof(buffer));
		printf("Child process received from parent: %s\n", buffer);
		close(pipefd[0]);
		exit(0);
	}
	else {
		close(pipefd[0]);
		const char *data = "Message";
		write(pipefd[1], data, strlen(data) + 1);
		close(pipefd[1]);
		wait(NULL);
	}
}

/*
========================================================================================================
Output:

./a.out
Child process received from parent: Message

========================================================================================================
*/
