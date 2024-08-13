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
