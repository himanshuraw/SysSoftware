/*
========================================================================================================
Name : 1.c
Author : Himanshu Rawat
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second
                a. ITIMER_PROF
Date: 12 Sept, 2024.
========================================================================================================
*/

#include<stdio.h>
#include<sys/time.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>


void alarmHandler(){
        printf("Timer expired\n");
        exit(0);
}

int main(){
        signal(SIGPROF, alarmHandler);

        struct timeval time;
        time.tv_sec = 10;
        time.tv_usec = 10;

        struct itimerval timer;
        timer.it_interval = time;
        timer.it_value = time;

        int flag = setitimer(ITIMER_PROF, &timer, NULL);
        if(flag == -1){
                perror("Unable to set Timer");
                return -1;
        }
        while(1){}
}

/*
========================================================================================================
Output:
./a.out
Timer expired

==============================================================================================
==========

*/
