#include<iostream>
#include<fcntl.h>
using namespace std;
int main(int argc, char** argv){
	if(argc < 2){
		cerr<<"Give more arguments";
		return -1;
	}
	int fd = creat(argv[1], 0644);
	if(fd == -1) cout<< "File can't be created";
	else cout<<"File created successfully: "<<fd;
}
