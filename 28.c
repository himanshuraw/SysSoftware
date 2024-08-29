/*
========================================================================================================

Name: 28
Author: Himanshu Rawat
Description: Write a program to get maximum and minimum real time priority.
Date: 29th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<sched.h>

int main(){
	int mxPriorityRR = sched_get_priority_max(SCHED_RR);
	int minPriorityRR = sched_get_priority_min(SCHED_RR);
	int mxPriorityFIFO = sched_get_priority_max(SCHED_FIFO);
	int minPriorityFIFO = sched_get_priority_min(SCHED_FIFO);

	printf("Max Priority (Round Robin): %d\n", mxPriorityRR);
	printf("Min Priority (Round Robin): %d\n", minPriorityRR);
	printf("Max Priority (FIFO): %d\n", mxPriorityFIFO);
	printf("Min Priority (FIFO): %d\n", minPriorityFIFO);
}

/*
========================================================================================================
Output:

./a.out

Max Priority (Round Robin): 99
Min Priority (Round Robin): 1
Max Priority (FIFO): 99
Min Priority (FIFO): 1

========================================================================================================
 */

