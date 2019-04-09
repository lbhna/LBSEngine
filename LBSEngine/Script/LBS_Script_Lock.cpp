#include "StdAfx.h"
#include ".\lbs_script_lock.h"

CLBS_Script_Lock::CLBS_Script_Lock(void)
{
	InitializeCriticalSection(&m_hSection);
}

CLBS_Script_Lock::~CLBS_Script_Lock(void)
{
	DeleteCriticalSection(&m_hSection);
}



CLBS_Script_Event::CLBS_Script_Event(void)
{
	m_hEvent	=CreateEvent(NULL,FALSE,TRUE,NULL);
}

CLBS_Script_Event::~CLBS_Script_Event(void)
{
	CloseHandle(m_hEvent);
	m_hEvent	=NULL;
}
