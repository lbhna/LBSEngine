#include "StdAfx.h"
#include ".\lbs_statement_ifelse.h"
#include "./LBS_CodeBlock.h"
#include "./LBS_Statement_Calculate.h"



CLBS_Statement_Ifelse::CLBS_Statement_Ifelse(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_IFELSE_STATEMENT;

	m_pIfCondition	=NULL;
	m_pIfCode		=NULL;		//if	下面的子句
	m_pElseCode		=NULL;		//else	下面的子句
}

CLBS_Statement_Ifelse::~CLBS_Statement_Ifelse(void)
{
	LBS_SafeRelease(m_pIfCondition);
	LBS_SafeRelease(m_pIfCode);
	LBS_SafeRelease(m_pElseCode);

}
BOOL			CLBS_Statement_Ifelse::Execute()
{
	SetIsReturn(FALSE);
	SetIsBreak(FALSE);
	SetIsContinue(FALSE);

	if(m_pIfCondition == NULL)
		return FALSE;
	m_pIfCondition->SetCallArea(GetCallArea());
	if(!m_pIfCondition->Execute())
		return FALSE;
	CLBS_Variable*	pRet =(CLBS_Variable*)m_pIfCondition->GetReturnVariable();
	if(pRet == NULL)
		return FALSE;
	if((BOOL)*pRet)
	{
		if(m_pIfCode == NULL)
			return FALSE;
		m_pIfCode->SetCallArea(GetCallArea());
		if(!m_pIfCode->Execute())
			return FALSE;

		SetReturnVariable(m_pIfCode->GetReturnVariable());

		SetIsReturn(m_pIfCode->IsReturn());
		SetIsBreak(m_pIfCode->IsBreak());
		SetIsContinue(m_pIfCode->IsContinue());

	}
	else
	{
		if(m_pElseCode != NULL)
		{
			m_pElseCode->SetCallArea(GetCallArea());
			if(!m_pElseCode->Execute())
				return FALSE;
		
			SetReturnVariable(m_pElseCode->GetReturnVariable());

			SetIsReturn(m_pIfCode->IsReturn());
			SetIsBreak(m_pIfCode->IsBreak());
			SetIsContinue(m_pIfCode->IsContinue());
		}
	}

	return TRUE;
}
BOOL			CLBS_Statement_Ifelse::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;
	LPSTR pParsePos =pCode;

	if(!LoadCondition(pParsePos,pEnd))
		return FALSE;
	if(!LoadIfBody(pParsePos,pEnd))
		return FALSE;
	if(!LoadElseBody(pParsePos,pEnd))
		return FALSE;
	
	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_Ifelse::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_Ifelse::LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	CHAR				Words[KEY_WORDS_MAX_LEN];
	LPSTR				pWordStart		=NULL;
	PKEYWORDSSTRUCT		pKeywords		=NULL;
	
	if(!m_pScriptFile->GetNextWords(pCode,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_STATEMENT_IF)
		return FALSE;
	
	if(!m_pScriptFile->GetNextWords(pCode,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;

	LBS_SafeRelease(m_pIfCondition);
	m_pIfCondition	=new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END);
	if(!m_pIfCondition->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pIfCondition);
		return FALSE;
	}

	return TRUE;
}
BOOL			CLBS_Statement_Ifelse::LoadIfBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LBS_SafeRelease(m_pIfCode);
	m_pIfCode	=new CLBS_CodeBlock(this);
	if(!m_pIfCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pIfCode);
		return FALSE;
	}
	return TRUE;
}
BOOL			CLBS_Statement_Ifelse::LoadElseBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LPSTR				pParsePos		=pCode;
	LPSTR				pWordsStart		=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];
	PKEYWORDSSTRUCT		pKeywords	=NULL;

	//{{-------------------------------
	//没有ELSE 语句，仍然返回TRUE
	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
		return TRUE;
	if(!IsMatchKeywords(Words,pKeywords))
		return TRUE;
	if(pKeywords->KeywordsType !=KWT_STATEMENT_ELSE)
		return TRUE;
	//}}-------------------------------

	pCode =pParsePos;

	//有ELSE语句
	LBS_SafeRelease(m_pElseCode);
	m_pElseCode	=new CLBS_CodeBlock(this);
	if(!m_pElseCode->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pElseCode);
		return FALSE;
	}
	return TRUE;
}
