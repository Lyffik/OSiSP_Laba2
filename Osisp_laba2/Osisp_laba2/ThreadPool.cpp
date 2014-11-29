#include "stdafx.h"
#include "ThreadPool.h"

#define FIXED_NUMBER 20

static DWORD WINAPI StaticThreadProc(LPVOID lParam)
{
	ThreadPool* pool = (ThreadPool*)lParam;
	pool->ThreadProc(lParam);
	return 0;
}

ThreadPool::ThreadPool(int count)
{
	threadsCount = count;
	active = true;
	#pragma warning(disable: 4996)
	logFile = fopen("log.txt", "wt");
	InitializeCriticalSection(&criticalSectionForThread);
	InitializeCriticalSection(&criticalSectionForFile);
	InitializeCriticalSection(&criticalSectionForTerminal);
	semaphoreThread = CreateSemaphore(NULL, count, 100, NULL);
	semaphoreTask = CreateSemaphore(NULL, 0, 100, NULL);
	for (int i = 0; i < count; i++)
	{
		threadHandels.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadProc, this, 0, NULL));
	}
}

ThreadPool::~ThreadPool()
{
	active = false;
	WaitForMultipleObjects(threadHandels.size(), threadHandels.data(), true, 5000);
	int threadNum = 0;
	for each (HANDLE thread in threadHandels)
	{
		if(TerminateThread(thread, 0) != NULL)
		{
			EnterCriticalSection(&criticalSectionForThread);
			fprintf(logFile, "Thread %d terminated\n", threadNum);
			LeaveCriticalSection(&criticalSectionForThread);
		}
		threadNum++;
	}
	free(threadHandels.data());
	fprintf(logFile, "Exit from the program.\n");
	fclose(logFile);
}


DWORD WINAPI ThreadPool::ThreadProc(LPVOID lParam)
{
	Task* task;
	while (active==true)
	{
		WaitForSingleObject(semaphoreTask, INFINITE);
		EnterCriticalSection(&criticalSectionForThread);
		activeThreadsCount++;
		task = taskQueue.front();
		taskQueue.pop();
		LeaveCriticalSection(&criticalSectionForThread);
		task->Execute();
		EnterCriticalSection(&criticalSectionForTerminal);
		task->Message();
		LeaveCriticalSection(&criticalSectionForTerminal);
		EnterCriticalSection(&criticalSectionForFile);
		fprintf(logFile, "Task completed\n");
		LeaveCriticalSection(&criticalSectionForFile);
		activeThreadsCount--;
		ReleaseSemaphore(semaphoreThread, 1, NULL);
	}
	return 0;
}	

void ThreadPool::AddTask(Task* task)
{
	if (activeThreadsCount == threadsCount)
	{
		EnterCriticalSection(&criticalSectionForTerminal);
		puts("All threads are busy\n");
		AddThread();
		puts("Added a new thread\n");
		LeaveCriticalSection(&criticalSectionForTerminal);
		EnterCriticalSection(&criticalSectionForFile);
		fprintf(logFile, "Add new thread\n");
		LeaveCriticalSection(&criticalSectionForFile);
		ReleaseSemaphore(semaphoreThread, 1, NULL);
	}
	WaitForSingleObject(semaphoreThread, INFINITE);
	EnterCriticalSection(&criticalSectionForThread);
	taskQueue.push(task);
	LeaveCriticalSection(&criticalSectionForThread);
	EnterCriticalSection(&criticalSectionForTerminal);
	puts( "Add task\n");
	LeaveCriticalSection(&criticalSectionForTerminal);
	EnterCriticalSection(&criticalSectionForFile);
	fprintf(logFile, "Add task\n");
	LeaveCriticalSection(&criticalSectionForFile);
	ReleaseSemaphore(semaphoreTask, 1, NULL);
}

void ThreadPool::AddThread()
{
	threadHandels.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadProc, this, 0, NULL));
	threadsCount++;
}
