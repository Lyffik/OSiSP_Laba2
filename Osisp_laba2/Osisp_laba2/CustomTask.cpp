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
	Sleep(4000);
}

void CustomTask::Message()
{
	//cout<<"Task complete";
}