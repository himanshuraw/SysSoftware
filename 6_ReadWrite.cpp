#include<iostream>
#include<unistd.h>
#include<cstring>

int main(){
	char data[20];
	int rd = read(0, data, 20);
	if(rd == -1) std::cout<<"Error occured while reading.";
	int wr = write(1, data, strlen(data));
	if(wr == -1) std::cout<<"Error occured while writing.";
}
