#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************
	created:	2012-5-29   15:12
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Break.h
	author:		LB
	
	purpose:	—≠ª∑÷–∂œ”Ôæ‰
*********************************************************************/


class CLBS_Statement_Break :
	public CLBS_CodeBase
{
public:
	CLBS_Statement_Break(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Break(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Save(CString& OUT strCode);
};
