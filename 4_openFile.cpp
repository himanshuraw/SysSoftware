#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

using namespace std;

int  main(int argc, char** argv){
	if(argc < 2){
		cerr<< "Input more arguments";
		return -1;
	}
	int fd = open(argv[1], O_CREAT, O_RDWR);
	if(fd == -1) {
		cout<< "Unable to open file"<< endl;
	}else {
		cout<< "Successfully opened in readwrite mode"<< endl;
	}

	int fd2 = open(argv[1] , O_CREAT|O_EXCL, O_RDWR);
	if(fd2 == -1) {
		cout<< "Unable to open the file in fd2"<< endl;
	}	
}
