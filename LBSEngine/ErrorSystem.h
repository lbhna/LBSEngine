#pragma once
/********************************************************************
	created:	2012-6-21   9:13
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\ErrorSystem.h
	author:		LB
	
	purpose:	错误信息管理系统
*********************************************************************/

#include "ErrorInformation.h"

class CErrorSystem
{
protected:
	static CErrorSystem*	m_pSingleton;
protected:
	CLBS_ObjListBase		m_theErrorList;
public:
	CErrorSystem(void);
	~CErrorSystem(void);
public:
	static CErrorSystem&	GetInstance();
	static VOID				ExitInstance();

public:
	VOID					AddErrorInfo(LPCSTR IN pFileName,DWORD IN dwLineNo,LPCSTR IN pszFormat,...);
	VOID					Clear(){m_theErrorList.Clear();}

	CString					GetErrorInfo();
	DWORD					GetErrorCount(){return m_theErrorList.GetCount();}
	CErrorInformation*		GetError(LONG IN lIndex){return (CErrorInformation*)m_theErrorList.GetObj(lIndex);}

};
