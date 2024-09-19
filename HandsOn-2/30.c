/*
========================================================================================================
Name : 30
Author : Himanshu Rawat
Description : Write a program to create a shared memory.
		a. write some data to the shared memory
		b. attach with O_RDONLY and check whether you are able to overwrite.
		c. detach the shared memory
		d. remove the shared memory
Date: 19 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

int main() {
	key_t k = ftok(".", 1);
	int shmid = shmget(k, 1024, 0666 | IPC_CREAT);
	char *data = (char*) shmat(shmid, NULL, 0);

	strcpy(data, "Writing to shared memory.");
	printf("Data written to shared memory: %s\n", data);

	shmdt(data);

	//with RDONLY
	data = (char*) shmat(shmid, NULL, SHM_RDONLY);
	printf("Attached with O_RDONLY. Data: %s\n", data);

	printf("Trying to overwrite in read-only mode...\n");
	strcpy(data, "New data"); // Should not work in read-only mode
	printf("Data after attempting to overwrite: %s\n", data);

	//detachs shared memory from shm add (data)
	shmdt(data);

	//Remove shared memory
	shmctl(shmid, IPC_RMID, NULL);
	printf("Shared memory removed.\n");
}

/*
========================================================================================================
Output:

========================================================================================================
*/
