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

