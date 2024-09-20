/*
========================================================================================================
Name : 32c
Author : Himanshu Rawat
Description : Write a program to implement semaphore to protect any critical section.
		c. Protect multiple pseudo resources (may be two) using counting semaphore.
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
	key_t sem_key;
	int sem_id;
	sem_union sem_set;

	sem_key = ftok(".", 333);
	sem_id = semget(sem_key, 1, 0);
	if (sem_id == -1) {
		sem_id = semget(sem_key, 1, IPC_CREAT | 0700);
		sem_set.val = 2;
		semctl(sem_id, 0, SETVAL, sem_set);
	}

	printf("Entering to obtain lock on the critical section\n");

	if (lock_sem(sem_id) == -1) {
		return 1;
	}

	printf("Obtained lock on critical section!\n");

	printf("Inside critical section");
	getchar();
	int data;
	int fd = open("./tmp/ticket.txt", O_RDWR | O_CREAT, S_IRWXU);

	int n = read(fd, &data, sizeof(data));
	if (n == 0) {
		data = 1;
	} else {
		data += 1;
		lseek(fd, 0, SEEK_SET);
	}
	write(fd, &data, sizeof(data));
	printf("Ticket number is: %d\n", data);
	printf("Ticket number is stored in file\n");

	printf("Exiting critical section\n");

	if (unlock_sem(sem_id) == -1) {
		return 1;
	}
}

/*
========================================================================================================
Output:

Terminal 1: 
./a.out
Entering to obtain lock on the critical section
Obtained lock on critical section!
Inside critical section
Ticket number is: 1
Ticket number is stored in file
Exiting critical section


Terminal 2:
./a.out
Entering to obtain lock on the critical section
Obtained lock on critical section!
Inside critical section
Ticket number is: 2
Ticket number is stored in file
Exiting critical section

Terminal 3:
./a.out
Entering to obtain lock on the critical section


========================================================================================================
*/
