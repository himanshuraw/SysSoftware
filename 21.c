#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();
	if(pid == -1){
		perror("Error in for");
		return -1;
	}
	if(pid == 0){
		printf("This is child process\n- child PID : %d\n- parent PID %d\n\n", getpid(), getppid());
	}	
	printf("This is parent process\n- child PID : %d\n- parent PID %d\n\n", pid, getpid());
}
