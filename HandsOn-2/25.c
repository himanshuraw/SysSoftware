/*
========================================================================================================
Name : 25
Author : Himanshu Rawat
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures):
		a. access permission
		b. uid, gid
		c. time of last message sent and received
		d. time of last change in the message queue
		e. size of the queue
		f. number of messages in the queue
		g. maximum number of bytes allowed
		h. pid of the msgsnd and msgrcv
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h>

int main() {
	key_t k;
	int qid;
	struct msqid_ds info;

	k = ftok(".", 1);
	if (k == -1) {
		perror("ftok failed");
		return 1;
	}

	qid = msgget(k, 0666 | IPC_CREAT);
	if (qid == -1) {
		perror("msgget failed");
		return 1;
	}

	if (msgctl(qid, IPC_STAT, &info) == -1) {
		perror("msgctl failed");
		return 1;
	}

	printf("Permissions: %o\n", info.msg_perm.mode);
	printf("UID: %d\n", info.msg_perm.uid);
	printf("GID: %d\n", info.msg_perm.gid);
	printf("Last sent: %s", ctime(&info.msg_stime));
	printf("Last received: %s", ctime(&info.msg_rtime));
	printf("Last change: %s", ctime(&info.msg_ctime));
	printf("Current size (bytes): %lu\n", info.__msg_cbytes);
	printf("Messages in queue: %lu\n", info.msg_qnum);
	printf("Max bytes allowed: %lu\n", info.msg_qbytes);
	printf("Last msgsnd PID: %d\n", info.msg_lspid);
	printf("Last msgrcv PID: %d\n", info.msg_lrpid);

	return 0;
}

/*
========================================================================================================
Output:

Permissions: 666
UID: 1000
GID: 1000
Last sent: Thu Jan  1 05:30:00 1970
Last received: Thu Jan  1 05:30:00 1970
Last change: Thu Sep 19 16:19:26 2024
Current size (bytes): 0
Messages in queue: 0
Max bytes allowed: 16384
Last msgsnd PID: 0
Last msgrcv PID: 0

========================================================================================================
*/
