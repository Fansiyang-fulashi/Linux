#pragma once

#include<string>
#include<vector>
#include <stdio.h>

class Command
{
public:
	Command()
	{}

	std::string Execute(const std::string& cmd)
	{
		FILE* fp=popen(cmd.c_str(),"r");
		if(fp==nullptr)
		{
			return std::string("这不是命令");
		}
		std::string res;
		char line[1024];
		while(fgets(line,sizeof(line),fp))
		{
			res+=line;
		}
		pclose(fp);
		return res;
	}

	~Command()
	{}
private:
	std::string _cmd;
	std::vector<std::string> _white_list;
};