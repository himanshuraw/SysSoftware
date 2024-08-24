#include<iostream>
#include<unistd.h>
#include<fcntl.h>
using namespace std;

int main(int argc, char** argv){
	
	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		cout<<"Error in opening file: " << argv[1];
		return -1;
	}

	int fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(fd2 == -1) {
		cout<<"Error in opening file: " << argv[2];
		return -1;
	}

	char buff;
	while(true){
		int readCount = read(fd1, &buff, 1);
		if(readCount == -1){
			cout<<"Error in reading the file";
			return -1;
		}
		if(readCount == 0) break;
		int writeCount = write(fd2, &buff, 1);
		if(writeCount == -1){
			cout<<"Error in writing the file";
			return -1;
		}
	}
	cout<< argv[1] << " - copied to -> " << argv[2];
	close(fd1);
	close(fd2);
}
