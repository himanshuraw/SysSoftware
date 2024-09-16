/*
========================================================================================================
Name : 2
Author : Himanshu Rawat
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/resource.h>

struct Resource {
	int macro;
	const char *name;
};

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
	struct Resource res[] = {
		{ RLIMIT_CPU, "CPU time" }, { RLIMIT_FSIZE, "File size" },
		{ RLIMIT_DATA, "Data seg size" }, { RLIMIT_STACK, "Stack size" },
	       	{ RLIMIT_CORE, "Core file size" }, { RLIMIT_RSS, "Resident set size" },
		{ RLIMIT_NPROC, "Processes" }, { RLIMIT_NOFILE, "Open files" },
		{ RLIMIT_MEMLOCK, "Locked memory" }, { RLIMIT_AS, "Address space" }
	};

	int n = sizeof(res) / sizeof(res[0]);
	for(int i = 0; i < n; i++){
		print_limits(res[i].macro, res[i].name);
	}
}

/*
========================================================================================================
Output:

./a.out
CPU time :
Soft Limit: Unlimited
Hard Limit: Unlimited

File size :
Soft Limit: Unlimited
Hard Limit: Unlimited

Data seg size :
Soft Limit: Unlimited
Hard Limit: Unlimited

Stack size :
Soft Limit: 8388608
Hard Limit: Unlimited

Core file size :
Soft Limit: 0
Hard Limit: Unlimited

========================================================================================================
*/
