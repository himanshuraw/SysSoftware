/*
========================================================================================================
Name : 6
Author : Himanshu Rawat
Description : Write a simple program to create three threads.
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* function(void* id){
	printf("Thread %d is running\n", *((int*)id));
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[3];
	int thread_ids[3];
	int i;

	for(i = 0; i<3; i++){
		thread_ids[i] = i+1;
		if(pthread_create(&threads[i], NULL, function, (void*)&thread_ids[i])){
			printf("Error in creating thread %d\n", i+1);
			exit(1);
		}
	}

	for(i = 0; i<3; i++){
		pthread_join(threads[i], NULL);
	}

	printf("All thread completed\n");
}

/*
========================================================================================================
Output:

./a.out
Thread 1 is running
Thread 2 is running
Thread 3 is running
All thread completed

========================================================================================================
*/
