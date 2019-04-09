#include "StdAfx.h"
#include ".\lbs_scriptmacrodefine.h"

CLBS_ScriptMacroDefine::CLBS_ScriptMacroDefine(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_MACRO;
	m_pMacroValue		=NULL;
	m_bIsString			=FALSE;
}

CLBS_ScriptMacroDefine::~CLBS_ScriptMacroDefine(void)
{
    LB_FREE((LPVoid&)m_pMacroValue);
}
BOOL			CLBS_ScriptMacroDefine::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
    LB_FREE((LPVoid&)m_pMacroValue);
	m_bIsString		=FALSE;
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	PKEYWORDSSTRUCT		pKeywords	=NULL;
	LPSTR				pValidPos	=pFile->SeekValid(pCode,pEnd);
	LPSTR				pParsePos	=pValidPos;
	if(pValidPos == NULL)
		return FALSE;
	LPSTR				pSepPos		=pFile->GetNextSeperatorPosition(pParsePos,pEnd,pKeywords);
	if(pSepPos == NULL)
		return FALSE;
	LONG				len			=(LONG)(pSepPos - pParsePos);
	if(len <= 0)
		return FALSE;

	m_pName	=(LPSTR)LB_ALLOC(len+1);
	strncpy(m_pName,pParsePos,len);
	m_pName[len]=0;

	if(IsMatchKeywords(m_pName,pKeywords))
	{
		ShowDebugTrace("#关键字[ %s ]为保留字，不能定义为宏！",m_pName);
		return FALSE;
	}


	pParsePos  = pSepPos;
	//------------------------------------

	pValidPos	=pFile->SeekValid(pParsePos,pEnd);
	pParsePos	=pValidPos;
	LPSTR				pKeyPos		=pFile->GetNextValidKeywordsPosition(pParsePos,pEnd,pKeywords);
	if(pKeyPos != pValidPos)
	{
		LPSTR pCarriage =pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_CARRIAGE);
		LPSTR pReturn	=pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_RETURN);
		
		pKeyPos =NULL;
		if(pCarriage != NULL)
		{
			pKeyPos =pCarriage;
			if(pReturn != NULL)
				pKeyPos		=min(pKeyPos,pReturn);
		}
		else if(pReturn != NULL)
		{
			pKeyPos		=pReturn;
		}

		if(pKeyPos == NULL)
			return FALSE;
		pCode		=pKeyPos;
	}
	else
	{
		if(pKeywords->KeywordsType == KWT_STRING_DOUBLE)
		{
			pParsePos += pKeywords->GetKeywordsLen();
			pKeyPos		=pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_DOUBLE);
			if(pKeyPos == NULL)
				return FALSE;
			pCode		=pKeyPos + GetKeywords(KWT_STRING_DOUBLE)->GetKeywordsLen();
			m_bIsString =TRUE;
		}
		else if(pKeywords->KeywordsType == KWT_STRING_SINGLE)
		{
			pParsePos += pKeywords->GetKeywordsLen();
			pKeyPos		=pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_SINGLE);
			if(pKeyPos == NULL)
				return FALSE;
			pCode		=pKeyPos + GetKeywords(KWT_STRING_SINGLE)->GetKeywordsLen();
			m_bIsString =TRUE;
		}
		else if(pKeywords->KeywordsType == KWT_PARA_LIST_START)
		{
			pKeyPos =pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_RETURN);
			pCode		=pKeyPos + GetKeywords(KWT_STRING_SINGLE)->GetKeywordsLen();

		}
		else
		{//直接关键字宏定义
			pKeyPos = pParsePos + pKeywords->GetKeywordsLen();
			pCode	=pKeyPos;
		}
	}

	len		=(LONG)(pKeyPos - pParsePos);
	if(len <= 0)
		return FALSE;

	m_pMacroValue	=(LPSTR)LB_ALLOC(len+1);
	strncpy(m_pMacroValue,pParsePos,len);
	m_pMacroValue[len]=0;




	return TRUE;
}
BOOL			CLBS_ScriptMacroDefine::Save(CString& OUT strCode)
{

	return TRUE;
}
