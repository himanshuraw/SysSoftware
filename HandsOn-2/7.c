/*
========================================================================================================
Name : 7
Author : Himanshu Rawat
Description : Write a simple program to print the created thread ids.
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* function(void* num){
	printf("Thread %d: Thread ID: %lu\n", *((int*)num), pthread_self());
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[3];
	int thread_num[3];
	int i;

	for(i = 0; i<3; i++){
		thread_num[i] = i+1;
		if(pthread_create(&threads[i], NULL, function, (void*)&thread_num[i])){
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
Thread 1: Thread ID: 140524669368000
Thread 2: Thread ID: 140524660975296
Thread 3: Thread ID: 140524652582592
All thread completed

========================================================================================================
*/

