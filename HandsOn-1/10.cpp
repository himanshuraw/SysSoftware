/*
========================================================================================================

Name: 10
Author: Himanshu Rawat
Description: Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
		a. check the return value of lseek
		b. open the file with od and check the empty spaces in between the data.
Date: 13th August, 2024

========================================================================================================
*/

#include<iostream>
#include<unistd.h>
#include<fcntl.h>

using namespace std;

int main(int argc, char** argv){
	
	if(argc < 2){
		cerr << "Please input appropriate arguments";
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_RDWR);
	char buff[] = "0123456789";

	int count = write(fd, buff, 10);
	if(count == -1){
		cerr << "Error in writing `I`";
	}

	auto val = lseek(fd, 10, SEEK_CUR);
	cout<<"lseek returned : "<< val<<endl;

	count = write(fd, buff, 10);
	if(count == -1){
		cerr << "Error in writing `I`";
	}

}

/*
========================================================================================================
Output:

./a.out f.txt
lseek returned : 20

========================================================================================================
*/

