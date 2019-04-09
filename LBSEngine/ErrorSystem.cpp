#include "StdAfx.h"
#include ".\errorsystem.h"

CErrorSystem*	CErrorSystem::m_pSingleton	=NULL;
CErrorSystem::CErrorSystem(void)
{
}

CErrorSystem::~CErrorSystem(void)
{
	Clear();
}
CErrorSystem&			CErrorSystem::GetInstance()
{
	if(CErrorSystem::m_pSingleton == NULL)
	{
		CErrorSystem::m_pSingleton	=new CErrorSystem();
	}
	return *CErrorSystem::m_pSingleton;
}
VOID					CErrorSystem::ExitInstance()
{
	LBS_SafeDelete(CErrorSystem::m_pSingleton);
}
VOID					CErrorSystem::AddErrorInfo(LPCSTR IN pFileName,DWORD IN dwLineNo,LPCSTR IN pszFormat,...)
{

	CString str;
	va_list argList;
	va_start( argList, pszFormat );
	str.FormatV( pszFormat, argList );
	va_end( argList );
	str+="\r\n";
	OutputDebugString(str);

	CErrorInformation*		pNewInfo =new CErrorInformation(str,pFileName,dwLineNo);
	m_theErrorList.AddObj(pNewInfo);
	pNewInfo->Release();
}
CString					CErrorSystem::GetErrorInfo()
{
	CString				info=_T("");
	CErrorInformation*	pInfo;
	for(LONG i=0;i<m_theErrorList.GetCount();i++)
	{
		pInfo =(CErrorInformation*)m_theErrorList.GetObj(i);
		pInfo->OutputString(info);
	}
	return info;
}
