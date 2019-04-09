#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-6-6   14:37
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Case.h
	author:		LB
	
	purpose:	case Óï¾ä
*********************************************************************/


class CLBS_Statement_Case :
	public CLBS_CodeBase
{
public:
	CLBS_Statement_Case(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Case(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					AddCaseConditionByWords(LPCSTR IN pWord);
};
