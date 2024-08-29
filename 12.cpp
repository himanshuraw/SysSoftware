/*
========================================================================================================

Name: 12
Author: Himanshu Rawat
Description: Write a program to find out the opening mode of a file. Use fcntl.
Date: 14th August, 2024

========================================================================================================
*/

#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr<<"Need more arguments!!";
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_RDONLY);
	if(fd == -1){
		cerr<<"Error in opening the file.";
		return -1;
	}

	int mode = fcntl(fd, F_GETFL, 0);
	if(mode == -1){
		cerr<<"Error in getting the mode of the file.";
		close(fd);
		return -1;
	}

	int aMode = mode & O_ACCMODE;

	switch(aMode){
		case O_RDONLY:	cout<<"Read only.";
			      	break;
		case O_WRONLY:	cout<<"Write only.";
			     	break;
		case O_RDWR:	cout<<"Read write.";
				break;
		default:	cerr<<"Unknown";
				close(fd);
				return -1;
	}

	close(fd);
}

/*
========================================================================================================
Output:

./a.out data.txt 
Read only.

========================================================================================================
*/

