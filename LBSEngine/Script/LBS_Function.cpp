#include "StdAfx.h"
#include ".\lbs_function.h"
#include "LBS_Variable.h"
#include "LBS_CodeBlock.h"

CLBS_Function::CLBS_Function(CLBS_CodeBase* IN pParent,LBS_FUNCTION_TYPE IN funcType,LBS_API_FUNCTION IN pSysFuncAddr):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_FUNCTION;
	m_FunctionType	=funcType;
	m_pSysFuncAddr  =pSysFuncAddr;
	m_pOriginBodyCode=NULL;
}

CLBS_Function::~CLBS_Function(void)
{
	m_theParameterVariableList.Clear();
    LB_FREE((LPVoid&)m_pOriginBodyCode);
}
VOID			CLBS_Function::SetOriginBodyCode(LPSTR IN pCode,LONG IN lSize)
{
    LB_FREE((LPVoid&)m_pOriginBodyCode);
	if(lSize<=0 || pCode==NULL)
		return;
	m_pOriginBodyCode	=(LPSTR)LB_ALLOC(lSize+1);
	strncpy(m_pOriginBodyCode,pCode,lSize);
	m_pOriginBodyCode[lSize]=0;
}
CLBS_ObjBase*	CLBS_Function::GetMemberVariable(LPCSTR IN pVarName)
{
	CLBS_ObjBase*	pVar	=m_theParameterVariableList.FindObj(pVarName);
	if(pVar == NULL)
	{
		pVar	=m_theVariableList.FindObj(pVarName);
	}
	return pVar;
}
BOOL	CLBS_Function::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	ClearParameterVariable();
	ClearMemberFunction();

	LPSTR	pStart =pCode;
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	if(!LoadFunctionHeader(pCode,pEnd))
		return FALSE;
	BOOL bRet =LoadFunctionBody(pCode,pEnd);
	if(bRet)
	{
		SetOriginCode(pStart,pCode-pStart);
	}
	return bRet;
}
BOOL	CLBS_Function::Save(CString& OUT strCode)
{

	return TRUE;
}
BOOL	CLBS_Function::Execute()
{
	CLBS_CodeBase*	pArea =GetCallArea()!=NULL?GetCallArea():GetParent();
	switch(GetFunctionType())
	{
	case LFT_APP_FUNCTION:
	case LFT_SYS_FUNCTION:
		{
			if(m_pSysFuncAddr == NULL)
				return FALSE;
			DWORD	code =0;
			LBS_SafeRelease(m_pReturnVariable);
			

			m_pReturnVariable =m_pSysFuncAddr(m_theParameterVariableList,pArea,code);
			if(code != LBS_AEC_SUCESS)
			{
				TraceCallFailureInfo(code);
				return FALSE;
			}
			return TRUE;
		}
		break;
	case LFT_USER_FUNCTION:
		{
			CLBS_CodeBase*	pCode =NULL;
			for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
			{
				pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj(i);
				pCode->SetCallArea(pArea);
				if(!pCode->Execute())
					return FALSE;
			}

			LBS_SafeRelease(m_pReturnVariable);
			if(pCode != NULL)
			{
				m_pReturnVariable	=pCode->GetReturnVariable();
				if(m_pReturnVariable != NULL)
					m_pReturnVariable->AddRef();
			}
			return TRUE;
		}
		break;
	case LFT_MEMBER_FUNCTION:
		{//是对象的成员函数
			if(m_pSysFuncAddr == NULL)
				return FALSE;
			DWORD	code =0;
			LBS_SafeRelease(m_pReturnVariable);

			m_pReturnVariable =m_pSysFuncAddr(m_theParameterVariableList,pArea,code);
			if(code != LBS_AEC_SUCESS)
			{
				TraceCallFailureInfo(code);
				return FALSE;
			}
			return TRUE;
		}
		break;
	default:
		break;
	}
	
	return FALSE;
}
BOOL	CLBS_Function::LoadFunctionHeader(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	CHAR				Words[KEY_WORDS_MAX_LEN];
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordsStart	=NULL;
	LPSTR				pKeyPos		=NULL;

	pKeyPos		=m_pScriptFile->GetNextValidKeywordsPosition(pParsePos,pEnd,pKeywords);
	if(pKeyPos == NULL)
		return FALSE;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
		return FALSE;

	if(pWordsStart == pKeyPos)
		return FALSE;
	if(pKeywords->KeywordsType != KWT_PARA_LIST_START)
		return FALSE;
	pParsePos	=pKeyPos + pKeywords->GetKeywordsLen();


	if(IsMatchKeywords(Words,pKeywords))
	{
		ShowDebugTrace("#关键字[ %s ]为保留字，不能定义为函数！",Words);
		return FALSE;
	}
	SetName(Words);//得到函数名

	while(m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
	{
		if(IsMatchKeywords(Words,pKeywords))
		{
			switch(pKeywords->KeywordsType)
			{
			case KWT_PARA_LIST_END:
				{

					pCode =pParsePos;
					return TRUE;
				}
				break;
			case KWT_SEGMENT_COMMENTS_START:
				{
					pParsePos	=m_pScriptFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_SEGMENT_COMMENTS_END);
					if(pParsePos == NULL)
						return FALSE;
					pParsePos	+= GetKeywords(KWT_SEGMENT_COMMENTS_END)->GetKeywordsLen();
				}
				break;
			case KWT_PARA_SEPERATOR:
				{
				}
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
			if(!AddParameterVariable(Words))
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}
BOOL	CLBS_Function::LoadFunctionBody(LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	LPSTR					pStart		=pCode;
	CLBS_CodeBlock*			pCodeBlock	=new CLBS_CodeBlock(this);
	if(pCodeBlock->Load(m_pScriptFile,pCode,pEnd))
	{
		AddExecuteCode(pCodeBlock);
		pCodeBlock->Release();
		SetOriginBodyCode(pStart,pCode-pStart);
		return TRUE;
	}
	pCodeBlock->Release();


	return FALSE;
}

BOOL	CLBS_Function::AddParameterVariable(LPCSTR IN pVarName)//添加参数变量
{
	if(IsDigit(pVarName[0]))
	{
		ShowDebugTrace("#错误：添加参数变量失败！参数变量[%s]命名非法!不能以数字开头。",pVarName);
		return FALSE;
	}
	if(m_theParameterVariableList.FindObj(pVarName)!=NULL)
		return FALSE;
	CLBS_Variable*	pParaVar	=new CLBS_Variable(this);
	pParaVar->SetName(pVarName);
	m_theParameterVariableList.AddObj(pParaVar);
	pParaVar->Release();
	return TRUE;
}
VOID	CLBS_Function::AddParameterVariable(CLBS_ObjBase* IN pVar)
{
	m_theParameterVariableList.AddObj(pVar);
}
CLBS_ObjBase*			CLBS_Function::GetParameterVariable(LONG IN lIndex)		//根据索引获取参数变量
{
	return m_theParameterVariableList.GetObj(lIndex);
}
LONG					CLBS_Function::GetParameterVariableCount()
{
	return m_theParameterVariableList.GetCount();
}	
VOID					CLBS_Function::TraceCallFailureInfo(DWORD IN code)
{
	switch(code)
	{
	case LBS_AEC_SUCESS:
		{
		}
		break;
	case LBS_AEC_FAILURE:
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！",GetName());
		}
		break;
	case LBS_AEC_PARA_INVALID:				//参数非法
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！参数非法!",GetName());
		}
		break;
	case LBS_AEC_PARA_TYPE_ERROR:			//参数变量类型不正确
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！参数类型不正确!",GetName());

		}
		break;

	case LBS_AEC_PARA_COUNT_ERROR:			//参数数量不正确
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！参数数量不正确!",GetName());

		}
		break;

	case LBS_AEC_PARA_NOT_FOUND:				//参数未找到
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！参数未找到!",GetName());
		}
		break;

	case LBS_AEC_SYSTEM_CALL_ERROR:			//系统调用失败
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！系统调用失败!",GetName());

		}
		break;

	case LBS_AEC_FUNCTION_NOT_DEFINATION:	//函数未定义
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！函数未定义!",GetName());

		}
		break;
	case LBS_AEC_NOT_SUPPORT_METHOD://对象不支持该方法
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！对象不支持该方法!",GetName());
		}
		break;
	case LBS_AEC_SCRIPT_NOT_VALID:			//脚本当前无效
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！脚本当前无效!",GetName());

		}
		break;
	default:
		{
			ShowDebugTrace("#错误：函数[ %s ]调用失败！未知错误!",GetName());
		}
		break;
	}
}
