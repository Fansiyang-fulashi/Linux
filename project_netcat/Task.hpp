
#pragma once
#include"cal.hpp"

class Cal
{
public:
	Response Execute(Request &resq)
	{
		Response resp;
		switch (resq.Oper())
		{
		case '+':
		{
			resp.SetResult() = resq.X() + resq.Y();
			break;
		}
		case '-':
		{
			resp.SetResult() = resq.X() - resq.Y();
			break;
		}
		case '*':
		{
			resp.SetResult() = resq.X() * resq.Y();
			break;
		}
		case '/':
		{
			if (resq.Y() == 0)
			{
				resp.SetCode() = CODE_KEY::DIVISION_BY_ZERO;
			}
			else
			{
				resp.SetResult() = resq.X() / resq.Y();
			}
			break;
		}
		case '%':
		{
			if (resq.Y() == 0)
			{
				resp.SetCode() = CODE_KEY::MODUIO_BY_ZERO;
			}
			else
			{
				resp.SetResult() = resq.X() % resq.Y();
			}
			break;
		}
		default:
		{
			resp.SetCode() = CODE_KEY::UNDINE_CAL;
			break;
		}
		}
		return resp;
	}

	void Execute(Response &resp)
	{
		if(resp.SetCode()!=CODE_KEY::TRUE)
		{
			LOG(LogLevel::INFO)<<"CAl ERROR!!!";
			return;
		}
		LOG(LogLevel::INFO)<<"运算结果为:"<<resp.SetResult();
	}

private:
};