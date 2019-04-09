#include "StdAfx.h"
#include ".\errorinformation.h"


CErrorInformation::CErrorInformation(void)
{
	m_pFileName			=NULL;
	m_pErrorInfo		=NULL;
	m_dwLineNo			=0;
}
CErrorInformation::CErrorInformation(LPCSTR IN pErrorInfo,LPCSTR IN pFileName,DWORD IN dwLineNum)
{
	m_pFileName			=NULL;
	m_pErrorInfo		=NULL;
	m_dwLineNo			=0;
	SetErrorInfo(pErrorInfo);
	SetFileName(pFileName);
	SetLineNum(dwLineNum);
}

CErrorInformation::~CErrorInformation(void)
{
    LB_FREE((LPVoid&)m_pFileName);
    LB_FREE((LPVoid&)m_pErrorInfo);
}

VOID			CErrorInformation::SetErrorInfo(LPCSTR IN pInfo)
{
    LB_FREE((LPVoid&)m_pErrorInfo);
	if(pInfo == NULL)
		return;

	DWORD	len =(DWORD)strlen(pInfo);
	m_pErrorInfo	=(LPSTR)LB_ALLOC(len+1);
	strncpy(m_pErrorInfo,pInfo,len);
	m_pErrorInfo[len]=0;
}
VOID			CErrorInformation::SetFileName(LPCSTR IN pFileName)
{
    LB_FREE((LPVoid&)m_pFileName);
	if(pFileName == NULL)
		return;

	DWORD	len =(DWORD)strlen(pFileName);
	m_pFileName	=(LPSTR)LB_ALLOC(len+1);
	strncpy(m_pFileName,pFileName,len);
	m_pFileName[len]=0;
}
VOID			CErrorInformation::SetLineNum(DWORD IN dwLineNum)
{
	m_dwLineNo	=dwLineNum;
}
VOID			CErrorInformation::OutputString(CString& OUT str)
{
	if(m_pErrorInfo != NULL)
		str+=(LPCSTR)m_pErrorInfo;
}



