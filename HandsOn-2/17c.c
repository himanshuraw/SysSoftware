/*
========================================================================================================
Name : 17c
Author : Himanshu Rawat
Description : Write a program to execute ls -l | wc.
		c. Use fcntl
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>

void main()
{
	int pipefds[2];
	pid_t pid;

	if (pipe(pipefds) == -1){
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if(pid == -1){
		perror("fork");
		exit(1);
	}
	
	if (pid == 0){
		close(0);
		fcntl(pipefds[0], F_DUPFD, 0);
		close(pipefds[1]);
		execl("/usr/bin/wc", "wc", NULL);
	}
	else{
		close(1);
		fcntl(pipefds[1], F_DUPFD, 1);
		close(pipefds[0]);
		execl("/usr/bin/ls", "ls", "-l", NULL);
	}
}

/*
========================================================================================================
Output:

./a.out
     33     290    2157

========================================================================================================
*/
