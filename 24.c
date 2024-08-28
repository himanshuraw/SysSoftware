#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();
	if(!pid){
		printf("Parent PID of child (Before PID): %d\n",getppid());
		sleep(3);
		printf("Parent PID of child (After PID): %d\n",getppid());
	}else{
		sleep(1);
	}
}
