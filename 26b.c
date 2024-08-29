#include<stdio.h>
#include<unistd.h>

int main(int argc, char** argv){
	execl("/home/himanshu-rawat/Documents/SysSoftware/a.out","./a.out", argv[1],  NULL); 
}

