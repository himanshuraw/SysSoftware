#include<iostream>

using namespace std;

int main(){
	extern char **environ;
	while(*environ){
		cout<<*environ<<endl;
		*environ++;
	}
}
