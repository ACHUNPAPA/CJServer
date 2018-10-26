#include "pch.h"
#include".\Session.h"

Session::Session(SOCKET c)
{
	this->m_c = c;
	m_remove = false;
}

Session::~Session(void)
{

}

BOOL Session::Send(const BYTE* pMsg,const WORD wSize)
{
	//包头
	PACKET_HEADER pack_hdr;
	pack_hdr.size = wSize;

	//发送包头
	int rt = send(m_c,(char*)&pack_hdr,sizeof(PACKET_HEADER),0);
	if (rt == SOCKET_ERROR || rt == 0)
	{
		m_remove = true;
		return FALSE;
	}
	return TRUE;
}


BOOL Session::RecvData(ServerProcessRecvData* m_pProcessRecvData)
{
	//包头
	pack_hdr.size = 0;

	//接收包头
	int rt = recv(m_c,(char*)&pack_hdr,sizeof(PACKET_HEADER),0);
	if (rt == SOCKET_ERROR || rt == 0)
	{
		m_remove = true;
		return FALSE;
	}

	//校验包头
	if (pack_hdr.size == 0)
	{
		m_remove = true;
		return FALSE;
	}

	//接收包体
	rt = recv(m_c,buf,pack_hdr.size,0);
	if (rt == SOCKET_ERROR || rt == 0)
	{
		m_remove = true;
		return FALSE;
	}
	m_pProcessRecvData(m_c,(BYTE*)buf,pack_hdr.size);
	return TRUE;
}
