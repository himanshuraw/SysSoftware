/*
========================================================================================================

Name: 18a
Author: Himanshu Rawat
Description: Write a program to perform Record locking.
		a. Implement write lock
		b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
Date: 28th August, 2024

========================================================================================================
*/


#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

struct Record{
	int data;
};

int main(int argc, char** argv){
	if(argc<2){
		perror("Input more arguments");
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_WRONLY, 0777);
	if(fd == -1){
		perror("Error in opening File");
		return -1;
	}

	struct Record records[3];
	
	for(int i = 0; i<3; i++){
		records[i].data = 0;
	}

	int count = write(fd, &records, sizeof(records));
        if(count == -1){
                perror("Error in writing");
                close(fd);
                return -1;
        }
	printf("Record Initialized \n");
}
