/*
========================================================================================================
Name : 21
Author : Himanshu Rawat
Description : Write two programs so that both can communicate by FIFO - Use two way communication.
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	const char *fifo1 = "./tmp/fifo2";
	const char *fifo2 = "./tmp/fifo";

	int fd_read = open(fifo2, O_RDONLY);

        char buffer[128];
	int n = read(fd_read, buffer, sizeof(buffer) - 1);
	
	buffer[n] = '\0';
        printf("message received: %s\n", buffer);
        close(fd_read);

	int fd_write = open(fifo1, O_WRONLY);

	const char msg[] = "hello from 21b";
	write(fd_write, &msg, sizeof(msg));

	close(fd_write);
}

/*
========================================================================================================
Output:

./b
message received: hello from 21a

========================================================================================================
*/

