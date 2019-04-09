#include "StdAfx.h"
#include "./LBS_Script_UDP.h"
#include "./LBS_Function.h"
#include "./LBS_EventWnd.h"



CLBS_UDP_Packet::CLBS_UDP_Packet(LPCSTR IN pFromIP,UINT IN uFromPort,BYTE* IN pData,DWORD IN dwSize)
{
	m_pBuffer	=new BYTE[dwSize+ (16-dwSize%8)];
	memcpy(m_pBuffer,pData,dwSize);
	m_dwSize	=dwSize;
	m_strFromIp	=pFromIP;
	m_uFromPort	=uFromPort;
}
CLBS_UDP_Packet::~CLBS_UDP_Packet()
{
	LBS_SafeDeleteArray(m_pBuffer);
	m_dwSize	=0;
}
BYTE*		CLBS_UDP_Packet::GetBuffer(){return m_pBuffer;}
DWORD		CLBS_UDP_Packet::GetSize(){return m_dwSize;}
UINT		CLBS_UDP_Packet::GetFromPort(){return m_uFromPort;}
LPCSTR		CLBS_UDP_Packet::GetFromIP(){return m_strFromIp;}




CLBS_Script_UDP::CLBS_Script_UDP()
{
	m_Type					=LOT_SCRIPT_UDP;
	m_hSocket				=INVALID_SOCKET;
	m_pRecvCallbackFunc		=NULL;
	m_uPort					=0;
	m_strIp					=_T("127.0.0.1");
	m_hRecvThread			=NULL;
	m_dwRecvThreadID		=0;

	m_bExit					=FALSE;
	InitializeCriticalSection(&m_hSync);
}
CLBS_Script_UDP::~CLBS_Script_UDP()
{
	Close();
	DeleteCriticalSection(&m_hSync);
}
BOOL	CLBS_Script_UDP::Create(UINT IN uPort,CLBS_ObjBase* IN pRecvCallbackFunc,LPCSTR IN pIpAddr)
{
	LBS_SafeRelease(m_pRecvCallbackFunc);
	m_pRecvCallbackFunc		=pRecvCallbackFunc;
	m_pRecvCallbackFunc->AddRef();
	m_uPort					=uPort;
	m_strIp					=pIpAddr;
	m_hSocket				=socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family				=AF_INET;
	sockAddr.sin_port				= htons((u_short)m_uPort);
	if (pIpAddr == NULL)
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		sockAddr.sin_addr.s_addr = inet_addr(pIpAddr);
	if(sockAddr.sin_addr.s_addr == INADDR_NONE)
		return FALSE;

	if(bind(m_hSocket,(SOCKADDR*)&sockAddr, sizeof(sockAddr))==SOCKET_ERROR)
		return FALSE;

	m_RecvTimer		=SetTimer(CLBS_EventWnd::GetInstance().GetWnd(),(UINT_PTR)this,10,CLBS_Script_UDP::Recv_TimerProc);
	m_hRecvThread	=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CLBS_Script_UDP::TF_RecvThread,(LPVOID)this,0,&m_dwRecvThreadID);
	if(m_hRecvThread == NULL)
		return FALSE;
	return TRUE;
}
VOID	CLBS_Script_UDP::Close()
{
	Lock();
	m_bExit	=TRUE;
	m_thePacketList.Clear();
	UnLock();
	if(m_hSocket != NULL)
	{
		closesocket(m_hSocket);
		m_hSocket	=NULL;
	}	
	if(m_hRecvThread != NULL)
	{
		Sleep(5);
		DWORD code =0;
		int count =0;
		GetExitCodeThread(m_hRecvThread,&code);
		while(code == STILL_ACTIVE && count++ <5)
		{
			TerminateThread(m_hRecvThread,1);
			Sleep(5);
			GetExitCodeThread(m_hRecvThread,&code);
		}
		m_hRecvThread =NULL;
	}

	KillTimer(CLBS_EventWnd::GetInstance().GetWnd(),m_RecvTimer);
	LBS_SafeRelease(m_pRecvCallbackFunc);
}

LONG	CLBS_Script_UDP::SendTo(LPCSTR IN pIp,UINT IN uPort,VOID* IN pData,LONG IN lDataLen)
{
	if (pIp == NULL || pData==NULL || lDataLen <= 0)
		return -1;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family				=AF_INET;
	sockAddr.sin_port				= htons((u_short)uPort);
	sockAddr.sin_addr.s_addr		= inet_addr(pIp);
	if(sockAddr.sin_addr.s_addr == INADDR_NONE)
		return -2;
	return sendto(m_hSocket,(const char*)pData,lDataLen,0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
}


VOID CALLBACK		CLBS_Script_UDP::Recv_TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	CLBS_Script_UDP*	pUDP =(CLBS_Script_UDP*)idEvent;
	if(pUDP != NULL)
	{
		pUDP->ProcessPacket();
	}
}
VOID				CLBS_Script_UDP::ProcessRecv()
{

	SOCKADDR_IN from;
	ZeroMemory(&from,sizeof(from));
	int fromlen =sizeof(from);
	char buf[4096];
	int len		=recvfrom(m_hSocket,buf,sizeof(buf),0,(struct sockaddr*)&from,&fromlen);
	if(len <=0)
		return;

	DWORD	port	=from.sin_port;
	CString ip;
	ip.Format("%d.%d.%d.%d",from.sin_addr.s_net,from.sin_addr.s_host,from.sin_addr.s_lh,from.sin_addr.s_impno);
	
	Lock();
	CLBS_UDP_Packet*	pPacket =new CLBS_UDP_Packet(ip,port,(BYTE*)buf,len);
	m_thePacketList.AddObj(pPacket);
	pPacket->Release();
	UnLock();

}
CLBS_UDP_Packet*			CLBS_Script_UDP::PopOnePacket()
{
	CLBS_UDP_Packet*	pPacket =NULL;
	Lock();
	if(m_thePacketList.GetCount()>0)
	{
		pPacket	=(CLBS_UDP_Packet*)m_thePacketList.GetObj(0);
		pPacket->AddRef();
		m_thePacketList.DelObj(pPacket);
	}
	UnLock();
	return pPacket;
}
VOID				CLBS_Script_UDP::ProcessPacket()
{
	CLBS_Function*						pFunc =(CLBS_Function*)m_pRecvCallbackFunc;
	CLBS_Script_CallFunctionPara		Parameter;
	CLBS_Variable*						pVar	=NULL;
	CLBS_Variable*						pPara	=NULL;
	DWORD								ID		=m_RecvTimer;
	CLBS_UDP_Packet*	pPacket =NULL;
	while( TRUE)
	{
		pPacket	=PopOnePacket();
		if(pPacket == NULL)
			break;
		if(pFunc != NULL)
		{
			Parameter.Clear();
			Parameter.Push(ID);
			Parameter.Push((LPCSTR)pPacket->GetFromIP());
			Parameter.Push((DWORD)pPacket->GetFromPort());
			Parameter.Push(pPacket->GetBuffer(),pPacket->GetSize());


			for(LONG i=0;i<pFunc->GetParameterVariableCount();i++)
			{
				pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(i);
				pPara	=(CLBS_Variable*)Parameter.GetObj(i);
				*pVar	=*pPara;
				pVar->SetRefVaraibleInfo(0,pPara);
			}
			pFunc->Execute();
		}
		pPacket->Release();
	}
}

UINT				CLBS_Script_UDP::TF_RecvThread(LPVOID IN lp)
{
	CLBS_Script_UDP*	pUDP =(CLBS_Script_UDP*)lp;
	while(!pUDP->IsExit())
	{
		pUDP->ProcessRecv();
	}
	return 0;
}
BOOL				CLBS_Script_UDP::IsExit()
{
	Lock();
	BOOL bExit =m_bExit;
	UnLock();
	return bExit;
}

VOID				CLBS_Script_UDP::Lock()
{
	EnterCriticalSection(&m_hSync);
}
VOID				CLBS_Script_UDP::UnLock()
{
	LeaveCriticalSection(&m_hSync);
}



