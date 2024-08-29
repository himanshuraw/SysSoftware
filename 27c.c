#include<stdio.h>
#include<unistd.h>

int main(){
	extern char **environ;
	execle("/bin/ls", "ls", "-Rl", NULL, environ); 
}

