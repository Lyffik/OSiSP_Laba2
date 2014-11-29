#include "stdafx.h"
#include "ThreadPool.h"

#define FIXED_NUMBER 20

ThreadPool::ThreadPool(int count)
{
	threadsCount = count;
	active = true;
	#pragma warning(disable: 4996)
	logFile = fopen("log.txt", "wt");
	InitializeCriticalSection(&criticalSectionForThread);
	InitializeCriticalSection(&criticalSectionForFile);
	semaphoreThread = CreateSemaphore(NULL, count, count, NULL);
	semaphoreTask = CreateSemaphore(NULL, 0, count, NULL);

	for (int i = 0; i < count; i++)
	{
		threadHandels.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadProc, this, 0, NULL));
	}
}

static DWORD WINAPI StaticThreadProc(LPVOID lParam)
{
	ThreadPool* pool = (ThreadPool*)lParam;
	pool->ThreadProc();
	return 0;
}

void ThreadPool::ThreadProc()
{
	Task* task;
	while (true)
	{
		WaitForSingleObject(semaphoreTask, INFINITE);
		EnterCriticalSection(&criticalSectionForThread);
		countActiveThreads++;
		task = taskQueue.front();
		taskQueue.pop();
		LeaveCriticalSection(&criticalSectionForThread);
		ReleaseSemaphore(semaphoreThread, 1, NULL);
		EnterCriticalSection(&criticalSectionForFile);
		fprintf(logFile, "Task completed. Result true\n");
		LeaveCriticalSection(&criticalSectionForFile);
		countActiveThreads--;
	}
}
void ThreadPool::AddTask(Task* task)
{
	if (countActiveThreads == threadsCount)
		puts("All threads are busy\n");
	WaitForSingleObject(semaphoreThread, INFINITE);
	EnterCriticalSection(&criticalSectionForThread);
	taskQueue.push(task);
	LeaveCriticalSection(&criticalSectionForThread);
	if (active)
		fprintf(logFile, "Add task\n");
	else
		fprintf(logFile, "Add task of terminated\n");
	ReleaseSemaphore(semaphoreTask, 1, NULL);
}
void ThreadPool::AddThread()
{
	threadHandels.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadProc, this, 0, NULL));
	threadsCount++;
}
