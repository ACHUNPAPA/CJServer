#pragma once
#include<list>
#include"INetServer.h"

class Session
{
	SOCKET m_c;
	bool m_remove;
	PACKET_HEADER pack_hdr;
	char buf[MAX_BUF_SIZE];

public:
	Session(SOCKET c);
	~Session(void);

	SOCKET GetSocket()
	{
		return m_c;
	}

	BOOL IsRemoved()
	{
		return m_remove;
	}

	BOOL Send(const BYTE* pMsg,const WORD wSize);

	BOOL RecvData(ServerProcessRecvData* m_pProcessRecvData);
};

typedef std::list<Session*> SESSION_LIST;
typedef SESSION_LIST::iterator SESSION_LIST_ITER;