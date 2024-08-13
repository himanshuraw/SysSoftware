#include<iostream>
#include<stdlib.h>
#include<unistd.h>

using namespace std;
int main(int argc, char** argv){
	if(argc < 3){
		cerr<< "Invalid argument count";
		return -1;
	}
	int flag = symlink(argv[1], argv[2]);
	if(flag == 0) cout<<"Soft Link has been created";
	else cout<<"Operation failed";
}
