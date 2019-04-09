#include "StdAfx.h"
#include ".\lbs_statement_case.h"
#include "./LBS_CodeBlock.h"
#include "./LBS_ScriptMacroDefine.h"

CLBS_Statement_Case::CLBS_Statement_Case(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_CASE_STATEMENT;
}

CLBS_Statement_Case::~CLBS_Statement_Case(void)
{
}

BOOL			CLBS_Statement_Case::Execute()
{
	if(!CLBS_CodeBase::Execute())
		return FALSE;
	
	
	return TRUE;
}
BOOL			CLBS_Statement_Case::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
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
	if(pKeywords->KeywordsType != KWT_STATEMENT_CASE)
		return FALSE;

	if(!LoadCondition(pParsePos,pEnd))
		return FALSE;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
		return FALSE;
	if(!IsMatchKeywords(Words,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_STATEMENT_CHILD_SPLIT)
		return FALSE;



	if(!LoadBody(pParsePos,pEnd))
		return FALSE;

	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_Case::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_Case::LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	LPSTR				pEndPos		=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];
	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Words))
		return FALSE;
	if(IsMatchKeywords(Words,pKeywords))
	{
		if(pKeywords->KeywordsType == KWT_STRING_SINGLE)
		{
			pEndPos =m_pScriptFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_SINGLE);
			if(pEndPos == NULL)
				return FALSE;
			
			SetName(pParsePos,(LONG)(pEndPos-pParsePos));
			pParsePos =pEndPos+GetKeywords(KWT_STRING_SINGLE)->GetKeywordsLen();
		}
		else if(pKeywords->KeywordsType==KWT_STRING_DOUBLE)
		{
			pEndPos =m_pScriptFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_DOUBLE);
			if(pEndPos == NULL)
				return FALSE;
			SetName(pParsePos,(LONG)(pEndPos-pParsePos));
			pParsePos =pEndPos+GetKeywords(KWT_STRING_DOUBLE)->GetKeywordsLen();
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(!AddCaseConditionByWords(Words))
			return FALSE;
	}
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Statement_Case::LoadBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	CLBS_CodeBlock*		pStatement =new CLBS_CodeBlock(this);
	if(!pStatement->Load(m_pScriptFile,pCode,pEnd))
	{
		LBS_SafeRelease(pStatement);
		return FALSE;
	}
	AddExecuteCode(pStatement);
	LBS_SafeRelease(pStatement);
	return TRUE;
}
BOOL			CLBS_Statement_Case::AddCaseConditionByWords(LPCSTR IN pWord)
{
	LPSTR	pRealWords					=(LPSTR)pWord;
	CLBS_ScriptMacroDefine* pMacro		=(CLBS_ScriptMacroDefine*)m_pScriptFile->GetParser()->GetMacro(pRealWords);
	while(pMacro != NULL)
	{
		pRealWords	=(LPSTR)pMacro->GetValue();
		if(pMacro->IsString())
			break;
		pMacro	=(CLBS_ScriptMacroDefine*)m_pScriptFile->GetParser()->GetMacro(pRealWords);
	}
	if(pMacro != NULL)
	{//找到了该宏
		if(pMacro->IsString())
		{//该宏是个字串
			SetName(pRealWords);
			return TRUE;
		}	
	}

	//先与关键字匹配
	PKEYWORDSSTRUCT			pKeywords=NULL;
	if(IsMatchKeywords(pRealWords,pKeywords))
	{//是关键字
		switch(pKeywords->KeywordsType)
		{
		case KWT_CONST_TRUE:
			{
				SetName("1");
				return TRUE;
			}
			break;
		case KWT_CONST_FALSE:
			{
				SetName("0");
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	CHAR					name[64];
	switch(GetStringType(pRealWords))
	{///返回-1 无效，0：字串，1：10进制整数  2: 浮点数 3：16进制整数
	case 0:
		break;
	case 1:
		{
			LONG	value		=atoll(pRealWords);
			sprintf(name,"%d",value);
			SetName(name);
			return TRUE;
		}
		break;
	case 2:
		{
			DOUBLE	value		=atof(pRealWords);
			sprintf(name,"%f",value);
			SetName(name);
			return TRUE;
		}
		break;
	case 3:
		{
			DWORD	value		=0;
			sscanf(pRealWords,"%x",&value);
			sprintf(name,"%d",value);
			SetName(name);
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}
