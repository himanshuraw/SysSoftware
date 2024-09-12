/*
========================================================================================================

Name: 16b
Author: Himanshu Rawat
Description: Write a program to perform mandatory locking.
		b. Implement read lock
Date: 24th August, 2024

========================================================================================================
*/

#include<iostream>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(int argc, char** argv){
	
	if(argc < 2){
		cerr<<"Provide more arguments"<<endl;
		return -1;
	}
	
	struct flock rdlock;
	rdlock.l_type = F_RDLCK;	

	int fd = open(argv[1], O_RDONLY);

	cout<<"Trying to aquire the lock"<<endl;

	int flag = fcntl(fd, F_SETLKW, &rdlock);
	if(flag == -1){
		cout<<"Error occurred"<<endl;
		close(fd);
		return -1;
	}
	cout<<"Acquired Read lock"<<endl
		<<"Enter a character to release the lock"<<endl;
	getchar();

	cout<<"Releasing the lock"<<endl;
	rdlock.l_type = F_UNLCK;
	flag = fcntl(fd, F_SETLK, &rdlock);

	if(flag == -1){
                cout<<"Error occurred"<<endl;
                close(fd);
                return -1;
        }
	
	close(fd);
}

/*
========================================================================================================
Output:

Terminal 1:
./a.out info.txt
Trying to aquire the lock
Acquired Read lock
Enter a character to release the lock

Terminal 2:
./a.out info.txt
Trying to aquire lock

========================================================================================================
*/

