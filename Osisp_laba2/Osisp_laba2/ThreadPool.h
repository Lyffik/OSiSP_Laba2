#pragma once

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <wchar.h>
#include <sys/types.h>
#include <Wincrypt.h>
#include <queue>
using namespace std;

class ThreadPool
{
	public:
		ThreadPool();
		~ThreadPool();
		queue<Task*> List;
		ThreadPool(int NumberOfThread);

	private:
		int Number;
		bool Active;
		HANDLE *ThreadHandels;
		static DWORD WINAPI ThreadProc(CONST LPVOID lpParam);

};

