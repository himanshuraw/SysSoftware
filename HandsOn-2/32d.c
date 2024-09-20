/*
========================================================================================================
Name : 32d
Author : Himanshu Rawat
Description : Write a program to implement semaphore to protect any critical section.
		d. Remove the created semaphore.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

typedef union sem_union {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
} sem_union;

int lock_sem(int sem_id) {
	struct sembuf sem_op;
	sem_op.sem_num = 0;
	sem_op.sem_flg = 0;
	sem_op.sem_op = -1;
	return semop(sem_id, &sem_op, 1);
}

int unlock_sem(int sem_id) {
	struct sembuf sem_op;
	sem_op.sem_num = 0;
	sem_op.sem_flg = 0;
	sem_op.sem_op = 1;
	return semop(sem_id, &sem_op, 1);
}

int main() {
	key_t shm_key = ftok(".", 666);
	int shm_id = shmget(shm_key, 1024, IPC_CREAT | 0744);
	char *shm_ptr = shmat(shm_id, (void *)0, 0);
	key_t sem_key = ftok(".", 335);
	int sem_id = semget(sem_key, 1, 0);
	sem_union sem_set;

	if (sem_id == -1) {
		sem_id = semget(sem_key, 1, IPC_CREAT | 0700);
		sem_set.val = 1;
		semctl(sem_id, 0, SETVAL, sem_set);
	}

	printf("Entering to obtain lock on the critical section\n");

	if (lock_sem(sem_id) == -1) {
		return 1;
	}

	printf("Obtained lock on critical section!\n");
	printf("Now entering critical section!\n");

	printf("Write in shared memory\n");
	scanf("%[^\n]", shm_ptr);
	printf("Data from shared memory: %s\n", shm_ptr);
	printf("Entering to exit from critical section!\n");

	if (unlock_sem(sem_id) == -1) {
		return 1;
	}

	printf("Deleting binary semaphore\n");
	semctl(sem_id, 0, IPC_RMID);
}


/*
========================================================================================================
Output:

./a.out
Entering to obtain lock on the critical section
Obtained lock on critical section!
Now entering critical section!
Write in shared memory
data
Data from shared memory: data
Entering to exit from critical section!
Deleting binary semaphore

========================================================================================================
*/
