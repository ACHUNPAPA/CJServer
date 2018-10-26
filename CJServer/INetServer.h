#pragma once
#pragma comment(lib,"Ws2_32")
#include<stdio.h>
#include<winsock.h>

typedef VOID WINAPI ServerProcessRecvData(DWORD dwNetworkIndex,BYTE* pMsg,WORD wSize);

class INetServer
{
public:
	virtual BOOL IsListener() = 0;

	virtual BOOL Init(char* IP,WORD Port,ServerProcessRecvData* serverProcessRecvData,DWORD MaxConnectNum) = 0;

	virtual VOID Shutdown() = 0;

	virtual VOID Update() = 0;

	virtual BOOL Disconnect(DWORD dwNetworkIndex) = 0;

	virtual BOOL Send(DWORD dwNetworkIndex,BYTE* pMsg,WORD wSize) = 0;

	virtual DWORD GetNumberOfConnections() = 0;

	virtual char* GetIP(DWORD dwNetworkIndex) = 0;
private:

};
