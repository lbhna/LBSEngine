#include "StdAfx.h"
#include ".\lbs_function_call.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Function.h"
#include "./LBS_Variable.h"



CLBS_Function_Call::CLBS_Function_Call(CLBS_CodeBase* IN pParent,CLBS_CodeBase* IN pCallArea):
CLBS_CodeBase(pParent)
{

	m_Type			=LOT_CODE_FUNC_CALL;
	m_pCallArea		=pCallArea;
}

CLBS_Function_Call::~CLBS_Function_Call(void)
{
}
BOOL			CLBS_Function_Call::Execute()
{
	CLBS_Function*	pFunc	=(CLBS_Function*)GetFunction(GetName());
	if(pFunc == NULL)
	{
		ShowDebugTrace("#错误：函数[ %s ]调用失败！未声明的函数调用!",GetName());
		return FALSE;
	}

	CLBS_Variable*	pVar	=NULL;
	CLBS_Variable*	pRetVar =NULL;
	CLBS_CodeBase*	pCode	=NULL;

	switch(pFunc->GetFunctionType())
	{
	case LFT_APP_FUNCTION:
	case LFT_SYS_FUNCTION:
		{
			pFunc->ClearParameterVariable();
			pFunc->SetCallArea(GetParent());
			for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
			{
				pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj((m_theExecuteCodeList.GetCount()-1)-i);
				if(pCode == NULL)
					return FALSE;
				pCode->SetCallArea(GetCallArea());
				if(!pCode->Execute())
					return FALSE;
				pRetVar	= (CLBS_Variable*)pCode->GetReturnVariable();
				if(pRetVar == NULL)
					return FALSE;

				pFunc->AddParameterVariable(pRetVar);

			}
		}
		break;
	case LFT_USER_FUNCTION:
		{
			pFunc->SetCallArea(GetCallArea());
			if(pFunc->GetParameterVariableCount() != m_theExecuteCodeList.GetCount())
			{
				ShowDebugTrace("#错误：函数[ %s ]调用失败！传入参数数量不正确.",GetName());
				return FALSE;
			}
			for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
			{
				pVar =(CLBS_Variable*)pFunc->GetParameterVariable((m_theExecuteCodeList.GetCount()-1)-i);
				if(pVar == NULL)
					return FALSE;
                pVar->Reset();
				pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj(i);
				if(pCode == NULL)
					return FALSE;
				pCode->SetCallArea(GetCallArea());
				if(!pCode->Execute())
					return FALSE;

				pRetVar	= (CLBS_Variable*)pCode->GetReturnVariable();
				if(pRetVar == NULL)
					return FALSE;
				
				*pVar = *pRetVar;
				pVar->SetRefVaraibleInfo(0,pRetVar);
			}
		}
		break;
	case LFT_MEMBER_FUNCTION:
		{
			pFunc->ClearParameterVariable();
			pFunc->SetCallArea(GetCallArea());
			for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
			{
				pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj((m_theExecuteCodeList.GetCount()-1)-i);
				if(pCode == NULL)
					return FALSE;
				pCode->SetCallArea(GetCallArea());
				if(!pCode->Execute())
					return FALSE;
				pRetVar	= (CLBS_Variable*)pCode->GetReturnVariable();
				if(pRetVar == NULL)
					return FALSE;
				pFunc->AddParameterVariable(pRetVar);
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}



	LBS_SafeRelease(m_pReturnVariable);
	if(!pFunc->Execute())
	{
		ShowDebugTrace("#错误：函数[ %s ] 执行失败！",pFunc->GetName());
		return FALSE;
	}
	m_pReturnVariable	=pFunc->GetReturnVariable();
	if(m_pReturnVariable != NULL)
		m_pReturnVariable->AddRef();

	return TRUE;
}
BOOL			CLBS_Function_Call::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
    LB_FREE((LPVoid&)m_pName);
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	CHAR				Word[KEY_WORDS_MAX_LEN];
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(IsMatchKeywords(Word,pKeywords))
		return FALSE;
	SetName(Word);//得到函数名

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(!IsMatchKeywords(Word,pKeywords))
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;

	//开始载入参数列表
	CLBS_Statement_Calculate*	pPara	=new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END,m_pCallArea);
	if(!pPara->Load(m_pScriptFile,pParsePos,pEnd))
	{
		pPara->Release();
		return FALSE;
	}
	if(pPara->GetFactorCount()>0)
		AddExecuteCode(pPara);
	pPara->Release();
	
	SetOriginCode(pCode,(LONG)(pParsePos-pCode));
	pCode =pParsePos;
	return TRUE;
}
BOOL			CLBS_Function_Call::Save(CString& OUT strCode)
{
	return TRUE;
}
