#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-5-22   9:30
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_CodeBlock.h
	author:		LB
	
	purpose:	´úÂë¿éÀà
*********************************************************************/



class CLBS_CodeBlock :
	public CLBS_CodeBase
{
public:
	CLBS_CodeBlock(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_CodeBlock(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);

};
