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

