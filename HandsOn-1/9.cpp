/*
========================================================================================================

Name: 9
Author: Himanshu Rawat
Description: Write a program to print the following information about a given file.
		a. inode
		b. number of hard links
		c. uid
		d. gid
		e. size
		f. block size
		g. number of blocks
		h. time of last access
		i. time of last modification
		j. time of last change
Date: 13th August, 2024

========================================================================================================
*/

#include<iostream>
#include<sys/stat.h>
#include<ctime>
using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cout<<"input more argument \n";
		return -1;
	}
	struct stat buffer;
	stat(argv[1], &buffer);
	cout<<"inode: "<< buffer.st_ino<< endl;
	cout<<"number of hard link: "<< buffer.st_nlink << endl;
	cout<<"uid: "<< buffer.st_uid << endl;
	cout<<"gid: "<< buffer.st_gid << endl;
	cout<<"size: "<< buffer.st_size << endl;
	cout<<"block: "<< buffer.st_blksize << endl;
	cout<<"number of blocks: "<< buffer.st_blocks <<endl;
	cout<<"time of last acess: "<< ctime(&buffer.st_atime);
	cout<<"time of last modification: "<< ctime(&buffer.st_mtime);
	cout<<"time of last change: "<< ctime(&buffer.st_ctime);
}

/*
========================================================================================================
Output:

./a.out data.txt
inode: 4624657
number of hard link: 1
uid: 1000
gid: 1000
size: 122
block: 4096
number of blocks: 8
time of last acess: Fri Aug 30 00:18:20 2024
time of last modification: Fri Aug 30 00:18:20 2024
time of last change: Fri Aug 30 00:18:20 2024

========================================================================================================
*/

