#include<iostream>
#include<unistd.h>

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr<<"Enter correct numbers of arguments";
		return -1;
	}
	int flag = link (argv[1], argv[2]);
	if(flag == 0) std::cout<< "Hardlink is created";
	else std::cout<< "Operation failed";
}
