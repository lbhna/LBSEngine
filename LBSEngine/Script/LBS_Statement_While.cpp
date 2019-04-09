#include "StdAfx.h"
#include ".\lbs_statement_while.h"
#include "./LBS_CodeBlock.h"
#include "./LBS_Statement_Calculate.h"



CLBS_Statement_While::CLBS_Statement_While(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_WHILE_STATEMENT;
	m_pWhileCondition	=NULL;
	m_pWhileCode		=NULL;
}

CLBS_Statement_While::~CLBS_Statement_While(void)
{
	LBS_SafeRelease(m_pWhileCondition);
	LBS_SafeRelease(m_pWhileCode);
}

BOOL			CLBS_Statement_While::Execute()
{
	if(m_pWhileCondition == NULL||m_pWhileCode == NULL)
		return FALSE;
	CLBS_Variable*	pCondition;

	SetIsReturn(FALSE);
	m_pWhileCondition->SetCallArea(GetCallArea());
	m_pWhileCode->SetCallArea(GetCallArea());

	while(TRUE)
	{
		if(!m_pWhileCondition->Execute())
			return FALSE;
		pCondition =(CLBS_Variable*)m_pWhileCondition->GetReturnVariable();
		if(pCondition == NULL)
			return FALSE;
		if((BOOL)*pCondition)
		{
			if(!m_pWhileCode->Execute())
				return FALSE;
			if(m_pWhileCode->IsReturn()||m_pWhileCode->IsBreak())
			{	
				SetIsReturn(m_pWhileCode->IsReturn());
				SetReturnVariable(m_pWhileCode->GetReturnVariable());
				break;
			}
		}
		else
		{
			break;
		}
	}
	return TRUE;
}
BOOL			CLBS_Statement_While::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	LPSTR pParsePos		=pCode;
	if(!LoadCondition(pParsePos,pEnd))
		return FALSE;
	if(!LoadBody(pParsePos,pEnd))
		return FALSE;
	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_While::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_While::LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	CHAR				Words[KEY_WORDS_MAX_LEN];
	LPSTR				pWordStart		=NULL;
	PKEYWORDSSTRUCT		pKeywords		=NULL;
	if(!m_pScriptFile->GetNextWords(pCode,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_STATEMENT_WHILE)
		return FALSE;

	if(!m_pScriptFile->GetNextWords(pCode,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;



	LBS_SafeRelease(m_pWhileCondition);
	m_pWhileCondition	=new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END);
	if(!m_pWhileCondition->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pWhileCondition);
		return FALSE;
	}
	return TRUE;
}
BOOL			CLBS_Statement_While::LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LBS_SafeRelease(m_pWhileCode);
	m_pWhileCode	=new CLBS_CodeBlock(this);
	if(!m_pWhileCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pWhileCode);
		return FALSE;
	}
	return TRUE;
}

