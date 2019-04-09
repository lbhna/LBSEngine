#include "StdAfx.h"
#include ".\lbs_codeblock.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Variable.h"
#include "./LBS_Statement_Return.h"
#include "./LBS_Statement_Ifelse.h"
#include "./LBS_Statement_While.h"
#include "./LBS_Statement_For.h"
#include "./LBS_Statement_Break.h"
#include "./LBS_Statement_Continue.h"
#include "./LBS_Statement_Switch.h"
#include "./LBS_Function.h"




CLBS_CodeBlock::CLBS_CodeBlock(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_BLOCK;
	m_bIsReturn		=FALSE;

}

CLBS_CodeBlock::~CLBS_CodeBlock(void)
{
}
BOOL			CLBS_CodeBlock::Execute()
{
	SetIsReturn(FALSE);
	SetIsBreak(FALSE);
	SetIsContinue(FALSE);
	CLBS_CodeBase*	pCode =NULL;
	for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
	{
		pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj(i);
		pCode->SetCallArea(GetCallArea());
		if(!pCode->Execute())
			return FALSE;
		if(pCode->IsReturn())
		{
			LBS_SafeRelease(m_pReturnVariable);
			m_pReturnVariable	=pCode->GetReturnVariable();
			if(m_pReturnVariable!=NULL)
				m_pReturnVariable->AddRef();
			if(GetParent()->GetType() != LOT_CODE_FUNCTION)
			{
				SetIsReturn(TRUE);
			}
			break;
		}
		
		if(pCode->IsBreak())
		{
			SetIsBreak(TRUE);
			break;
		}
		if(pCode->IsContinue())
		{
			SetIsContinue(TRUE);
			break;
		}

	}

	return TRUE;
}
BOOL			CLBS_CodeBlock::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;

	CHAR				Words[KEY_WORDS_MAX_LEN];
	PKEYWORDSSTRUCT		pKeywords			=NULL;
	LPSTR				pParsePos			=pCode;
	LPSTR				pWordsStart			=NULL;
	BOOL				bHaveStartKeyword	=FALSE;
	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
		return FALSE;
	if(IsMatchKeywords(Words,pKeywords))
	{
		if(pKeywords->KeywordsType == KWT_CODE_BLOCK_START)
		{
			bHaveStartKeyword =TRUE;
		}
	}
	if(!bHaveStartKeyword)
		pParsePos =pWordsStart;

	
	while(m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
	{
		if(IsMatchKeywords(Words,pKeywords))
		{
			switch(pKeywords->KeywordsType)
			{
			case KWT_VARIABLE_DEFINE:
				{
					CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(this,TRUE);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();
				}
				break;
			case KWT_END_LINE:
				{
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_CODE_BLOCK_START:
				{
					pParsePos	=pWordsStart;
					CLBS_CodeBlock*			pCodeBlock	=new CLBS_CodeBlock(this);
					if(!pCodeBlock->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pCodeBlock->Release();
						return FALSE;
					}
					AddExecuteCode(pCodeBlock);
					pCodeBlock->Release();
				
				}
				break;
			case KWT_CODE_BLOCK_END:
				{//代码块结束
					pCode	=pParsePos;
					return TRUE;
				}
				break;
			case KWT_STATEMENT_IF:
				{
					pParsePos =pWordsStart;
					CLBS_Statement_Ifelse*		pStatement =new CLBS_Statement_Ifelse(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();
					
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_STATEMENT_WHILE:
				{
					pParsePos =pWordsStart;
					CLBS_Statement_While*		pStatement =new CLBS_Statement_While(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_STATEMENT_FOR:
				{
					pParsePos =pWordsStart;
					CLBS_Statement_For*		pStatement =new CLBS_Statement_For(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_RETURN_VALUE:
				{
					CLBS_Statement_Return*		pStatement =new CLBS_Statement_Return(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_CONTINUE:
				{
					CLBS_Statement_Continue*	pStatement=new CLBS_Statement_Continue(this);
					AddExecuteCode(pStatement);
					pStatement->Release();
					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_BREAK:
				{
					CLBS_Statement_Break*	pStatement=new CLBS_Statement_Break(this);
					AddExecuteCode(pStatement);
					pStatement->Release();

					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;
			case KWT_STATEMENT_SWITCH:
				{
					pParsePos =pWordsStart;
					CLBS_Statement_Switch*	pStatement =new CLBS_Statement_Switch(this);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					AddExecuteCode(pStatement);
					pStatement->Release();

					if(!bHaveStartKeyword)
					{
						pCode =pParsePos;
						return TRUE;
					}
				}
				break;

			case KWT_LINE_COMMENTS:
				{
					pParsePos = m_pScriptFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_RETURN);
					
				}
				break;
			case KWT_SEGMENT_COMMENTS_START:
				{
					pParsePos = m_pScriptFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_SEGMENT_COMMENTS_END);
					if(pParsePos == NULL)
						return FALSE;
					pParsePos += GetKeywords(KWT_SEGMENT_COMMENTS_END)->GetKeywordsLen();
				}
				break;
			case KWT_FUNCTION_DEFINE:
				{
					CLBS_Function*				pFunction	=new CLBS_Function(this);
					if(!pFunction->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pFunction->Release();
						return FALSE;
					}
					if(!AddMemberFunction(pFunction))
					{
						ShowDebugTrace("#错误：类的成员函数 [ %s ]重复定义！",pFunction->GetName());
						pFunction->Release();
						return FALSE;
					}
					pFunction->Release();
				}
				break;
            case KWT_OPERATOR_DELETE:
                {
                    pParsePos = pWordsStart;
                    CLBS_Statement_Calculate*	pStatement = new CLBS_Statement_Calculate(this);
                    if (!pStatement->Load(m_pScriptFile, pParsePos, pEnd))
                    {
                        pStatement->Release();
                        return FALSE;
                    }
                    AddExecuteCode(pStatement);
                    pStatement->Release();
                }
                break;
			default:
				{
					ShowDebugTrace("#错误：不能处理的关键字 [ %s ] [ %d ].",Words,pKeywords->KeywordsType);
					pCode	=pParsePos;
					return FALSE;
				}
				break;
			}
		}
		else
		{
			pParsePos =pWordsStart;
			CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(this);
			if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
			{
				pStatement->Release();
				return FALSE;
			}
			AddExecuteCode(pStatement);
			pStatement->Release();
			if(!bHaveStartKeyword)
			{
				pCode =pParsePos;
				return TRUE;
			}
		}
	}
	pCode	=pParsePos;
	return FALSE;
}
BOOL			CLBS_CodeBlock::Save(CString& OUT strCode)
{
	return TRUE;
}
