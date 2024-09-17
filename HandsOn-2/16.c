/*
========================================================================================================
Name : 16
Author : Himanshu Rawat
Description : Write a program to send and receive data from parent to child vice versa. Use two way communication.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main(){
	int pipefdChild[2];
       	int pipefdParent[2];
	char buffer[100];

	if(pipe(pipefdChild) == -1) {
		perror("Child pipe");
		exit(1);
	}

	if(pipe(pipefdParent) == -1) {
                perror("Parent pipe");
                exit(1);
        }

	int pid = fork();
	
	if(pid == -1){
		perror("Fork");
		exit(1);
	}

	if(pid == 0){
		close(pipefdChild[0]);
		close(pipefdParent[1]);

		//Parent -> Child
		read(pipefdParent[0], buffer, sizeof(buffer));
		printf("Parent: %s\n", buffer);

		//Child -> Parent
		const char* msg = "Hello";
		write(pipefdChild[1], msg, strlen(msg) + 1);

		close(pipefdChild[1]);
                close(pipefdParent[0]);
		exit(0);
	}else{
		close(pipefdChild[1]);
                close(pipefdParent[0]);

		//Child -> Parent
                const char* msg = "Hello";
                write(pipefdParent[1], msg, strlen(msg) + 1);

		//Child -> Parent
		read(pipefdChild[0], buffer, sizeof(buffer));
                printf("Child: %s\n", buffer);

		close(pipefdChild[0]);
                close(pipefdParent[1]);
		wait(NULL);
	}
}
/*
========================================================================================================
Output:

./a.out
Parent: Hello
Child: Hello

========================================================================================================
*/
