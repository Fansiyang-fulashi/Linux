#include"SelectServer.hpp"

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		LOG(LogLevel::FATAL)<<"使用错误";
		exit(ERROR::USE_ERROR);
	}

	std::unique_ptr<EpollServer> ss=std::make_unique<EpollServer>(std::stoi(argv[1]));

	ss->Start();
	
}