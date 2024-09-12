/*
========================================================================================================

Name: 14a and 14b
Author: Himanshu Rawat
Description: Write a program to find the type of a file.
		a. Input should be taken from command line.
		b. program should be able to identify any type of a file.
Date: 22th August, 2024

========================================================================================================
*/

#include<iostream>
#include<sys/stat.h>
using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr<< "Less number of arguments"<<endl;
		return -1;
	}

	struct stat s;

	if(lstat(argv[1], &s) == -1){
		cerr<< "Error using lstat"<<endl;
		return -1;
	}

	cout<<"File type: ";

	switch(s.st_mode & S_IFMT){
		case S_IFBLK: cout<<"Block Device"<<endl; break;    
		case S_IFCHR: cout<<"Character Device"<<endl; break;
		case S_IFDIR: cout<<"Directory"<<endl; break;
		case S_IFIFO: cout<<"FIFO/Pipe"<<endl; break;
		case S_IFLNK: cout<<"Symbolic Link"<<endl; break;
		case S_IFREG: cout<<"Regular File"<<endl; break;
		case S_IFSOCK: cout<<"Socket"<<endl; break;
		default: cout<<"Unknown"<<endl; break;
	}
}

/*
========================================================================================================
Output:

./a.out 10.cpp
File type: Regular File

========================================================================================================
*/

