#pragma once
#include "../Base/LBS_ObjBase.h"
/********************************************************************\
	created:	2013/01/24   9:48
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_Lock.h
	author:		LB
	purpose:	多线程同步锁
\*********************************************************************/


class CLBS_Script_Lock :
	public CLBS_ObjBase
{
protected:
	CRITICAL_SECTION			m_hSection;

public:
	CLBS_Script_Lock(void);
	virtual ~CLBS_Script_Lock(void);
public:
	VOID				Lock()	{EnterCriticalSection(&m_hSection);}
	VOID				Unlock(){LeaveCriticalSection(&m_hSection);}
};
class CLBS_Script_Event :
	public CLBS_ObjBase
{
protected:
	HANDLE			m_hEvent;

public:
	CLBS_Script_Event(void);
	virtual ~CLBS_Script_Event(void);
public:
	VOID				setEvent()	{SetEvent(m_hEvent);}
	VOID				waitEvent()	{WaitForSingleObject(m_hEvent,INFINITE);}
};
