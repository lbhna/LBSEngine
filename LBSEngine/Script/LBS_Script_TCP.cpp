#include "stdafx.h"
#include "./LBS_Script_TCP.h"
#include "./LBS_Function.h"

#include "../Syntax/LBS_SyntaxParser.h"

CLBS_Script_TCP_Packet::CLBS_Script_TCP_Packet(LPVOID IN pBuf,LONG IN lLen)
{
	m_pBuf		=new BYTE[lLen+ (16-lLen%8)];
	memcpy(m_pBuf,pBuf,lLen);
	m_lSize		=lLen;
	m_lSendPos	=0;
}
CLBS_Script_TCP_Packet::~CLBS_Script_TCP_Packet()
{
	LBS_SafeDeleteArray(m_pBuf);
}

BOOL			CLBS_Script_TCP_Packet::SendData(CLBS_Script_TCP* IN pTcp)
{
	if(m_lSendPos < m_lSize )
	{
		LONG send_len =send(pTcp->GetHandle(),(CHAR*)m_pBuf+m_lSendPos,m_lSize - m_lSendPos,0);
		if(send_len == -1)
		{
			if(GetLastError()!=WSAEWOULDBLOCK)
				return FALSE;
		}
		else
		{
			m_lSendPos += send_len;
		}
	}
	return TRUE;
}
BOOL			CLBS_Script_TCP_Packet::IsSendCompleted()
{
	return (m_lSendPos>=m_lSize);
}

CLBS_Script_TCP::CLBS_Script_TCP(CLBS_SyntaxParser* IN pParser)
{
	m_Type					=LOT_SCRIPT_TCP;
	m_pParser				=pParser;
	m_hSocket				=INVALID_SOCKET;
	m_pSocket				=NULL;
	m_bServer				=FALSE;
	m_pAcceptCallbackFunc	=NULL;
	m_pRecvCallbackFunc		=NULL;
	m_pCloseCallbackFunc	=NULL;
	m_strBindIpaddr			=_T("");
	m_uBindPort				=0;
	m_pCurrentSendBuffer	=NULL;

}
CLBS_Script_TCP::~CLBS_Script_TCP()
{
	Close();
	m_hSocket	=INVALID_SOCKET;
	LBS_SafeRelease(m_pAcceptCallbackFunc);
	LBS_SafeRelease(m_pRecvCallbackFunc	);
	LBS_SafeRelease(m_pCloseCallbackFunc);

}
BOOL			CLBS_Script_TCP::Create(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc,BOOL IN bServer)
{
	m_bServer	=bServer;
	m_pSocket	=new CLBS_Socket(this);	
	if(m_bServer)
	{
		if(!m_pSocket->Create(uPort,SOCK_STREAM,pIp))
		{
			LBS_SafeDelete(m_pSocket);
			return FALSE;
		}
		m_strBindIpaddr		=pIp;
		m_uBindPort			=uPort;
		m_pSocket->Listen();
	}
	else
	{
		if(!m_pSocket->Create())
		{
			LBS_SafeDelete(m_pSocket);
			return FALSE;
		}
	}
	m_hSocket	=m_pSocket->m_hSocket;

	m_pAcceptCallbackFunc	=pAcceptCallbackFunc;
	m_pAcceptCallbackFunc->AddRef();
	m_pRecvCallbackFunc		=pRecvCallbackFunc;
	m_pRecvCallbackFunc->AddRef();
	m_pCloseCallbackFunc	=pCloseCallbackFunc;
	m_pCloseCallbackFunc->AddRef();


	return TRUE;
}
BOOL			CLBS_Script_TCP::Create(CLBS_Socket* IN pSocket,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc)
{
	m_bServer	=FALSE;
	m_pSocket	=pSocket;
	m_hSocket	=m_pSocket->m_hSocket;

	m_pAcceptCallbackFunc	=pAcceptCallbackFunc;
	m_pAcceptCallbackFunc->AddRef();
	m_pRecvCallbackFunc		=pRecvCallbackFunc;
	m_pRecvCallbackFunc->AddRef();
	m_pCloseCallbackFunc	=pCloseCallbackFunc;
	m_pCloseCallbackFunc->AddRef();

	m_pSocket->GetPeerName(m_strBindIpaddr,m_uBindPort);
	return TRUE;
}


BOOL			CLBS_Script_TCP::Connect(LPCSTR IN pIp,UINT IN uPort)
{
	if(IsServer() || m_pSocket == NULL)
		return FALSE;
	m_strBindIpaddr	=pIp;
	m_uBindPort		=uPort;
	return m_pSocket->Connect(pIp,uPort);
}
LONG			CLBS_Script_TCP::Send(LPVOID IN pBuf,LONG IN lLen)
{
	CLBS_Script_TCP_Packet*		pPacket =new CLBS_Script_TCP_Packet(pBuf,lLen);
	m_theSendBuffers.AddObj(pPacket);
	pPacket->Release();
	if(m_pCurrentSendBuffer == NULL)
	{
		m_pCurrentSendBuffer	=pPacket;
		m_pCurrentSendBuffer->SendData(this);
		if(m_pCurrentSendBuffer->IsSendCompleted())
			m_pCurrentSendBuffer	=NULL;
	}

	return lLen;
}
LONG			CLBS_Script_TCP::SendToClient(SOCKET IN hClientHandle,LPVOID IN pBuf,LONG IN lLen)
{
	CLBS_Script_TCP*	pClient =(CLBS_Script_TCP*)FindClient(hClientHandle);
	if(pClient == NULL)
		return -1;
	return pClient->Send(pBuf,lLen);
}

CLBS_ObjBase*	CLBS_Script_TCP::FindClient(SOCKET IN hSocket)
{
	CLBS_Script_TCP*	pTcp	=NULL;
	for(LONG i=0;i<GetClientCount();i++)
	{
		pTcp	=(CLBS_Script_TCP*)GetClient(i);
		if(pTcp->GetHandle() == hSocket)
			return pTcp;
	}
	return NULL;
}
VOID			CLBS_Script_TCP::CloseClient(SOCKET IN hSocket)
{
	m_theClients.DelObj(FindClient(hSocket));
}
LONG			CLBS_Script_TCP::GetClientCount()
{
	return m_theClients.GetCount();
}
CLBS_ObjBase*	CLBS_Script_TCP::GetClient(LONG IN lIndex)
{
	if(!IsServer())
		return NULL;
	return m_theClients.GetObj(lIndex);
}
BOOL			CLBS_Script_TCP::Close()
{
	m_pCurrentSendBuffer=NULL;
	m_theSendBuffers.Clear();
	m_hSocket	=INVALID_SOCKET;
	m_theClients.Clear();
	LBS_SafeDelete(m_pSocket);
	return TRUE;
}
void			CLBS_Script_TCP::OnAccept	(CLBS_Socket* IN pSocket)
{
	if(pSocket != m_pSocket)
		return;
	CLBS_Socket*	pClient =new CLBS_Socket(this);
	if(!m_pSocket->Accept(*pClient))
	{
		LBS_SafeDelete(pClient);
		return;
	}
	CLBS_Script_TCP*	pNewClient =new CLBS_Script_TCP(m_pParser);
	pNewClient->Create(pClient,m_pAcceptCallbackFunc,m_pRecvCallbackFunc,m_pCloseCallbackFunc);
	m_theClients.AddObj(pNewClient);
	pNewClient->Release();

	//--------------------------------------------
	CLBS_Function*						pFunc =(CLBS_Function*)m_pAcceptCallbackFunc;
	CLBS_Script_CallFunctionPara		Parameter;
	Parameter.Push((DWORD)pNewClient->GetHandle());
	
	CLBS_Variable	*pVar,*pPara;	
	pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(0);
	pPara	=(CLBS_Variable*)Parameter.GetObj(0);
	*pVar	=*pPara;
	pVar->SetRefVaraibleInfo(0,pPara);	
	pFunc->Execute();

}
void			CLBS_Script_TCP::OnClose	(CLBS_Socket* IN pSocket)
{
	//--------------------------------------------
	CLBS_Function*						pFunc =(CLBS_Function*)m_pCloseCallbackFunc;
	CLBS_Script_CallFunctionPara		Parameter;
	Parameter.Push((DWORD)pSocket->m_hSocket);

	CLBS_Variable	*pVar,*pPara;	
	pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(0);
	pPara	=(CLBS_Variable*)Parameter.GetObj(0);
	*pVar	=*pPara;
	pVar->SetRefVaraibleInfo(0,pPara);	
	pFunc->Execute();

	//--------------------------------------------
	if(pSocket != m_pSocket)
		CloseClient(pSocket->m_hSocket);
}
void			CLBS_Script_TCP::OnReceive	(CLBS_Socket* IN pSocket)
{
	CHAR	buf[4096];
	int		len		=pSocket->Receive(buf,sizeof(buf)-2);
	if(len > 0)
	{
		buf[len]	=0;
		buf[len+1]	=0;

		CLBS_Function*						pFunc =(CLBS_Function*)m_pRecvCallbackFunc;
		CLBS_Script_CallFunctionPara		Parameter;
		CLBS_Variable						*pVar,*pPara;	

		Parameter.Push((DWORD)pSocket->m_hSocket);
		Parameter.Push(buf,len);

		for(LONG i=0;i<pFunc->GetParameterVariableCount();i++)
		{
			pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(i);
			pPara	=(CLBS_Variable*)Parameter.GetObj(i);
			*pVar	=*pPara;
			pVar->SetRefVaraibleInfo(0,pPara);
		}
		pFunc->Execute();
	}
}

void			CLBS_Script_TCP::OnSend		(CLBS_Socket* IN pSocket)
{
	if(pSocket == m_pSocket)
	{
		if(m_pCurrentSendBuffer != NULL)
		{
			if(m_pCurrentSendBuffer->IsSendCompleted())
			{
				m_theSendBuffers.DelObj((LONG)0);
				m_pCurrentSendBuffer	=(CLBS_Script_TCP_Packet*)m_theSendBuffers.GetObj(0);
			}
		}

		if(m_pCurrentSendBuffer != NULL)
		{
			if(!m_pCurrentSendBuffer->SendData(this))
			{
				m_pCurrentSendBuffer	=NULL;
			}
		}
	}
	else
	{
		CLBS_Script_TCP*	pClient =(CLBS_Script_TCP*)FindClient(pSocket->m_hSocket);
		if(pClient != NULL)
		{
			pClient->OnSend(pSocket);
		}
	}
}

