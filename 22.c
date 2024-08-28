#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char** argv){
	if(argc < 2){
		perror("Input more arguments");
		return -1;
	}
	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	if(fd == -1){
		perror("Error in opening File");
		return -1;
	}

	fork();
	char data[] = "data";

	int cnt = write(fd, &data, sizeof(data));
	if(cnt == -1){
		perror("Error in writing");
	}
	printf("Finished printing");
	close(fd);
}
