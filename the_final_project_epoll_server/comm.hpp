#pragma once

enum ERROR
{
	OK=0,
	USE_ERROR,
	SOKET_ERROR,
	BIND_ERROR,
	LISTEN_ERROR,
	READ_ERROR,
	CONNECT_ERROR,
	FORK_ERROR,
	ACCEPT_ERROR,
	EPOLL_CREATE_ERROR,
};

class NoCopy
{
public:
	NoCopy()
	{}

	~NoCopy()
	{}

	NoCopy(const NoCopy&)=delete;

	const NoCopy& operator=(const NoCopy&)=delete;
private:
};

void SetNonBlock(int fd)
{
	int fl=fcntl(fd,F_GETFL);
	if(fl<0)
		return;
	fcntl(fd,F_SETFL,fl|O_NONBLOCK);
}

