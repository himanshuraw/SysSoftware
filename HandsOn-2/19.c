/*
========================================================================================================
Name : 19
Author : Himanshu Rawat
Description : Create a FIFO file by 
		a.mknod command
		b.mkfifo command
		c.use strace command to find out,which command(mknod or mkfifo) is better.
		d.mknod system call
		e.mkfifo library function
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/stat.h>
int main(){
	int pipe1 = mkfifo("./mkfifo", 0666);
	if(pipe1 == -1)
		perror("mkfifo");
	else
		printf("Fifo created using mkfifo\n");

	int pipe2 = mknod("./mknod", S_IFIFO | 0666, 0);
	if(pipe2 == -1)
                perror("mknod");
	else
		printf("Fifo created using mknod\n");
}

/*
========================================================================================================
Output:

sudo ./a.out
Fifo created using mkfifo
Fifo created using mknod

========================================================================================================
*/
