#pragma once
#include "../Base/LBS_ObjBase.h"
/********************************************************************
	created:	2012-6-5   11:19
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_Timer.h
	author:		LB
	
	purpose:	脚本定时器类
*********************************************************************/




class CLBS_Script_Timer :
	public CLBS_ObjBase
{
protected:
	DWORD						m_dwInterval;
	CLBS_ObjBase*				m_pTimerFunc;
	UINT_PTR					m_uTimerID;

public:
	CLBS_Script_Timer(CLBS_ObjBase* IN pTimerFunc,DWORD IN dwInterval);
	virtual ~CLBS_Script_Timer(void);
public:
	UINT_PTR					GetID(){return m_uTimerID;}
public:
	static VOID CALLBACK		Script_TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
protected:
	BOOL						Execute();
};


