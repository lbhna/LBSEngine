#include "StdAfx.h"
#include ".\lbs_statement_for.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_CodeBlock.h"

CLBS_Statement_For::CLBS_Statement_For(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_FOR_STATEMENT;
	m_pInitCode			=NULL;	//初始化代码
	m_pConditionCode	=NULL;//条件代码
	m_pCounterCode		=NULL;	//
	m_pBodyCode			=NULL;	//

}

CLBS_Statement_For::~CLBS_Statement_For()
{
	LBS_SafeRelease(m_pInitCode);
	LBS_SafeRelease(m_pConditionCode);
	LBS_SafeRelease(m_pCounterCode);
	LBS_SafeRelease(m_pBodyCode);
}
BOOL			CLBS_Statement_For::Execute()
{

	if(m_pConditionCode == NULL)
		return FALSE;
	SetIsReturn(FALSE);
	if(m_pInitCode != NULL)
	{
		m_pInitCode->SetCallArea(GetCallArea());
		if(!m_pInitCode->Execute())
			return FALSE;
	}

	m_pConditionCode->SetCallArea(GetCallArea());
	if(m_pBodyCode != NULL)
		m_pBodyCode->SetCallArea(GetCallArea());
	if(m_pCounterCode != NULL)
		m_pCounterCode->SetCallArea(GetCallArea());

	CLBS_Variable*	pRet;
	while(TRUE)
	{
		if(!m_pConditionCode->Execute())
			return FALSE;
		pRet =(CLBS_Variable*)m_pConditionCode->GetReturnVariable();
		if(pRet == NULL)
			return FALSE;
		if((BOOL)*pRet)
		{
			if(m_pBodyCode != NULL)
			{
				if(!m_pBodyCode->Execute())
					return FALSE;
				if(m_pBodyCode->IsReturn()||m_pBodyCode->IsBreak())
				{
					SetIsReturn(m_pBodyCode->IsReturn());
					SetReturnVariable(m_pBodyCode->GetReturnVariable());
					break;
				}
			}
		}
		else
		{
			break;
		}

		if(m_pCounterCode != NULL)
		{
			if(!m_pCounterCode->Execute())
				return FALSE;
		}
	}
	return TRUE;
}
BOOL			CLBS_Statement_For::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	CHAR				Word[KEY_WORDS_MAX_LEN];
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	
	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(!IsMatchKeywords(Word,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_STATEMENT_FOR)
		return FALSE;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(!IsMatchKeywords(Word,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;

	pCode =pParsePos;
	if(!LoadCondition(pParsePos,pEnd))
		return FALSE;

	if(!LoadBody(pParsePos,pEnd))
		return FALSE;
	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_For::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_For::LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	CHAR				Word[KEY_WORDS_MAX_LEN];
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	
	BOOL	bVarDeclare =FALSE;
	if(IsMatchKeywords(Word,pKeywords))
	{
		if(pKeywords->KeywordsType == KWT_VARIABLE_DEFINE)
		{
			pCode		=pParsePos;
			bVarDeclare =TRUE;
		}
	}


	LBS_SafeRelease(m_pInitCode);
	m_pInitCode		=new CLBS_Statement_Calculate(this,bVarDeclare);
	if(!m_pInitCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pInitCode);
		return FALSE;
	}
	
	LBS_SafeRelease(m_pConditionCode);
	m_pConditionCode		=new CLBS_Statement_Calculate(this);
	if(!m_pConditionCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pConditionCode);
		return FALSE;
	}

	LBS_SafeRelease(m_pCounterCode);
	m_pCounterCode		=new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END);
	if(!m_pCounterCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pCounterCode);
		return FALSE;
	}
	

	return TRUE;
}
BOOL			CLBS_Statement_For::LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LBS_SafeRelease(m_pBodyCode);
	m_pBodyCode		=new CLBS_CodeBlock(this);
	if(!m_pBodyCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pBodyCode);
		return FALSE;
	}

	return TRUE;
}
