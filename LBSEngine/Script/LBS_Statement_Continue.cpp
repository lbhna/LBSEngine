#include "StdAfx.h"
#include ".\lbs_statement_continue.h"

CLBS_Statement_Continue::CLBS_Statement_Continue(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_CONTINUE_STATEMENT;
	m_bIsContinue		=TRUE;
}

CLBS_Statement_Continue::~CLBS_Statement_Continue(void)
{
}
BOOL			CLBS_Statement_Continue::Execute()
{

	return TRUE;
}
BOOL			CLBS_Statement_Continue::Save(CString& OUT strCode)
{
	return TRUE;
}
