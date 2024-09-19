/*
========================================================================================================
Name : 28
Author : Himanshu Rawat
Description : Write a program to change the existing message queue permission (use msqid_ds structure).
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int main() {
	key_t k = ftok(".", 1);
	int qid = msgget(k, 0666 | IPC_CREAT);
	struct msqid_ds info;

	msgctl(qid, IPC_STAT, &info);
	printf("Current permissions: %o\n", info.msg_perm.mode);

	info.msg_perm.mode = 0777;

	msgctl(qid, IPC_SET, &info);
	msgctl(qid, IPC_STAT, &info);
	printf("Updated permissions: %o\n", info.msg_perm.mode);

	return 0;
}

/*
========================================================================================================
Output:

./a.out
Current permissions: 666
Updated permissions: 777

========================================================================================================
*/
