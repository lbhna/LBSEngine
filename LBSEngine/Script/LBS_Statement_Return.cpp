#include "StdAfx.h"
#include ".\lbs_statement_return.h"
#include "./LBS_Statement_Calculate.h"



CLBS_Statement_Return::CLBS_Statement_Return(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_RETURN_STATEMENT;
	m_bIsReturn			=TRUE;

}

CLBS_Statement_Return::~CLBS_Statement_Return(void)
{

}
BOOL			CLBS_Statement_Return::Execute()
{
	CLBS_CodeBase*	pCode =(CLBS_CodeBase*)m_theExecuteCodeList[0];
	if(pCode != NULL)
	{
		pCode->SetCallArea(GetCallArea());
		if(pCode->Execute())
		{
			SetReturnVariable(pCode->GetReturnVariable());
			return TRUE;
		}
	}
	return FALSE;
}
BOOL			CLBS_Statement_Return::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;
	LPSTR		pParsePos	=pCode;
	CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(this);
	if(!pStatement->Load(pFile,pParsePos,pEnd))
	{
		pStatement->Release();
		return FALSE;
	}
	
	AddExecuteCode(pStatement);
	pStatement->Release();

	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;


	return TRUE;
}
BOOL			CLBS_Statement_Return::Save(CString& OUT strCode)
{


	return TRUE;
}
