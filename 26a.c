/*
========================================================================================================

Name: 26a
Author: Himanshu Rawat
Description: Write a program to execute an executable program.
		a. use some executable program
Date: 29th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(){
	execl("/bin/pwd","pwd", NULL); 
}

/*
========================================================================================================
Output:

./b.out
/home/himanshu-rawat/Documents/SysSoftware

========================================================================================================
*/
