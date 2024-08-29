#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
	int pid1 = fork();
	if(!pid1){
		sleep(2);
		printf("First child\n");
	}
	else{
		int pid2 = fork();
		if(!pid2){
			sleep(3);
			printf("Second child\n");
		}
		else{
			int pid3 = fork();
			if(!pid3){
				sleep(4);
				printf("Third child\n");
			}
			else{
				printf("Waiting for 2nd Child\n");
				waitpid(pid2, NULL, 0);
			}
		}
	}
}
