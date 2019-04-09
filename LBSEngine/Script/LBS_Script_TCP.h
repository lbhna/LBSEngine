#pragma once
#include "../Base/LBS_ObjBase.h"
/*====================================================================\
		#创建日期:	2015/09/23
		#文件名  :	LBS_Script_TCP
		#作者    :	LBHNA   
		#说明    :	TCP通讯接口类
\====================================================================*/

#include "LBS_Socket.h"

class CLBS_SyntaxParser;
class CLBS_Script_TCP;
class CLBS_Script_TCP_Packet:
	public CLBS_ObjBase
{
protected:
	BYTE*			m_pBuf;
	LONG			m_lSize;
	LONG			m_lSendPos;
public:
	CLBS_Script_TCP_Packet(LPVOID IN pBuf,LONG IN lLen);
	virtual ~CLBS_Script_TCP_Packet();
public:
	BOOL			SendData(CLBS_Script_TCP* IN pTcp);
	BOOL			IsSendCompleted();
};
class CLBS_Script_TCP:
	public CLBS_ObjBase
{
protected:
	CLBS_SyntaxParser*			m_pParser;
	BOOL						m_bServer;
	CString						m_strBindIpaddr;
	UINT						m_uBindPort;

	CLBS_ObjBase*				m_pAcceptCallbackFunc;
	CLBS_ObjBase*				m_pRecvCallbackFunc;
	CLBS_ObjBase*				m_pCloseCallbackFunc;

	CLBS_ObjListBase			m_theClients;
	SOCKET						m_hSocket;
	CLBS_Socket*				m_pSocket;
	CLBS_ObjListBase			m_theSendBuffers;
	CLBS_Script_TCP_Packet*		m_pCurrentSendBuffer;

public:
	CLBS_Script_TCP(CLBS_SyntaxParser* IN pParser);
	virtual ~CLBS_Script_TCP();
public:
	SOCKET			GetHandle(){return m_hSocket;}
	BOOL			IsServer(){return m_bServer;}
	CLBS_Socket*	GetSocket(){return m_pSocket;}
	LPCSTR			GetIpaddress(){return m_strBindIpaddr;}
	UINT			GetPort(){return m_uBindPort;}
public:

	BOOL			Create(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc,BOOL IN bServer);
	BOOL			Create(CLBS_Socket* IN pSocket,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc);
	BOOL			Connect(LPCSTR IN pIp,UINT IN uPort);
	LONG			Send(LPVOID IN pBuf,LONG IN lLen);
	LONG			SendToClient(SOCKET IN hClientHandle,LPVOID IN pBuf,LONG IN lLen);
	CLBS_ObjBase*	FindClient(SOCKET IN hSocket);
	VOID			CloseClient(SOCKET IN hSocket);
	LONG			GetClientCount();
	CLBS_ObjBase*	GetClient(LONG IN lIndex);
	BOOL			Close();
public:
	void			OnAccept	(CLBS_Socket* IN pSocket);
	void			OnClose		(CLBS_Socket* IN pSocket);
	void			OnReceive	(CLBS_Socket* IN pSocket);
	void			OnSend		(CLBS_Socket* IN pSocket);
};


