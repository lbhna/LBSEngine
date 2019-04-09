#include "StdAfx.h"
#include ".\lbs_script_timer.h"
#include "./LBS_Function.h"
#include "./LBS_EventWnd.h"

CLBS_Script_Timer::CLBS_Script_Timer(CLBS_ObjBase* IN pTimerFunc,DWORD IN dwInterval)
{
	m_Type			=LOT_SCRIPT_TIMER;
	m_pTimerFunc	=pTimerFunc;
	m_pTimerFunc->AddRef();
	m_dwInterval	=dwInterval;
	m_uTimerID		=SetTimer(CLBS_EventWnd::GetInstance().GetWnd(),(UINT_PTR)this,dwInterval,CLBS_Script_Timer::Script_TimerProc);
}

CLBS_Script_Timer::~CLBS_Script_Timer(void)
{
	KillTimer(CLBS_EventWnd::GetInstance().GetWnd(),m_uTimerID);
	LBS_SafeRelease(m_pTimerFunc);
}
BOOL					CLBS_Script_Timer::Execute()
{
	CLBS_Function*		pFunc =(CLBS_Function*)m_pTimerFunc;
	if(pFunc == NULL)
		return FALSE;
	return pFunc->Execute();
}
VOID CALLBACK			CLBS_Script_Timer::Script_TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	CLBS_Script_Timer*	pTimer =(CLBS_Script_Timer*)idEvent;
	if(pTimer != NULL)
	{
		pTimer->Execute();
	}
}