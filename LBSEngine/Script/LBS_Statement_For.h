#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-5-23   17:08
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_For.h
	author:		LB
	
	purpose:	FOR循环语句
*********************************************************************/



class CLBS_Statement_For :
	public CLBS_CodeBase
{
protected:
	CLBS_CodeBase*			m_pInitCode;	//初始化代码
	CLBS_CodeBase*			m_pConditionCode;//条件代码
	CLBS_CodeBase*			m_pCounterCode;	//
	CLBS_CodeBase*			m_pBodyCode;	//
public:
	CLBS_Statement_For(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_For();
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);

};
