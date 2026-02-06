#include <windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

//int main(int argc, char* argv[])
//{
//	if (argc < 2)
//	{
//		std::cout << "Command line args error!\n";
//		return 1;
//	}
//	std::ifstream input(argv[1]);
//	if (!input)
//	{
//		std::cout << "Can't open file!\n";
//		return 1;
//	}
//	std::vector< std::string > programPath;
//	std::string str;
//	while (std::getline(input, str))
//	{
//		programPath.push_back(str);
//	}
//
//	STARTUPINFO si;
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//
//	PROCESS_INFORMATION pi;
//
//	for (auto & path : programPath)
//	{
//		if (!CreateProcess(NULL, LPWSTR((std::wstring(path.begin(), path.end()).c_str())), NULL, NULL, FALSE, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
//		{
//			std::cout << "Can't create procces: " << GetLastError() << '\n';
//			return 1;
//		}
//	}
//	CloseHandle(pi.hProcess);
//	CloseHandle(pi.hThread);
//	return 0;
//}