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
