#include<unistd.h>
#include<time.h>
#include<iostream>

int main(){

	long long int processID = fork();
	switch(processID){
	case(-1):
	std::cout << "Error, please restart" << '\n';
	break;
	
	case(0):
	std::cout << "Child is born"<< '\n';
	
	chdir("./..");
	sleep(60);
	
	std::cout << "Child is dead"<< '\n';
	break;
	
	default:
	std::cout << "Parent is born with pid: " << processID << '\n';
	
	sleep(60);
	
	std::cout << "Parent is dead"<< '\n';
	}
	return 0;
}


