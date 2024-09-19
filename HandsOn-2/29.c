/*
========================================================================================================
Name : 29
Author : Himanshu Rawat
Description : Write a program to remove the message queue.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int main() {
	key_t k = ftok(".", 1);
	int qid = msgget(k, 0666 | IPC_CREAT);

	if (msgctl(qid, IPC_RMID, NULL) == 0) {
		printf("Message queue removed successfully.\n");
	} else {
		printf("Failed to remove message queue.\n");
	}

	return 0;
}


/*
========================================================================================================
Output:

./a.out
Message queue removed successfully.

========================================================================================================
*/

