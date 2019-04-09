#pragma once
#include "./Base/LBS_ObjBase.h"
/********************************************************************
	created:	2012-6-21   9:18
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\ErrorInformation.h
	author:		LB
	
	purpose:	¥ÌŒÛ–≈œ¢¿‡
*********************************************************************/


class CErrorInformation :
	public CLBS_ObjBase
{
protected:
	LPSTR			m_pErrorInfo;
	LPSTR			m_pFileName;
	DWORD			m_dwLineNo;
public:
	CErrorInformation(void);
	CErrorInformation(LPCSTR IN pErrorInfo,LPCSTR IN pFileName=NULL,DWORD IN dwLineNum=0);
	virtual ~CErrorInformation(void);
public:
	LPCSTR			GetErrorInfo(){return m_pErrorInfo;}
	LPCSTR			GetFileName(){return m_pFileName;}
	DWORD			GetLineNum(){return m_dwLineNo;}

	VOID			SetErrorInfo(LPCSTR IN pInfo);
	VOID			SetFileName(LPCSTR IN pFileName);
	VOID			SetLineNum(DWORD IN dwLineNum);

	VOID			OutputString(CString& OUT str);
};

