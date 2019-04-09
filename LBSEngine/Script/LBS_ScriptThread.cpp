#include "StdAfx.h"
#include "LBS_ScriptThread.h"
#include "LBS_Function.h"

CLBS_ScriptThread::CLBS_ScriptThread(CLBS_ObjBase* IN pThreadFunc)
{
	m_pThreadFunction	=pThreadFunc;
	m_pThreadFunction->AddRef();
	m_dwThreadID		=0;
	m_hThread			=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CLBS_ScriptThread::TF_ScriptThreadFunction,(LPVOID)this,0,&m_dwThreadID);
}

CLBS_ScriptThread::~CLBS_ScriptThread(void)
{
	Terminate();
	LBS_SafeRelease(m_pThreadFunction);

}
VOID				CLBS_ScriptThread::Terminate()
{
	if(m_hThread != NULL)
	{
		TerminateThread(m_hThread,-1);
		CloseHandle(m_hThread);
		m_hThread		=NULL;
		m_dwThreadID	=0;
	}
}
VOID				CLBS_ScriptThread::Suspend()
{
	if(m_hThread != NULL)
		SuspendThread(m_hThread);
}
VOID				CLBS_ScriptThread::Resume()
{
	if(m_hThread != NULL)
		ResumeThread(m_hThread);
}
VOID				CLBS_ScriptThread::Execute()
{
	if(m_pThreadFunction != NULL)
		((CLBS_Function*)m_pThreadFunction)->Execute();
}

UINT				CLBS_ScriptThread::TF_ScriptThreadFunction(LPVOID IN lp)
{
	CLBS_ScriptThread*	pObj =(CLBS_ScriptThread*)lp;
	pObj->Execute();
	return 0;
}
