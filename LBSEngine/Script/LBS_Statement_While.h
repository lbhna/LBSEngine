#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-5-23   8:54
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_While.h
	author:		LB
	
	purpose:	while 语句
*********************************************************************/


class CLBS_Statement_While :
	public CLBS_CodeBase
{
protected:

	CLBS_CodeBase*			m_pWhileCondition;	//循环条件语句
	CLBS_CodeBase*			m_pWhileCode;		//循环代码

public:
	CLBS_Statement_While(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_While(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);
};
