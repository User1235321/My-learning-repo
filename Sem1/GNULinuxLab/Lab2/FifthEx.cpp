#include<unistd.h>
#include<time.h>
#include<iostream>
#include<fstream>

int main(){
	std::fstream txtFile;
	txtFile.open("txtFile.txt");
	
	long long int processID = fork();
	char a = '0';
	switch(processID){
	case(-1):
	std::cout << "Error, please restart" << '\n';
	break;
	
	case(0):
	std::cout << "This is child"<< '\n';

	sleep(2);
	while(!txtFile.eof()){
		txtFile.get(a);
		std::cout<<a;
	}
	
	std::cout << "Child is dead"<< '\n';
	break;
	
	default:
	std::cout << "Parent is born with pid: " << processID << '\n';
	
	sleep(2);
	for(int i = 0; ((i < 50) && (!txtFile.eof())); ++i){
		txtFile.get(a);
		std::cout<<a;
	}
	while(!txtFile.eof()){
		txtFile.get(a);
		std::cout<<a;
	}
	
	std::cout << "Parent is dead"<< '\n';
	}
	return 0;
}


