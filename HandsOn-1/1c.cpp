/*
========================================================================================================

Name: 1c
Author: Himanshu Rawat
Description: Create the following types of a files using (i) shell command (ii) system call
        c. FIFO (mkfifo Library Function or mknod system call)
Date: 10th August, 2024

========================================================================================================
*/

#include<iostream>
#include<sys/stat.h>

using namespace std;

int main(int argc, char** argv){

	if(argc < 2){
		cerr<<"Invalid argument count";
		return -1;
	}
	int flag = mknod(argv[1], 0777, 0);
	if( flag == 0 ) cout<<"Done";
	else cout<<"Fail";
}

/*
========================================================================================================
Output:

./a.out pipe
Done
========================================================================================================
*/

