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
		{//�Ƕ���ĳ�Ա����
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
		ShowDebugTrace("#�ؼ���[ %s ]Ϊ�����֣����ܶ���Ϊ������",Words);
		return FALSE;
	}
	SetName(Words);//�õ�������

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

BOOL	CLBS_Function::AddParameterVariable(LPCSTR IN pVarName)//��Ӳ�������
{
	if(IsDigit(pVarName[0]))
	{
		ShowDebugTrace("#������Ӳ�������ʧ�ܣ���������[%s]�����Ƿ�!���������ֿ�ͷ��",pVarName);
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
CLBS_ObjBase*			CLBS_Function::GetParameterVariable(LONG IN lIndex)		//����������ȡ��������
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
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�",GetName());
		}
		break;
	case LBS_AEC_PARA_INVALID:				//�����Ƿ�
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ������Ƿ�!",GetName());
		}
		break;
	case LBS_AEC_PARA_TYPE_ERROR:			//�����������Ͳ���ȷ
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ��������Ͳ���ȷ!",GetName());

		}
		break;

	case LBS_AEC_PARA_COUNT_ERROR:			//������������ȷ
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�������������ȷ!",GetName());

		}
		break;

	case LBS_AEC_PARA_NOT_FOUND:				//����δ�ҵ�
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�����δ�ҵ�!",GetName());
		}
		break;

	case LBS_AEC_SYSTEM_CALL_ERROR:			//ϵͳ����ʧ��
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�ϵͳ����ʧ��!",GetName());

		}
		break;

	case LBS_AEC_FUNCTION_NOT_DEFINATION:	//����δ����
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�����δ����!",GetName());

		}
		break;
	case LBS_AEC_NOT_SUPPORT_METHOD://����֧�ָ÷���
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�����֧�ָ÷���!",GetName());
		}
		break;
	case LBS_AEC_SCRIPT_NOT_VALID:			//�ű���ǰ��Ч
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ��ű���ǰ��Ч!",GetName());

		}
		break;
	default:
		{
			ShowDebugTrace("#���󣺺���[ %s ]����ʧ�ܣ�δ֪����!",GetName());
		}
		break;
	}
}
