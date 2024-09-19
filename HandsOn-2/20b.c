/*
========================================================================================================
Name : 20
Author : Himanshu Rawat
Description : Write two programs so that both can communicate by FIFO - Use one way communication.
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
        int fd = open("./tmp/fifo", O_RDONLY);
        if(fd == -1){
                perror("Open");
                exit(1);
        }

	char buffer[100];
	int n = read(fd, buffer, sizeof(buffer) -1);
	if(n == -1){
		perror("Read");
		exit(1);
	}

	buffer[n] = '\0';
	printf("message received: %s\n", buffer);
	close(fd);
}


/*
========================================================================================================
Output:

./b
message received: Message from writer

========================================================================================================
*/
