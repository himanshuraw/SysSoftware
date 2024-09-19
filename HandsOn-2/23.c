/*
========================================================================================================
Name : 23
Author : Himanshu Rawat
Description : Write a program to print the maximum number of files that can be opened within a process and size of a pipe (circular buffer).
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<stdio.h>

int main() {
	long maxOpenFiles; 
	long pipeSize;

	char *path = "23_fifo";
	mkfifo(path, S_IRWXU);

	maxOpenFiles = sysconf(_SC_OPEN_MAX);
	printf("Max open files per process: %ld\n", maxOpenFiles);

	printf("Max pipe size: %d\n", PIPE_BUF);

	return 0;
}

/*
========================================================================================================
Output:

./a.out
Max open files per process: 1024
Max pipe size: 4096

========================================================================================================
*/

