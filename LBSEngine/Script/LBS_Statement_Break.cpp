#include "StdAfx.h"
#include ".\lbs_statement_break.h"

CLBS_Statement_Break::CLBS_Statement_Break(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_BREAK_STATEMENT;
	m_bIsBreak		=TRUE;
}

CLBS_Statement_Break::~CLBS_Statement_Break(void)
{
}
BOOL			CLBS_Statement_Break::Execute()
{
	return TRUE;
}
BOOL			CLBS_Statement_Break::Save(CString& OUT strCode)
{
	return TRUE;
}
