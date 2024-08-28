#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

struct Record{
	int data;
};

int main(int argc, char** argv){
	if(argc<2){
		perror("Input more arguments");
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_WRONLY, 0777);
	if(fd == -1){
		perror("Error in opening File");
		return -1;
	}

	struct Record records[3];
	
	for(int i = 0; i<3; i++){
		records[i].data = 0;
	}

	int count = write(fd, &records, sizeof(records));
        if(count == -1){
                perror("Error in writing");
                close(fd);
                return -1;
        }
	printf("Record Initialized \n");
}
