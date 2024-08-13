#include<iostream>
#include<sys/stat.h>

using namespace std;

int main(int argc, char** argv){

	if(argc < 2){
		cerr<<"Invalid argument count";
		return -1;
	}
	int flag = mknod(argv[1], 0777, 0);
	if( flag == 0 ) cout<<"Done";
	else cout<<"Fail";
}
