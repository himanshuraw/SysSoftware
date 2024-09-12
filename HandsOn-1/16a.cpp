/*
========================================================================================================

Name: 16a
Author: Himanshu Rawat
Description: Write a program to perform mandatory locking.
		a. Implement write lock
Date: 24th August, 2024

========================================================================================================
*/

#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(int argc,char** argv){
	if(argc < 2){
		cerr<<"Provide more arguments"<<endl;
		return -1;
	}

	struct flock wrlock;
	wrlock.l_type = F_WRLCK;

	int fd = open(argv[1], O_WRONLY);

	cout<<"Trying to aquire lock"<<endl;
	int flag = fcntl(fd, F_SETLKW, &wrlock);
	if(flag == -1){
		cout<<"Error occurred"<<endl;
		close(fd);
		return -1;
	}
	cout<<"Acquired Write lock"<<endl
		<<"Enter a character to release the lock"<<endl;
	getchar();

	cout<<"Releasing the lock"<<endl;
	wrlock.l_type = F_UNLCK;
	flag = fcntl(fd, F_SETLK, &wrlock);
	if(flag == -1){
                cout<<"Error occurred"<<endl;
                close(fd);
                return -1;
        }
	cout<<"Done";
	close(fd);
}

/*
========================================================================================================
Output:

Terminal 1:
./a.out info.txt
Trying to aquire lock
Acquired Write lock
Enter a character to release the lock

Releasing the lock
Done

Terminal 2:
./a.out info.txt
Trying to aquire lock

========================================================================================================
*/

