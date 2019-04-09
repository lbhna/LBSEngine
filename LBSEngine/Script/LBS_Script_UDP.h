#pragma once
#include "../Base/LBS_ObjBase.h"
/*====================================================================\
		#创建日期:	2015/09/17
		#文件名  :	LBS_Script_UDP
		#作者    :	LBHNA   
		#说明    :	UDP通讯接口
\====================================================================*/

class CLBS_UDP_Packet:
	public CLBS_ObjBase
{
protected:
	BYTE*	m_pBuffer;
	DWORD	m_dwSize;

	CString m_strFromIp;
	UINT	m_uFromPort;
public:
	CLBS_UDP_Packet(LPCSTR IN pFromIP,UINT IN uFromPort,BYTE* IN pData,DWORD IN dwSize);
	virtual ~CLBS_UDP_Packet();
public:
	BYTE*		GetBuffer();
	DWORD		GetSize();
	UINT		GetFromPort();
	LPCSTR		GetFromIP();

};
class CLBS_Script_UDP:
	public CLBS_ObjBase
{

public:
	CLBS_Script_UDP();
	virtual ~CLBS_Script_UDP();
protected:
	CLBS_ObjBase*				m_pRecvCallbackFunc;
	UINT						m_uPort;
	CString						m_strIp;

	SOCKET						m_hSocket;

	UINT_PTR					m_RecvTimer;
	HANDLE						m_hRecvThread;
	DWORD						m_dwRecvThreadID;
	CLBS_ObjListBase			m_thePacketList;

	BOOL						m_bExit;
	CRITICAL_SECTION			m_hSync;
public:
	UINT						GetPort(){return m_uPort;}
	LPCSTR						GetIp(){return m_strIp;}
	UINT_PTR					GetID(){return m_RecvTimer;}


	BOOL						Create(UINT IN uPort,CLBS_ObjBase* IN pRecvCallbackFunc,LPCSTR IN pIpAddr=NULL);
	VOID						Close();
	LONG						SendTo(LPCSTR IN pIp,UINT IN uPort,VOID* IN pData,LONG IN lDataLen);

	VOID						Lock();
	VOID						UnLock();
public:
	static VOID CALLBACK		Recv_TimerProc( HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	static UINT					TF_RecvThread(LPVOID IN lp);
protected:
	VOID						ProcessRecv();
	VOID						ProcessPacket();
	BOOL						IsExit();
	CLBS_UDP_Packet*			PopOnePacket();
};



