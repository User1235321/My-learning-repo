#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <ctime>

volatile bool runFlag = true;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	int param = *(int*)lpParameter;
	while (runFlag)
	{
		std::cout << "Thread " << param << " have " << GetThreadPriority(GetCurrentThread()) << " priority\n";
		Sleep(100);
	}
	std::cout << "Thread " << param << " die!\n";
	return 0;
}

int main(int argc, char* argv[])
{
	runFlag = true;

	DWORD currentPriorityClass = GetPriorityClass(GetCurrentProcess());
	std::cout << "Start main priority: " << currentPriorityClass << '\n';
	if (SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
		std::cout << "Priority class of main changed to HIGH_PRIORITY_CLASS\n";
	}
	SetProcessPriorityBoost(GetCurrentProcess(), true);
	std::cout << "Disable dinamic priority change\n";

	if (argc < 3)
	{
		std::cout << "Uncorrect number of comand line agrs!\n";
		return 1;
	}
	int threadNumber = atoi(argv[1]);
	std::vector< DWORD > mArg(threadNumber), mId(threadNumber);
	std::vector< HANDLE > threads;
	for (int i = 0; i < threadNumber; ++i)
	{
		mArg[i] = i;
		threads.push_back(CreateThread(NULL, 0, ThreadProc, &(mArg[i]), 0, &(mId[i])));
		if ((i % 3) == 0)
		{
			SetThreadPriority(threads[i], THREAD_PRIORITY_HIGHEST);
		}
		else if ((i % 2) == 0)
		{
			SetThreadPriority(threads[i], THREAD_PRIORITY_NORMAL);
		}
		else
		{
			SetThreadPriority(threads[i], THREAD_PRIORITY_LOWEST);
		}
	}

	std::time_t startTime = std::time(nullptr);
	while ((std::time(nullptr) - startTime) < atoi(argv[2]))
	{
		Sleep(1000);
	}
	std::cout << "Set runFlag to false, wait 5 seconds\n";
	runFlag = false;
	Sleep(5000);

	SetPriorityClass(GetCurrentProcess(), currentPriorityClass);
	return 0;
}