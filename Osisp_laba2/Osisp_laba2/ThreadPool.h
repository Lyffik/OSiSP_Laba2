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
#include <vector>
#include "Task.h"


using namespace std;

class ThreadPool
{
	public:

		ThreadPool(int threadCount);
		~ThreadPool();
		void AddThread();
		void AddTask(Task* task);
		DWORD WINAPI ThreadProc(CONST LPVOID lpParam);
		void ThreadProc();

	private:

		bool active;
		int threadsCount;
		FILE * logFile;
		int activeThreadsCount;
		queue<Task*> taskQueue;
		vector<HANDLE> threadHandels;
		CRITICAL_SECTION criticalSectionForThread, criticalSectionForFile,criticalSectionForTerminal;
		HANDLE semaphoreThread, semaphoreTask;

};

