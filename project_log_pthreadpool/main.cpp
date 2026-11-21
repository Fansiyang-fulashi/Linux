#include"log.hpp"
#include"threadpool.hpp"
#include"task.hpp"

int main()
{
	// Enable_Console_Log_Stategy();
	// LOG(LogLevel::DEBUG)<<"鉴纯夏";
	// LOG(LogLevel::DEBUG)<<"鉴纯夏";
	// LOG(LogLevel::DEBUG)<<"鉴纯夏";
	// LOG(LogLevel::DEBUG)<<"鉴纯夏"<<" "<<1314;
	// Log log;
	// //log.UseFileLogStategy();
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";
	// log(LogLevel::DEBUG,"main.cpp",6)<<"hello world!"<<"鉴纯夏";

	Enable_Console_Log_Stategy();
	ThreadPool<func_t>* t=ThreadPool<func_t>::GetInstance();
	t->Push(Download);
	
	sleep(2);
	t->Stop();
	t->Join();
	return 0;
}