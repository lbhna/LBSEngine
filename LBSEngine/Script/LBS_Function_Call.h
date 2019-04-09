#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Function_Call.h
	created:	2012/05/11
	author:		LB
	purpose:	º¯Êýµ÷ÓÃÓï¾ä
\*********************************************************************/


class CLBS_Function_Call :
	public CLBS_CodeBase
{
public:
	CLBS_Function_Call(CLBS_CodeBase* IN pParent,CLBS_CodeBase* IN pCallArea);
	virtual ~CLBS_Function_Call(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
};
