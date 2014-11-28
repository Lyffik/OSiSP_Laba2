#include "stdafx.h"
#include "CustomTask.h"
#include <windows.h>


CustomTask::CustomTask()
{
}

void CustomTask::Execute()
{
	Sleep(5000);
}

CustomTask::~CustomTask()
{
}
