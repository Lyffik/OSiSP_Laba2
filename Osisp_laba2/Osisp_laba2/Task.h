#pragma once
class Task
{
	public:
		Task();
		virtual void Execute();
		virtual void Message();
		~Task();
};

