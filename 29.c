#include<stdio.h>
#include<sched.h>
#include<unistd.h>

int main(){
	int pid = getpid();

	int scheduling_policy = sched_getscheduler(pid);
	if(scheduling_policy == -1){
		perror("Error in getting scheduling policy");
		return -1;
	}
	switch(scheduling_policy){
		case SCHED_FIFO:
			printf("SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf("SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf("SCHED_OTHER\n");
			break;
		default:
			break;
	}
	
	printf("Select the number to change the policy:\n");
	printf("1: SCHED_FIFO\n");
	printf("2: SCHED_RR\n");
	printf("3: SCHED_OTHER\n");
	
	int choice;
	scanf("%d", &choice);
	struct sched_param s;

	int value;
	switch(choice){
		case 1:
			s.sched_priority = 99;
			value = sched_setscheduler(pid, SCHED_FIFO, &s);
			if(value == -1){
				perror("Error while setting policy");
				return -1;
			}
			break;
		case 2:
                        s.sched_priority = 99;
                        value = sched_setscheduler(pid, SCHED_RR, &s);
			if(value == -1){
                                perror("Error while setting policy");
                                return -1;
                        }
                        break;
		case 3:
                        s.sched_priority = 0;
                        value = sched_setscheduler(pid, SCHED_OTHER, &s);
                        if(value == -1){
                                perror("Error while setting policy");
                                return -1;
                        }
                        break;
		default:
			perror("Invalid choice, Pick a number from 1 to 3 ");
			break;
	}

	int npid = getpid();

        scheduling_policy = sched_getscheduler(npid);
        if(scheduling_policy == -1){
                perror("Error in getting scheduling policy");
                return -1;
        }
        switch(scheduling_policy){
                case SCHED_FIFO:
                        printf("SCHED_FIFO\n");
                        break;
                case SCHED_RR:
                        printf("SCHED_RR\n");
                        break;
                case SCHED_OTHER:
                        printf("SCHED_OTHER\n");
                        break;
                default:
                        break;
        }
}
