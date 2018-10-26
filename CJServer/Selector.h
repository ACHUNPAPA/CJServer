#pragma once
#pragma comment(lib,"Ws2_32")
#include<stdio.h>
#include<winsock.h>

class Selector
{
public:
	Selector(void)
	{

	}
	~Selector(void)
	{

	}

	inline void reset()
	{
		FD_ZERO(&fdSocket);
	}

	inline void add_sock(SOCKET s)
	{
		FD_SET(s,&fdSocket);
	}

	inline int test_sock(SOCKET s)
	{
		return FD_ISSET(s,&fdSocket);
	}

	int select(int timeout = 10)
	{
		struct timeval tv =
		{
			0,timeout
		};

		int nRet = ::select(0, &fdSocket, NULL,NULL,&tv);
		return nRet;
	}

private:
	fd_set fdSocket;
};
