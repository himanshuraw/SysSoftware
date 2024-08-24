#include<iostream>
#include<sys/select.h>

using namespace std;

int main(){
	fd_set read;
	FD_ZERO(&read);
	FD_SET(0, &read);
	struct timeval time;
	time.tv_sec = 10;
	time.tv_usec = 0;
	cout<<"Timer started. Type within 10 seconds"<<endl;
	int res = select(1, &read, NULL, NULL, &time);
	if(res == 0) cerr<<"TIME OUT!!"<<endl;
	else if(res == -1) cerr<<"Error in select"<<endl;
	else cout<<"Done";
}
