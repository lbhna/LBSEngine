// LBS_Socket.cpp : implementation file
//

#include "stdafx.h"
#include "LBS_Socket.h"
#include "./LBS_Script_TCP.h"

// CLBS_Socket

CLBS_Socket::CLBS_Socket(CLBS_Script_TCP* IN pTCP)
{
	m_pTCP			=pTCP;
}

CLBS_Socket::~CLBS_Socket()
{
}
// CLBS_Socket member functions
void CLBS_Socket::OnAccept(int nErrorCode)
{
	m_pTCP->OnAccept(this);
	CSocket::OnAccept(nErrorCode);
}
void CLBS_Socket::OnClose(int nErrorCode)
{
	m_pTCP->OnClose(this);
	CSocket::OnClose(nErrorCode);
}
void CLBS_Socket::OnReceive(int nErrorCode)
{
	m_pTCP->OnReceive(this);
	CSocket::OnReceive(nErrorCode);
}
void CLBS_Socket::OnSend(int nErrorCode)
{
	m_pTCP->OnSend(this);
	CSocket::OnSend(nErrorCode);
}
