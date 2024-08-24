#include<iostream>
#include<sys/stat.h>
#include<ctime>
using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cout<<"input 1 argument \n";
		return -1;
	}
	struct stat buffer;
	stat(argv[1], &buffer);
	cout<<"inode: "<< buffer.st_ino<< endl;
	cout<<"number of hard link: "<< buffer.st_nlink << endl;
	cout<<"uid: "<< buffer.st_uid << endl;
	cout<<"gid: "<< buffer.st_gid << endl;
	cout<<"size: "<< buffer.st_size << endl;
	cout<<"block: "<< buffer.st_blksize << endl;
	cout<<"number of blocks: "<< buffer.st_blocks <<endl;
	cout<<"time of last acess: "<< ctime(&buffer.st_atime);
	cout<<"time of last modification: "<< ctime(&buffer.st_mtime);
	cout<<"time of last change: "<< ctime(&buffer.st_ctime);
}
