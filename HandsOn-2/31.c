/*
========================================================================================================
Name : 31
Author : Himanshu Rawat
Description : Write a program to create a semaphore and initialize value to the semaphore.
		a. Create a binary semaphore.
		b. Create a counting.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef union {
	int val;
	struct semid_ds *buf;
	unsigned short int *arr;
} sem_union;

int main() {
	//Binary semaphore
	sem_union binary;
	key_t k = ftok(".", 3);
	int sid = semget(k, 1, IPC_CREAT | 0744);
	binary.val = 1;
	printf("binary semaphore created\n");
	semctl(sid, 0, SETVAL, binary);

	//Counting semaphore
	sem_union counting;
	k = ftok(".", 4);
	sid = semget(k, 1, IPC_CREAT | 0744);
	counting.val = 10;
	printf("counting semaphore created size is :%d \n", counting.val);
	semctl(sid, 0, SETVAL, counting);
}


/*
========================================================================================================
Output:

./a.out
binary semaphore created
counting semaphore created size is :10

------ Semaphore Arrays --------
key        semid      owner      perms      nsems
0x031493ca 0          himanshu-r 744        1
0x041493ca 1          himanshu-r 744        1

========================================================================================================
*/
