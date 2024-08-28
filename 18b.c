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

	int fd = open(argv[1], O_RDONLY, 0777);
	struct Record record;

	printf("Which record you want to read from: ");
	int index;
	scanf("%d", &index);
	printf("Waiting...\n");

	lseek(fd, (index-1)*sizeof(record), SEEK_SET);

	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(record);

	fcntl(fd, F_SETLKW, &lock);

	int count = read(fd, &record, sizeof(record));
	if(count == -1){
		perror("Error in reading");
		close(fd);
		return -1;
	}

	printf("Data in record: %d\n",record.data);
	printf("Press key to exit");
	getchar();
	getchar();

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
}
