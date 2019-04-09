#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-6-6   14:39
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Return.h
	author:		LB
	
	purpose:	∑µªÿ”Ôæ‰
*********************************************************************/

class CLBS_Statement_Return :
	public CLBS_CodeBase
{
public:
	CLBS_Statement_Return(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Return(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
};
