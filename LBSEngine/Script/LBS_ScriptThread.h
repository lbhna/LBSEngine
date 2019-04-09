#pragma once
#include "../Base/LBS_ObjBase.h"
/********************************************************************\
	created:	2013/01/24   9:24
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_ScriptThread.h
	author:		LB
	purpose:	脚本中的线程对象
\*********************************************************************/



class CLBS_ScriptThread:
	public CLBS_ObjBase
{
protected:
	CLBS_ObjBase*			m_pThreadFunction;
	HANDLE					m_hThread;
	DWORD					m_dwThreadID;

public:
	CLBS_ScriptThread(CLBS_ObjBase* IN pThreadFunc);
	~CLBS_ScriptThread(void);
public:
	VOID					Terminate();
	VOID					Suspend();
	VOID					Resume();

	HANDLE					GetThreadHandle(){return m_hThread;}
	DWORD					GetThreadID(){return m_dwThreadID;}
protected:
	VOID					Execute();
	static UINT				TF_ScriptThreadFunction(LPVOID IN lp);
};
