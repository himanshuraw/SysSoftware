#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<cstring>
using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cout<< "Enter 2 arguments \n";
		return -1;
	}

	int input = open(argv[1], O_RDONLY);
	char buff[100];
	int cur = 0;

	while(true){
		int byteRead = read(input, &buff[cur], 1);
		if(byteRead == -1){
			std::cout<<"Can't read";
			break;
		}
	
		if(byteRead == 0) break;
	
		if(buff[cur] == '\n'){
			write(1, buff, cur);
			getchar();
			cur = 0;
			continue;
		}
		cur++;
	}
	close(input);
}
