#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

struct Ticket{
	int no;
};

int main(int argc, char** argv){
	if(argc < 2){
		perror("Provide more arguments");
		return -1;
	}

	int fd = open(argv[1], O_CREAT | O_WRONLY, 0777 );
	if(fd == -1){
		perror("Error in Opening/Creating the file");
		return -1;
	}

	struct Ticket ticket;
	ticket.no = 0;

	int count = write(fd, &ticket, sizeof(ticket));

	if(count == -1){
		perror("Error in writing to the file");
		return -1;
	}

	printf("Ticket created successfully!!!");
	close(fd);
}
