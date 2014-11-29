#include "stdafx.h"
#include "CustomTask.h"
#include "Task.h"
#include "ThreadPool.h"
#include <iostream>
#include <conio.h>

DWORD WINAPI ControlFunction(CONST LPVOID lpParam)
{
	int count;
	puts("Enter number of thread");
	scanf_s("%d", &count);
	if (count < 0)
	{
		count = 4;
	}
	ThreadPool* threadPool;
	threadPool = new ThreadPool(count);
	puts("Enter number of task: 1, 2, 3");
	puts("Press 'Enter' for exit");
	int n = 0;
	while (n != 13)
	{
		n = _getch();
		Task* currentTask;
		currentTask = (Task*)new CustomTask();
		switch (n)
		{
			case '1':
			{
				currentTask = (Task*)new CustomTask();
				break; 
			}
			case '2':
			{
				currentTask = (Task*)new CustomTask();
				break;
			}
			case '3':
			{
				currentTask = (Task*)new CustomTask();
				break;
			}
		}
		threadPool->AddTask(currentTask);
	}
	delete threadPool;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE tempThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ControlFunction, NULL, 0, NULL);
	WaitForSingleObject(tempThread, INFINITE);
	return 0;
}

