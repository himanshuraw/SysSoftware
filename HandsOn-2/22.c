/*
========================================================================================================
Name : 22
Author : Himanshu Rawat
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
Date: 17 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(){
	const char *path = "./tmp/fifo";
	if(mkfifo(path, 0666) == -1){
		perror("mkfifo");
	}

	int fd = open(path, O_RDONLY | O_NONBLOCK);
	if(fd == -1){
		perror("open");
		exit(1);
	}

	fd_set rdfds;;
	FD_ZERO(&rdfds);
	FD_SET(fd, &rdfds);

	struct timeval timeout = {10, 0};

	int flag = select(fd + 1, &rdfds, NULL, NULL, &timeout);

	if(flag == -1){
		perror("Select");
		close(fd);
		exit(1);
	}
	if (flag == 0) printf("Timeout\n");
	else if(FD_ISSET(fd, &rdfds)){
		char buffer[128];
		int n = read(fd, &buffer, sizeof(buffer));
		if(n > 0)
			printf("Data from FIFO: %s\n", buffer);
		else
			printf("No data");
	}
}

/*
========================================================================================================
Output:
./a.out
mkfifo: File exists
Data from FIFO: hello

========================================================================================================
*/
