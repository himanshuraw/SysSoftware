/*
========================================================================================================

Name: 5
Author: Himanshu Rawat
Description: Write a program to create five new files with infinite loop. Execute the program in the background
and check the file descriptor table at /proc/pid/fd.
Date: 13th August, 2024

========================================================================================================
*/


#include<fcntl.h>
#include<string>

int main(){
	int fds[5];
	for(int i = 0; i<5; i++){
		std::string s = "file" + std::to_string(i+1);
		const char *name = s.c_str();
		fds[i] = creat(name, 0777); 
	}

	while(1){}
}
