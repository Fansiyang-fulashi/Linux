#pragma once

#include"comm.hpp"

class Task
{
public:
	Task()
	{}

	Task(int x,int y)
	:_x(x)
	,_y(y)
	{
		_ans=_x+_y;
	}

	~Task()
	{}
private:
	int _x;
	int _y;
	int _ans;
};