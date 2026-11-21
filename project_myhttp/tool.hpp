#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

class Tool
{
public:
	static bool ReadFile(std::string filename,std::string* out)
	{
		(*out).clear();
		std::ifstream r(filename,std::ios::in|std::ios::binary);
		if(!r.is_open())
		{
			std::cout<<"打开文件错误";
			return false;
		}
		std::stringstream buffer;
		buffer<<r.rdbuf();
		*out=buffer.str();
		return true;
	}

	static bool GetLine(std::string& str,std::string src,std::string* out)
	{
		auto pos=str.find(src);
		if(pos==std::string::npos)
			return false;
		*out=str.substr(0,pos);
		str.erase(0,pos+src.size());
		return true;
	}

	static size_t FileSize(std::string filename)
	{
		std::ifstream in(filename,std::ios::binary);
		in.seekg(0,in.end);
		size_t filesize=in.tellg();
		in.seekg(0,in.beg);
		return filesize;
	}
};