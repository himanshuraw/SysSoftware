#include<fcntl.h>
#include<string>

int main(){
	int fds[5];
	for(int i = 0; i<5; i++){
		std::string s = "file" + std::to_string(i+1);
		const char *name = s.c_str();
		fds[i] = creat(name, 0777); 
	}

	while(1){}
}
