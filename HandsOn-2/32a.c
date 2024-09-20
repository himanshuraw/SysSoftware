/*
========================================================================================================
Name : 32a
Author : Himanshu Rawat
Description : Write a program to implement semaphore to protect any critical section.
		a. Rewrite the ticket number creation program using semaphore.
Date: 20 Sept, 2024.
========================================================================================================
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef union {
	int val;
	struct semid_ds *buf;
	unsigned short int *arr;
	struct seminfo *__buf;
} sem_union;

int main() {
	sem_union sem;
	int k = ftok(".", 331);
	int sid = semget(k, 1, 0);

	if (sid == -1) {
		sid = semget(k, 1, IPC_CREAT | 0700);
		sem.val = 1;
		semctl(sid, 0, SETVAL, sem);
	}

	struct sembuf s_op;
	s_op.sem_num = 0;
	s_op.sem_flg = 0;

	printf("Entering critical section\n");

	// Lock the critical section
	s_op.sem_op = -1;
	semop(sid, &s_op, 1);

	printf("Inside critical section\n");
	getchar();

	int ticket_number;
	int file_descriptor = open("./tmp/ticket.txt", O_RDWR | O_CREAT, S_IRWXU);
	if (file_descriptor == -1) {
		perror("Error while opening file!");
		return 0;
	}

	int bytes_read = read(file_descriptor, &ticket_number, sizeof(ticket_number));
	ticket_number = (bytes_read == 0) ? 1 : ticket_number + 1;
	lseek(file_descriptor, 0, SEEK_SET);
	write(file_descriptor, &ticket_number, sizeof(ticket_number));
	printf("Ticket number is: %d\n", ticket_number);

	printf("Exiting critical section\n");

	// Unlock the critical section
	s_op.sem_op = 1;
	semop(sid, &s_op, 1);

}


/*
========================================================================================================
Output:

Terminal 1:
```````````
./a.out
Entering critical section
Inside critical section

Ticket number is: 1
Exiting critical section

Terminal 2:
```````````
./a.out
Entering critical section
Inside critical section

Ticket number is: 2
Exiting critical section
himanshu-rawat@himanshu-ra

========================================================================================================
*/
