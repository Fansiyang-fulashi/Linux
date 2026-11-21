#pragma once

#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<filesystem>
#include<fstream>
#include<memory>
#include"mutex.hpp"

std::string gsep ="\r\n";
std::string default_path="./log";
std::string default_filename="log.txt";

class LogStategy
{
public:
	virtual ~LogStategy()=default;
	virtual void SyncLog(const std::string& message)=0;
};

class ConsoleLogStategy:public LogStategy
{
public:
	void SyncLog(const std::string& message) override
	{
		{
			LockGuard lockguard(_mutex);
			std::cout << message << gsep;
		}
	}
private:
	Mutex _mutex;
};

class FileLogStategy:public LogStategy
{
public:
	FileLogStategy(std::string path=default_path,std::string filename=default_filename)
	:_path(path)
	,_filename(filename)
	{
		// mkdir(_path.c_str(), 0755);
		// std::string name = _path + (_path.back() == '/' ? std::string("") : std::string("/")) + _filename;
		// _fd = open(name.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
		// if (_fd < 0)
		// {
		// 	std::cout << "创建错误" << std::endl;
		// 	std::cerr << errno << std::endl;
		// }

		if(std::filesystem::exists(_path))
		{
			return;
		}
		try
		{
			std::filesystem::create_directories(_path);
		}
		catch(const std::filesystem::filesystem_error& e)
		{
			std::cerr<<e.what()<<std::endl;
		}
	}

	void SyncLog(const std::string& message) override
	{
		// {
		// 	LockGuard lockguard(_mutex);
		// 	int ret=write(_fd, message.c_str(), message.size());
		// 	if(ret<0)
		// 	{
		// 		std::cout<<"写入错误"<<std::endl;
		// 		std::cerr << errno << std::endl;
		// 	}
		// 	ret=write(_fd,gsep.c_str(),gsep.size());
		// 	if(ret<0)
		// 	{
		// 		std::cout<<"写入错误"<<std::endl;
		// 		std::cerr << errno << std::endl;
		// 	}
		// }
		{
		LockGuard lockguard(_mutex);
		std::string name = _path + (_path.back() == '/' ? std::string("") : std::string("/")) + _filename;
			std::ofstream out(name,std::ios::app);
			if(!out.is_open())
			{
				return;
			}
			out<<message<<gsep;
			out.close();
		}
	}

	~FileLogStategy()
	{
		//close(_fd);
	}
private:
	std::string _path;
	std::string _filename;
	int _fd;
	Mutex _mutex;
};

enum class LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL
};

std::string GetTime()
{
	time_t curr=time(nullptr);
	struct tm curr_time;
	localtime_r(&curr,&curr_time);
	char TIME[256];
	snprintf(TIME,sizeof(TIME),"%4d-%02d-%02d %02d:%02d:%02d",curr_time.tm_year+1900,curr_time.tm_mon+1,curr_time.tm_mday,curr_time.tm_hour,curr_time.tm_min,curr_time.tm_sec);
	return TIME;
}

class Log
{
public:
	Log()
	{
		UseConsoleLogStategy();
	}

	~Log()
	{}

	void UseConsoleLogStategy()
	{
		_fflush_stategy=std::make_unique<ConsoleLogStategy>();
	}

	void UseFileLogStategy()
	{
		_fflush_stategy=std::make_unique<FileLogStategy>();
	}

	class LogMessage
	{
	public:
		LogMessage(LogLevel loglevel,std::string src_name,int line_number,Log& log)
		:_time(GetTime())
		,_loglevel(loglevel)
		,_pid(getpid())
		,_src_name(src_name)
		,_line_number(line_number)
		,_log(log)
		{
			std::stringstream ss;
			ss<<"["<<_time<<"]"<<"["<<LogLevel_Print()<<"]"<<"["<<_pid<<"]"<<"["<<_src_name<<"]"<<"["<<_line_number<<"]"<<"-";
			_loginfo=ss.str();
		}

		template<class T>
		LogMessage& operator<<(const T& info)
		{
			std::stringstream ss;
			ss<<info;
			_loginfo+=ss.str();
			return *this;
		}

		~LogMessage()
		{
			if(_log._fflush_stategy)
			{
				_log._fflush_stategy->SyncLog(_loginfo);
			}
		}

	private:
		std::string LogLevel_Print()
		{
			switch (_loglevel)
			{
			case LogLevel::DEBUG:
				return "DEBUG";
			case LogLevel::ERROR:
				return "ERROR";
			case LogLevel::FATAL:
				return "FATAL";
			case LogLevel::INFO:
				return "INFO";
			case LogLevel::WARNING:
				return "WARNING";
			default:
				return "UNKONWN";
			}
		}

		std::string _time;
		LogLevel _loglevel;
		pid_t _pid; 
		std::string _src_name;
		int _line_number;
		std::string _loginfo;
		Log& _log;
	};

	LogMessage operator()(LogLevel loglevel,const std::string& name,int line)
	{
		return LogMessage(loglevel,name,line,*this);
	}

private:
	std::unique_ptr<LogStategy> _fflush_stategy;
};

Log log;
#define LOG(level) log(level,__FILE__,__LINE__)
#define Enable_Console_Log_Stategy() log.UseConsoleLogStategy()
#define Enable_File_Log_Stategy() log.UseFileLogStategy()