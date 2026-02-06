#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <ctime>

//volatile bool runFlag = true;
//
////DWORD WINAPI ThreadProc(LPVOID lpParameter)
////{
////	int param = *(int *)lpParameter;
////	std::cout << "I am ThreadProc = " << param << " Hello!\n";
////	while (runFlag)
////	{
////		std::cout << "ThreadProc = " << param << " in while!\n";
////		Sleep(100);
////	}
////	std::cout << "ThreadProc = " << param << " die!\n";
////	return 0;
////}
////
////int main(int argc, char* argv[])
////{
////	runFlag = true;
////	std::cout << "I am main! Hello!\n";
////
////	if (argc < 3)
////	{
////		std::cout << "Uncorrect number of comand line agrs!\n";
////		return 1;
////	}
////	int threadNumber = atoi(argv[1]);
////	std::vector< DWORD > mArg(threadNumber), mId(threadNumber);
////
////	for (int i = 0; i < threadNumber; ++i)
////	{
////		mArg[i] = i;
////		CreateThread(NULL, 0, ThreadProc, &(mArg[i]), 0, &(mId[i]));
////	}
////	
////	std::time_t startTime = std::time(nullptr);
////	while ((std::time(nullptr) - startTime) < atoi(argv[2]))
////	{
////		Sleep(1000);
////	}
////	std::cout << "Set runFlag to false, wait 5 seconds\n";
////	runFlag = false;
////	Sleep(5000);
////	return 0;
////}