#include<stdio.h>
#include<unistd.h>

int main(){
	char* args[] = {"ls", "-Rl", NULL};
	execvp("ls", args); 
}

