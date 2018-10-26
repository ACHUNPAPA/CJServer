#pragma once
#include<winsock.h>

class PacketHandler
{
public:
	PacketHandler(void);
	~PacketHandler(void);
	
	void ParsePacket(DWORD dwNetworkIndex,BYTE* pMsg,WORD wSize);

	void On_C2S_LOGIN(DWORD dwNetworkIndex,BYTE* pMsg,WORD wSize);
};
