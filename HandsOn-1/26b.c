/*
========================================================================================================

Name: 26b
Author: Himanshu Rawat
Description: Write a program to execute an executable program.
		b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 29th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char** argv){
	execl("/home/himanshu-rawat/Documents/SysSoftware/a.out","./a.out", argv[1],  NULL); 
}

/*
========================================================================================================
Output:

./b.out "from 26.c"
from 26.c
========================================================================================================
 * /
