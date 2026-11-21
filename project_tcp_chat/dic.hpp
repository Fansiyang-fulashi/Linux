#pragma once

#include<iostream>
#include<string>
#include<unordered_map>
#include<fstream>
#include"log.hpp"
#include"InetAddr.hpp"

std::string DAULT_PATH="./dictionary.txt";
std::string SEP=":";

class Dic
{
public:
	Dic(std::string path=DAULT_PATH)
	:_path(path)
	{}

	void Load()
	{
		std::ifstream fin(_path);
		std::string line;
		while(getline(fin,line))
		{
			auto pos=line.find(SEP);
			if(pos==std::string::npos)
			{
				LOG(LogLevel::WARNING)<<"cannot find sep";
				continue;
			}
			std::string english=line.substr(0,pos);
			std::string chinese=line.substr(pos+SEP.size());
			if(english.empty()||chinese.empty())
			{
				LOG(LogLevel::WARNING)<<"english or chinese cannot find";
				continue;
			}
			_dic.insert({english,chinese});
		}
		LOG(LogLevel::FATAL)<<"load final";
	}

	std::string Find(const std::string& english)
	{
		auto res=_dic.find(english);
		if(res==_dic.end())
		{
			return "None";
		}
		return _dic[english];
	}

	~Dic()
	{}
private:
	std::string _path;
	std::unordered_map<std::string,std::string> _dic;
};