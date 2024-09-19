/*
========================================================================================================
Name : 27a
Author : Himanshu Rawat
Description : Write a program to receive messages from the message queue.
		a. With 0 as a flag
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_TEXT 100

struct msg_buffer {
	long type;
	char text[MAX_TEXT];
} msg;

int main() {
	key_t k = ftok(".", 1);
	int qid = msgget(k, 0666 | IPC_CREAT);

	msgrcv(qid, &msg, sizeof(msg.text), 1, 0);

	printf("Message received: %s\n", msg.text);

	return 0;
}



/*
========================================================================================================
Output:

./a.out
Message received: Here is the message

========================================================================================================
*/
