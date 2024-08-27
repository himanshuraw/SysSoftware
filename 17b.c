#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

struct Ticket{
	int no;
};

int main(int argc, char** argv){
	if(argc < 2){
		perror("Input more arguments");
		return -1;
	}

	int fd = open(argv[1], O_RDWR);
	if(fd == -1){
		perror("Error in opening file");
		return -1;
	}

	struct Ticket ticket;
	int count = read(fd, &ticket, sizeof(ticket));
        if(count == -1){
                printf("Error in reading file");
                close(fd);
                return -1;
        }
	int ticket_no = ticket.no;
	printf("Ticket number (Before): %d\n", ticket_no);

	struct flock lock;
	lock.l_type = F_WRLCK;

	printf("Waiting...\n");
	int flag = fcntl(fd, F_SETLKW, &lock);

	lseek(fd, 0, SEEK_SET);	
	count = read(fd, &ticket, sizeof(ticket));
	if(count == -1){
		printf("Error in reading file");
		close(fd);
		return -1;
	}

	ticket_no = ++ticket.no;
	printf("Ticket number (After): %d\n", ticket_no);
	lseek(fd, 0, SEEK_SET); 
	count = write(fd, &ticket, sizeof(ticket));
	if(count == -1){
                printf("Error in writing file");
                close(fd);
                return -1;
        }

	printf("Press a key to finish\n");
	getchar();

	lock.l_type = F_UNLCK;
	flag = fcntl(fd, F_SETLK, &lock);
	if(flag == -1){
		printf("Error occurred\n");
		close(fd);
		return -1;
	}
	close(fd);
}
