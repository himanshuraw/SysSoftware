/*
========================================================================================================

Name: 13
Author: Himanshu Rawat
Description: Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 21st August, 2024

========================================================================================================
*/

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
/*
========================================================================================================
Output:

./a.out
Timer started. Type within 10 seconds
TIME OUT!!

========================================================================================================
*/

