#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr<<"Enter more arguments!!";
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	int newFd;
	string fn = string(argv[2]);
	if(fn == "1") newFd = dup(fd);
	if(fn == "2") newFd = dup2(fd, 5);
	if(fn == "3") newFd = fcntl(fd, F_DUPFD);

	if(newFd == -1){
		cerr<<"Error in openning the file."<<endl;
		close(fd);
		return -1;
	}

	char buffer[] = "This is from buffer";
	int sz = strlen(buffer);
	int cnt = write(fd, buffer, sz);
	if(cnt == -1){
		cerr<<"Can't write using old FD.";
		close(fd);
		close(newFd);
		return -1;
	}
		
	cnt = write(newFd, buffer, sz);
	if(cnt == -1){
		cerr<<"Can't write using new FD.";
		close(fd);
		close(newFd);
		return -1;
	}

	string v = ( fn == "1" ) ? "dup()" : ( fn == "2" ) ? "dup2()" : "fcntl()";
	cout<< "Finished using: " << v;
}
