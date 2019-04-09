#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-6-1   14:40
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Switch.h
	author:		LB
	
	purpose:	switch语句
*********************************************************************/



class CLBS_Statement_Switch :
	public CLBS_CodeBase
{
protected:
	CLBS_ObjHashListBase*	m_pCaseStatementList;//case语句映射表
	CLBS_CodeBase*			m_pSwitchCondition;	//switch中的条件语句
	CLBS_CodeBase*			m_pDefaultStatement;//默认语句

public:
	CLBS_Statement_Switch(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Switch(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					AddCaseStatement(CLBS_CodeBase* IN pStatement);
};
