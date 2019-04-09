#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-6-6   14:37
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Continue.h
	author:		LB
	
	purpose:	continue Óï¾ä
*********************************************************************/

class CLBS_Statement_Continue :
	public CLBS_CodeBase
{
public:
	CLBS_Statement_Continue(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Continue(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Save(CString& OUT strCode);

};
