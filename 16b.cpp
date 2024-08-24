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
