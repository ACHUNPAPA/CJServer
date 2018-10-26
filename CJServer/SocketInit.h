#pragma once
#pragma comment(lib,"Ws2_32")
#include<winsock.h>

class SocketInit
{
	SocketInit(void)
	{
		WSADATA ws;
		::WSAStartup(MAKEWORD(2,2),&ws);
	}

public:
	static SocketInit& GetInstance()
	{
		static SocketInit s;
		return s;
	}

	~SocketInit()
	{
		::WSACleanup();
	}

private:

};
