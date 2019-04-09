#include "stdafx.h"
#include "LBS_EventWnd.h"


CLBS_EventWnd* CLBS_EventWnd::m_pSingleton	=NULL;
CLBS_EventWnd::CLBS_EventWnd()
{
	m_pEventWnd		=new CWnd();
	m_pEventWnd->CreateEx(0, AfxRegisterWndClass(0),_T("LBScript Notification Window"),WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL);
}
CLBS_EventWnd::~CLBS_EventWnd()
{
	if(m_pEventWnd != NULL)
	{
		m_pEventWnd->DestroyWindow();
		delete m_pEventWnd;
		m_pEventWnd=NULL;
	}
}

CLBS_EventWnd&			CLBS_EventWnd::GetInstance()
{
	if(CLBS_EventWnd::m_pSingleton == NULL)
		CLBS_EventWnd::m_pSingleton=new CLBS_EventWnd();
	return *CLBS_EventWnd::m_pSingleton;
}
void							CLBS_EventWnd::ExitInstance()
{
	if(CLBS_EventWnd::m_pSingleton == NULL)
		return;
	delete CLBS_EventWnd::m_pSingleton;
	CLBS_EventWnd::m_pSingleton=NULL;
}
HWND							CLBS_EventWnd::GetWnd()
{
	return m_pEventWnd->GetSafeHwnd();
}
