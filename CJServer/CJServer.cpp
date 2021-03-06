// CJServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include<conio.h>
#include <iostream>
#include<tchar.h>
#include"SelectIOServer.h"

SelectIOServer s;

struct STest
{
	int cmd;
	byte cc;
	int a;
	DWORD time;
	char name[100];
	char name2[10];
};

int sendcount = 0;
int lastsendcount = 0;
int recvcount = 0;
int lastrecvcount = 0;

void WINAPI RecvDataCall(DWORD dwNetworkIndex, BYTE* pMsg, WORD wSize)
{
	if (wSize == 0 || pMsg == NULL)
		return;
	else
	{
		std::cout << pMsg << std::endl;
		++recvcount;
		s.Send(dwNetworkIndex,pMsg,wSize);
		++sendcount;
	}
}

int _tmain(int argc,_TCHAR* argv[])
{
	int port = 123;
	char ip[] = "127.0.0.1";
	if (s.Init(ip, port, RecvDataCall, 999))
	{
		printf("Create Server Success\n");
	}
	else
	{
		printf("Create Server Fail\n");
		return 0;
	}

	printf("按任意键退出。。。。。。。。。\n");

	DWORD last = ::GetTickCount();
	while (s.IsLitenering())
	{
		if (_kbhit())
		{
			_getch();
			break;
		}
		s.Update();

		if (GetTickCount() - last > 1000)
		{
			last = GetTickCount();
			printf("有[%d]个客户连接 发包 [%d/s]  收包[%d/s]\n",s.GetNumberOfConnections(),sendcount,recvcount);
			lastrecvcount = recvcount;
			lastsendcount = sendcount;
			sendcount = 0;
			recvcount = 0;
		}
		Sleep(1);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
