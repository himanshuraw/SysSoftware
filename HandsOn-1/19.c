/*
========================================================================================================

Name: 19
Author: Himanshu Rawat
Description: Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 29th August, 2024

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
        int getpid();
        end = rdtsc();

        unsigned long long int timeTaken = (end - start)/2.4;
        
        printf("Time taken for execution: %lld nanoseconds\n",timeTaken);
}

/*
========================================================================================================
Output:

./a.out
Time taken for execution: 29 nanoseconds

========================================================================================================
*/

