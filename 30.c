/*
========================================================================================================

Name: 30
Author: Himanshu Rawat
Description: Write a program to run a script at a specific time using a Daemon process.
Date: 29th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char** argv){
	if(argc < 4){
		perror("Input more arguments in the form of \"HH MM SS\"");
		return -1;
	}
	time_t cur, end;
	struct tm *endtime;

	time(&cur);
	endtime = localtime(&cur);

	endtime->tm_hour = atoi(argv[1]);
	endtime->tm_min = atoi(argv[2]);
	endtime->tm_sec = atoi(argv[3]);

	//endtime->tm_sec += 5;
	end = mktime(endtime);

	int pid = fork();
	if(pid == 0){
		setsid();
		do{
			time(&cur);
		}while(difftime(end, cur) > 0);
		system("echo Wake Up");
	}
	else{
		exit(0);
	}
}

/*
========================================================================================================

Output:
./a.out 19 6 50
Wake Up

========================================================================================================
 */
