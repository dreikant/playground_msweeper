// dev_load.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#include <tlhelp32.h>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <stdexcept>
using namespace std;

#include "dev_load_settings.h"

HANDLE dllinject(const char* program, const char* dll) {
	int i;
	DWORD pid;
	HANDLE h, t;
	LPVOID mem;
	PROCESSENTRY32 p;
	p.dwSize = sizeof(PROCESSENTRY32);
	
	//If all characters are numeric, program is a pid
	for (i = 0; program[i]; i++)
		if (!isdigit(program[i])) break;


	//If it's an imagename, find the first matching process
	if (i < strlen(program)) {
		if ((h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
			throw runtime_error("Failed to query processes");
		for (
		Process32First(h, &p);
		GetLastError() != ERROR_NO_MORE_FILES;
		Process32Next(h, &p))
			if (!strcmp(p.szExeFile, program)) {
				pid = p.th32ProcessID;
				break;
			}
		CloseHandle(h);
	} else pid = atoi(program);

	//Open process by pid
	if ((h = OpenProcess(
	PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
	PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION,
	false, pid)) == NULL)
		throw runtime_error("Failed to open process");
	
	//Allocate and write dll string in it
	mem = VirtualAllocEx(h, 0, strlen(dll) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (mem == NULL) { CloseHandle(h); throw runtime_error("Failed to allocate dll string"); }

	WriteProcessMemory(h, mem, dll, strlen(dll) + 1, NULL);
	
	//Load dll into process
	if ((
	t = CreateRemoteThread(h, NULL, NULL,
	(LPTHREAD_START_ROUTINE)LoadLibraryA,
	mem, NULL, NULL)) == NULL) { 
		CloseHandle(h);
		throw runtime_error("Failed to execute remote thread");
	}
	
	//Free string and close
	Sleep(100);
	VirtualFreeEx(h, mem, NULL, MEM_RELEASE);
	CloseHandle(h);
	return t;
}

string ExtractDirectory( const string& path )
{
  return path.substr( 0, path.find_last_of( '\\' ) +1 );
}
 
int _tmain(int argc, _TCHAR* argv[])
{
		if (argc != 3) {
			printf(
				"DLL Injector"
				"Usage: %s PROGRAM DLL\n",
				argv[0]
			);
		}

		char buffer1[5120];
		GetModuleFileNameA(NULL, buffer1, 5120);
		string dir = ExtractDirectory(string(buffer1));
		string shit = dir.append(dllname);

		try { dllinject(targetname, shit.c_str()); } catch (exception& ex) {
		printf("Error: %s\nError code: %d\n", ex.what(), GetLastError());
		return 1;
	}
	puts("Success");
	return 0;
	return 0;
}

