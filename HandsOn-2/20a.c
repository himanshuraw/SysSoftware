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
	int fd = open("./tmp/fifo", O_WRONLY);
	if(fd == -1){
		perror("Open");
		exit(1);
	}

	const char msg[] = "Message from writer";
	write(fd, &msg, sizeof(msg));
	printf("message sent\n");
	close(fd);
}


/*
========================================================================================================
Output:

./a
message sent

========================================================================================================
*/
