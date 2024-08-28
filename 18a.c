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

	struct Record record;

	printf("Which record you want to write in: ");
	int index;
	scanf("%d", &index);
	if(index > 3){
		printf("Enter record less than equal to 3");
		return -1;
	}
	printf("Waiting...\n");

	lseek(fd, (index-1)*sizeof(record), SEEK_SET);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(record);

	fcntl(fd, F_SETLKW, &lock);

	printf("Enter a number to write in the record: ");
	int number;
	scanf("%d", &number);

	record.data = number;
	int count = write(fd, &record, sizeof(record));
        if(count == -1){
                perror("Error in writing");
                close(fd);
                return -1;
        }
	printf("Record set correctly\n");

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
}
