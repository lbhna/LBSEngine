#include "StdAfx.h"
#include ".\lbs_statement_switch.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Statement_Case.h"
#include "./LBS_CodeBlock.h"
#include "./LBS_ApiImplement.h"

CLBS_Statement_Switch::CLBS_Statement_Switch(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_SWITCH_STATEMENT;

	m_pCaseStatementList=NULL;
	m_pDefaultStatement =NULL;
	m_pSwitchCondition	=NULL;

}

CLBS_Statement_Switch::~CLBS_Statement_Switch(void)
{
	LBS_SafeRelease(m_pSwitchCondition);
	LBS_SafeRelease(m_pDefaultStatement);
	LBS_SafeRelease(m_pCaseStatementList);
}
BOOL			CLBS_Statement_Switch::Execute()
{
	if(m_pSwitchCondition == NULL )
		return FALSE;
	SetIsReturn(FALSE);
	SetIsBreak(FALSE);
	SetIsContinue(FALSE);

	m_pSwitchCondition->SetCallArea(GetCallArea());
	if(!m_pSwitchCondition->Execute())
		return FALSE;
	CLBS_Variable*	pCondition	=(CLBS_Variable*)m_pSwitchCondition->GetReturnVariable();
	
	DWORD	code;
	CLBS_ObjListBase	paraList;
	pCondition					=(CLBS_Variable*)CLBS_ApiImplement::var_toString(paraList,pCondition,code);
	
	if(code != 0 || pCondition == NULL)
		return FALSE;

	CLBS_CodeBase*		pStatement =NULL;
	if(m_pCaseStatementList != NULL)
		pStatement =(CLBS_CodeBase*)m_pCaseStatementList->GetObj((LPCSTR)*pCondition);
	LBS_SafeRelease(pCondition);

	if(pStatement == NULL)
	{
		pStatement =m_pDefaultStatement;
	}

	if(pStatement != NULL)
	{
		pStatement->SetCallArea(GetCallArea());
		if(!pStatement->Execute())
			return FALSE;
		SetReturnVariable(pStatement->GetReturnVariable());
		SetIsReturn(pStatement->IsReturn());
	}

	return TRUE;
}
BOOL			CLBS_Statement_Switch::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];
	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_STATEMENT_SWITCH)
		return FALSE;
	
	if(!LoadCondition(pParsePos,pEnd))
		return FALSE;
	if(!LoadBody(pParsePos,pEnd))
		return FALSE;

	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_Switch::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_Switch::LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{

	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];
	if(!m_pScriptFile->GetNextWords(pCode,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;



	LBS_SafeRelease(m_pSwitchCondition);
	m_pSwitchCondition	=new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END);
	if(!m_pSwitchCondition->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(m_pSwitchCondition);
		return FALSE;
	}
	return TRUE;
}
BOOL			CLBS_Statement_Switch::LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LBS_SafeRelease(m_pDefaultStatement);
	LBS_SafeRelease(m_pCaseStatementList);

	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];


	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType !=KWT_CODE_BLOCK_START)
		return FALSE;
	while(m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
	{
		if(IsMatchKeywords(Words,pKeywords))
		{
			switch(pKeywords->KeywordsType)
			{
			case KWT_CODE_BLOCK_END:
				{//ÔØÈëÍê³É
					pCode =pParsePos;
					return TRUE;
				}
				break;
			case KWT_STATEMENT_CASE:
				{
					pParsePos =pWordStart;
					CLBS_Statement_Case*	pStatement =new CLBS_Statement_Case(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						LBS_SafeRelease(pStatement);
						return FALSE;
					}
					if(!AddCaseStatement(pStatement))
					{
						LBS_SafeRelease(pStatement);
						return FALSE;
					}
					LBS_SafeRelease(pStatement);
				}
				break;
			case KWT_STATEMENT_DEFAULT:
				{
					if(m_pDefaultStatement != NULL)
						return FALSE;

					if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
						return FALSE;
					if(!IsMatchKeywords(Words,pKeywords))
						return FALSE;
					if(pKeywords->KeywordsType !=KWT_STATEMENT_CHILD_SPLIT)
						return FALSE;

					m_pDefaultStatement		=new CLBS_CodeBlock(this);
					if(!m_pDefaultStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						LBS_SafeRelease(m_pDefaultStatement);
						return FALSE;
					}
				}
				break;
			case KWT_BREAK:
			case KWT_END_LINE:
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;
}
BOOL					CLBS_Statement_Switch::AddCaseStatement(CLBS_CodeBase* IN pStatement)
{
	if(pStatement == NULL)
		return FALSE;
	if(m_pCaseStatementList == NULL)
	{
		m_pCaseStatementList =new CLBS_ObjHashListBase(50);
	}

	if(m_pCaseStatementList->GetObj(pStatement->GetName()) != NULL)
		return FALSE;
	m_pCaseStatementList->AddObj(pStatement);
	return TRUE;
}
