/*
========================================================================================================
Name : 4
Author : Himanshu Rawat
Description : Write a program to measure how much time is taken to execute 100 getppid ( )
system call. Use time stamp counter.
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

unsigned long long rdtsc(){
	unsigned long long dst;
	__asm__ __volatile__("rdtsc":"=A" (dst));
}

int main(){
        unsigned long long int start, end;

        start = rdtsc();
	
	for(int i = 0; i < 100; i++){
       		getppid();
	}

        end = rdtsc();

        unsigned long long int timeTaken = (end - start)/2.4;
        
        printf("Time taken for execution: %lld nanoseconds\n",timeTaken);
}

/*
========================================================================================================
Output:

./a.out
Time taken for execution: 259111 nanoseconds

========================================================================================================
*/

