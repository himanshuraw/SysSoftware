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
	int flag = fcntl(fd, F_SETLKW, rdlock);
	
	cout<<"Enter a character to release the lock"<<endl;
	getchar();

	cout<<"Releasing the lock"<<endl;
	flag = fcntl(fd, F_SETLK, rdlock);
	
	close(fd);
}
