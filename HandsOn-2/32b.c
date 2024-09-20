/*
========================================================================================================
Name : 32b
Author : Himanshu Rawat
Description : Write a program to implement semaphore to protect any critical section.
		b. Protect shared memory from concurrent write access.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef union {
	int val;
	struct semid_ds *buf;
	unsigned short int *arr;
	struct seminfo *__buf;
} sem_union;

int main() {
	key_t shm_key = ftok(".", 666);
	int shm_id = shmget(shm_key, 1024, IPC_CREAT | 0744);
	char *shm_ptr = shmat(shm_id, NULL, 0);
	
	key_t sem_key = ftok(".", 332);
	if (sem_key == -1) {
		perror("Generating semaphore key");
		exit(1);
	}
	
	int sem_id = semget(sem_key, 1, 0);
	if (sem_id == -1) {
		sem_id = semget(sem_key, 1, IPC_CREAT | 0700);
		if (sem_id == -1) {
			perror("Creating semaphore");
			exit(1);
		}

		sem_union sem_val;
		sem_val.val = 1;
		if (semctl(sem_id, 0, SETVAL, sem_val) == -1) {
			perror("Setting semaphore value");
			exit(1);
		}
	}

	struct sembuf s_op;
	s_op.sem_num = 0;
	s_op.sem_flg = 0;

	printf("Requesting lock for critical section...\n");

	// Lock the critical section
	s_op.sem_op = -1;
	if (semop(sem_id, &s_op, 1) == -1) {
		perror("Error while locking semaphore");
		exit(1);
	}

	printf("Entered critical section!\n");
	
	// Critical section: write to shared memory
	printf("Write to shared memory: ");
	scanf(" %[^\n]", shm_ptr);
	getchar();
	printf("Data written to shared memory: %s\n", shm_ptr);

	printf("Exiting critical section...\n");

	// Unlock the critical section
	s_op.sem_op = 1;
	if (semop(sem_id, &s_op, 1) == -1) {
		perror("Error while unlocking semaphore");
		exit(1);
	}
}

/*
========================================================================================================
Output:

========================================================================================================
*/
