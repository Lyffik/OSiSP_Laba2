#include "stdafx.h"
#include "CustomTask.h"
#include <windows.h>
#include <iostream>

using namespace std;


CustomTask::CustomTask()
{
}

void CustomTask::Execute()
{
	Sleep(6000);
}

void CustomTask::Message()
{
	puts("Task 1 complete");
}