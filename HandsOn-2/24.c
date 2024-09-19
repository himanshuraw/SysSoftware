/*
========================================================================================================
Name : 24
Author : Himanshu Rawat
Description : Write a program to create a message queue and print the key and message queue id.
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct msg_buf {
	long type;
	char text[100];
} msg_buf;

int main() {
	msg_buf msg;

	long k = ftok(".", 1);
	if (k == -1) {
		perror("ftok failed");
		return 1;
	}
	printf("Key: %ld\n", k);

	int qid = msgget(k, 0666 | IPC_CREAT);
	if (qid == -1) {
		perror("msgget failed");
		return 1;
	}
	printf("Queue ID: %d\n", qid);
}



/*
========================================================================================================
Output:

Key: 18125770
Queue ID: 0

========================================================================================================
*/
