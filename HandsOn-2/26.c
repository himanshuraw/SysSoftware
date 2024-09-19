/*
========================================================================================================
Name : 26
Author : Himanshu Rawat
Description : Write a program to send messages to the message queue. Check $ipcs -q.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#define MAX_TEXT 100

struct msg_buffer {
	long type;
	char text[MAX_TEXT];
} msg;

int main() {
	key_t k;
	int qid;
	
	k = ftok(".", 1);

	qid = msgget(k, 0666 | IPC_CREAT);

	msg.type = 1;
	printf("Enter a message: ");
	fgets(msg.text, MAX_TEXT, stdin);
	msg.text[strcspn(msg.text, "\n")] = '\0';

	if (msgsnd(qid, &msg, sizeof(msg.text), 0) == -1) {
		perror("msgsnd failed");
		return 1;
	}

	printf("Message sent to queue: %s\n", msg.text);
}


/*
========================================================================================================
Output:

========================================================================================================
*/
