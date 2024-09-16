/*
========================================================================================================
Name : 3
Author : Himanshu Rawat
Description : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 16 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/resource.h>


void print_limits(int macro, const char *name){
	struct rlimit limit;
	if(getrlimit(macro, &limit) == 0){
		printf("%s :\n", name);
		if(limit.rlim_cur == RLIM_INFINITY)
			printf("Soft Limit: Unlimited\n");
		else
			printf("Soft Limit: %lu\n", limit.rlim_cur); // max it can go to hard limit

		if(limit.rlim_max == RLIM_INFINITY)
			printf("Hard Limit: Unlimited\n");
		else
                        printf("Hard Limit: %lu\n", limit.rlim_max);
	}else{
		perror("Error in getting limits");
	}
	printf("\n");
}

int main(){
	printf("Before\n");
	print_limits(RLIMIT_CPU, "CPU time");

	struct rlimit limit;

	limit.rlim_cur = 10;
	limit.rlim_max = RLIM_INFINITY;

	if(setrlimit(RLIMIT_CPU, &limit) == 0)
		printf("CPU limit set to 10.\n\n");
	else
		perror("Error in setting the limit");

	printf("After\n");
	print_limits(RLIMIT_CPU, "CPU time");
}

/*
========================================================================================================
Output:

./a.out
Before
CPU time :
Soft Limit: Unlimited
Hard Limit: Unlimited

CPU limit set to 10.

After
CPU time :
Soft Limit: 10
Hard Limit: Unlimited

========================================================================================================
*/

