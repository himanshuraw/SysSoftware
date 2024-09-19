/*
========================================================================================================
Name : 18
Author : Himanshu Rawat
Description : Write a program to find out total number of directories on the pwd. Execute ls -l | grep ^d | wc. Use only dup2.
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(){
	int pipe1[2], pipe2[2];
	int pid1, pid2;
	if(pipe(pipe1) == -1){
		perror("Pipe1");
		exit(1);
	}
	if(pipe(pipe2) == -1){
                perror("Pipe2");
		exit(1);
	}

	pid1 = fork();
	if(pid1 == 0){
		pid2 = fork();
		if(pid2 == 0){
			close(pipe1[0]);
			close(pipe1[1]);
			close(pipe2[1]);

			close(0);

			dup2(pipe2[0], 0);
			execl("/usr/bin/wc", "wc", NULL);
		} else {
			close(pipe1[1]);
			close(pipe2[0]);

			close(0);
			dup2(pipe1[0],0);

			close(1);
                        dup2(pipe2[1],1);
			execl("/usr/bin/grep", "grep", "^d", NULL);
		}
	} else {
		close(pipe2[0]);
		close(pipe2[1]);
		close(pipe1[0]);
		
		close(1);
		dup2(pipe1[1],1);

		execl("/usr/bin/ls", "ls", "-l", NULL);
	}
}
/*
========================================================================================================
Output:

./a.out
      0       0       0

========================================================================================================
*/
