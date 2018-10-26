#include"pch.h"
#include".\SelectIOServer.h"
#include"SocketInit.h"

SelectIOServer::SelectIOServer(void)
{
	m_s = INVALID_SOCKET;
	SocketInit::GetInstance();
}

SelectIOServer::~SelectIOServer(void)
{
	if (m_s != INVALID_SOCKET)
		Shutdown();
}

BOOL SelectIOServer::IsLitenering()
{
	return m_s != INVALID_SOCKET;
}

BOOL SelectIOServer::Init(char* IP,WORD Port,ServerProcessRecvData* pProcessRecvData,DWORD MaxConnectNum)
{
	m_dwMaxSessionNum = MaxConnectNum;
	m_pProcessRecvData = pProcessRecvData;
	m_s = socket(AF_INET,SOCK_STREAM,0);

	sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(Port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(m_s,(struct sockaddr*)&my_addr,sizeof(struct sockaddr));

	if (ret == SOCKET_ERROR)
	{
		Shutdown();
		return FALSE;
	}
	listen(m_s,5);
	return TRUE;
}


VOID SelectIOServer::Shutdown()
{
	closesocket(m_s);
	m_s = INVALID_SOCKET;
}


VOID SelectIOServer::Update()
{
	while (true)
	{
		m_select.reset();
		m_select.add_sock(m_s);

		SESSION_LIST_ITER iter;
		for (iter = m_clientlist.begin(); iter != m_clientlist.end(); ++iter)
		{
			Session* pSession = *iter;
			m_select.add_sock(pSession -> GetSocket());
		}

		if (m_select.select() > 0)
		{
			HandleAccept();
			HandleSession();
		}

		for (iter = m_clientlist.begin(); iter != m_clientlist.end(); ++iter)
		{
			Session* pSession = *iter;
			if (pSession->IsRemoved())
			{
				m_clientlist.erase(iter--);
				m_pProcessRecvData(pSession -> GetSocket(),0,0);
				delete pSession;
			}
		}
	}
}

void SelectIOServer::HandleAccept()
{
	if (m_select.test_sock(m_s))
	{
		if (GetNumberOfConnections() < m_dwMaxSessionNum)
		{
			SOCKET c = ::accept(m_s,0,0);
			if (c == INVALID_SOCKET)
				return;
			Session* pSession = new Session(c);
			m_clientlist.push_back(pSession);
		}
	}
}

void SelectIOServer::HandleSession()
{
	SESSION_LIST_ITER iter;
	for (iter = m_clientlist.begin(); iter != m_clientlist.end(); iter++)
	{
		Session* pSession = *iter;
		if (m_select.test_sock(pSession->GetSocket()))
		{
			pSession->RecvData(m_pProcessRecvData);
		}
	}
}


BOOL SelectIOServer::Send(DWORD dwNetworIndex, BYTE* pMsg, WORD wSize)
{
	Session* pSession = _List_Find(dwNetworIndex);
	if (!pSession)
		return FALSE;
	return pSession->Send(pMsg,wSize);
}

char* SelectIOServer::GetIP(DWORD dwNetworkIndex)
{
	sockaddr_in sa;
	int len = sizeof(sa);
	getpeername(dwNetworkIndex,(sockaddr*)&sa,&len);
	return inet_ntoa(sa.sin_addr);
}

DWORD SelectIOServer::GetNumberOfConnections()
{
	return (DWORD)m_clientlist.size();
}

BOOL SelectIOServer::Disconnect(DWORD dwNetworkIndex)
{
	closesocket(dwNetworkIndex);
	return TRUE;
}

Session* SelectIOServer::_List_Find(DWORD dwNetworkIndex)
{
	SESSION_LIST_ITER iter;
	for (iter = m_clientlist.begin(); iter != m_clientlist.end(); iter++)
	{
		Session* pSession = *iter;
		if (pSession->GetSocket() == dwNetworkIndex)
			return pSession;
	}
	return NULL;
}