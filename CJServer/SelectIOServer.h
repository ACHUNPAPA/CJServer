#pragma once
#include"Session.h"
#include"Selector.h"

class SelectIOServer
{
public:
	SelectIOServer(void);
	virtual ~SelectIOServer(void);

	virtual BOOL IsLitenering();

	virtual BOOL Init(char* IP,WORD Port,ServerProcessRecvData* pProcessRecvData,DWORD MaxConnectNum);

	virtual VOID Shutdown();

	virtual VOID Update();

	virtual BOOL Disconnect(DWORD dwNetworkIndex);

	virtual BOOL Send(DWORD dwNetworkIndex,BYTE* pMsg,WORD wSize);

	virtual DWORD GetNumberOfConnections();

	virtual char* GetIP(DWORD dwNetworkIndex);

private:
	SOCKET m_s;

	DWORD m_dwMaxSessionNum;

	ServerProcessRecvData* m_pProcessRecvData;

	SESSION_LIST m_clientlist;

	Selector m_select;

	Session* _List_Find(DWORD dwNetworkIndex);

	void HandleAccept();

	void HandleSession();
};