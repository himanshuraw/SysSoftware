/*
========================================================================================================
Name : 12
Author : Himanshu Rawat
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
the parent process from the child process.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main() {
	int pid = fork();
	
	if (pid < 0) {
		perror("fork failed");
		exit(1);
	} else if (pid == 0) {
		printf("Child process (PID: %d) created.\n", getpid());
		printf("Child sending SIGKILL to parent (PID: %d).\n", getppid());
		
		kill(getppid(), SIGKILL);
		
		printf("Child (PID: %d) is now an orphan.\n", getpid());
		sleep(5);
		printf("Child (PID: %d) is still alive and adopted by init (or another process PID: %d).\n", getpid(), getppid());
		exit(0);
	} else {
		printf("Parent process (PID: %d) created a child (PID: %d).\n", getpid(), pid);
		sleep(10);
	}
}

/*
========================================================================================================
Output:

./a.out
Parent process (PID: 7790) created a child (PID: 7791).
Child process (PID: 7791) created.
Child sending SIGKILL to parent (PID: 7790).
Child (PID: 7791) is now an orphan.
Killed
Child (PID: 7791) is still alive and adopted by init (or another process PID: 1852).
^C

========================================================================================================
*/


