#include<stdio.h>
#include<unistd.h>
#include<time.h>

int main(){
        struct timespec start, end;

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        int getpid();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

        double timeTaken =
                (end.tv_sec - start.tv_sec)*1000000000 
                + (end.tv_nsec - start.tv_nsec);
        
        printf("Time taken for execution: %f nanoseconds\n",timeTaken);
}

