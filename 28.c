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
