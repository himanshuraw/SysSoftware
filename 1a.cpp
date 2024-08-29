/*
========================================================================================================

Name: 1a
Author: Himanshu Rawat
Description: Create the following types of a files using (i) shell command (ii) system call
	a. soft link (symlink system call)
Date: 10th August, 2024

========================================================================================================
*/

#include<iostream>
#include<stdlib.h>
#include<unistd.h>

using namespace std;
int main(int argc, char** argv){
	if(argc < 3){
		cerr<< "Invalid argument count";
		return -1;
	}
	int flag = symlink(argv[1], argv[2]);
	if(flag == 0) cout<<"Soft Link has been created";
	else cout<<"Operation failed";
}

/*
========================================================================================================
Output:

./a.out data.txt slink
Soft Link has been created
========================================================================================================
*/

