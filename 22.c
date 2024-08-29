/*
========================================================================================================

Name: 22
Author: Himanshu Rawat
Description: Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file.
Date: 28th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char** argv){
	if(argc < 2){
		perror("Input more arguments");
		return -1;
	}
	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	if(fd == -1){
		perror("Error in opening File");
		return -1;
	}

	fork();
	char data[] = "data";

	int cnt = write(fd, &data, sizeof(data));
	if(cnt == -1){
		perror("Error in writing");
	}
	printf("Finished printing\n");
	close(fd);
}

/*
========================================================================================================
Output:

 ./a.out text.txt
Finished printing
Finished printing

In text.txt:
datadata

========================================================================================================
*/

