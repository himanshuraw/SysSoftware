#include<stdio.h>
#include<unistd.h>

int main(){
	char* args[] = {"ls", "-Rl", NULL};
	execv("/bin/ls", args); 
}

