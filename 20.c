#include<stdio.h>
#include<unistd.h>
#include<sys/resource.h>

int main(){
	int priority = getpriority(PRIO_PROCESS, 0);

	if(priority == -1){
		perror("Error in getting priority");
		return -1;
	}
	printf("Priority of currect process is : %d\n", priority);

	priority =  nice(1);

	if(priority == -1){
                perror("Error using nice");
                return -1;
        }
        printf("Priority of currect process after using nice is : %d\n", priority);
}
