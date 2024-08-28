#include<stdio.h>
#include<unistd.h>

int main(){
	int pid = fork();

	if(!pid){
		printf("Child Process [PID: %d]", getpid());
	}
	else{
		printf("Press a key to escape\n");
		getchar();

	}
}
