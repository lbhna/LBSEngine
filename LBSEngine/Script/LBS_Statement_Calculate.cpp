#include "StdAfx.h"
#include ".\lbs_statement_calculate.h"
#include "./LBS_Function.h"
#include "./LBS_ScriptMacroDefine.h"
#include "./LBS_Variable.h"
#include "./LBS_Function_Call.h"
#include "./LBS_Script_ClassBase.h"



CLBS_Statement_Calculate::CLBS_Statement_Calculate(CLBS_CodeBase* IN pParent,BOOL IN bVarDeclareStatement,KEYWORDS_TYPE IN EndKeywordType,CLBS_CodeBase* IN pCallArea):
CLBS_CodeBase(pParent)
{
	m_Type					=LOT_CODE_CALC_STATEMENT;
	m_pCallArea				=pCallArea;

	m_EndKeywordType		=EndKeywordType;
	m_bVarDeclareStatement	=bVarDeclareStatement;
}

CLBS_Statement_Calculate::~CLBS_Statement_Calculate(void)
{
	Clear();
}
VOID			CLBS_Statement_Calculate::Clear()
{
	m_theFactorList.Clear();
}
BOOL			CLBS_Statement_Calculate::Execute()
{
	if(!CLBS_CodeBase::Execute())
		return FALSE;


	LBS_SafeRelease(m_pReturnVariable);

	if(m_theFactorList.GetCount() == 0)
		return TRUE;

	CLBS_Statement_CalculateFactor*		pFactor		=NULL;
	LONG								i			=0;
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		if(pFactor->GetFactorType() == LBS_CT_CALC_STATEMENT)
		{//��ִ���Ӿ�
			CLBS_Statement_Calculate* pStatement =*(CLBS_Statement_Calculate**)pFactor->GetFactorData();
			pStatement->SetCallArea(GetCallArea());
			if(!pStatement->Execute())
				return FALSE;
		}
	}
	//TRACE("Execute:%s\r\n",GetString());


	CLBS_Statement_CalculateFactor*		pResFactor	=NULL;
	CLBS_Statement_CalculateFactor*		pOper		=NULL;
	CLBS_Statement_CalculateFactor*		pDstFactor	=NULL;
	CLBS_Variable*						pRet		=NULL;
	LONG								index		=0;
	BOOL								bSucess		=FALSE;
	BOOL								bParaList	=FALSE;
	FACTOR_CLASS						fc;
	LBS_CT_TYPE							ft;
	LONG								FactorCount =m_theFactorList.GetCount();
	while(index < FactorCount)
	{
		pDstFactor	=NULL;
		pOper		=NULL;
		if(pResFactor == NULL)
		{
			pResFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(index);
			pResFactor->AddRef();
			ft		=pResFactor->GetFactorType();
			fc		=pResFactor->GetFactorClass();
			index ++;
			if(index >= FactorCount)
			{
				if(bParaList)
				{
					if(fc != LBS_FC_OPERFACTOR)
					{
						LBS_SafeRelease(pRet);
						LBS_SafeRelease(pResFactor);
						return FALSE;
					}
					CLBS_Variable* pParameter =pResFactor->GetValueVariable();
					if(pRet == NULL)
					{
						pRet =new CLBS_Variable(GetParent());
						pRet->Set(&pParameter,sizeof(CLBS_Variable*),LVT_VAR_ARRAY);
					}
					else
					{
						pRet->SetSuffixValue(pRet->GetMemberCount(),pParameter);
					}
					pParameter->Release();
					pRet->ReverseOrder();
					m_pReturnVariable	=pRet;
					return TRUE;
				}
				break;
			}
		}



		pOper		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(index);
		index ++;
		if(index >= FactorCount)
			break;

		fc	=pOper->GetFactorClass();
		ft	=pOper->GetFactorType();

		if(ft == LBS_CT_SEPERATOR)
		{//�����б�
			bParaList	=TRUE;
			CLBS_Variable* pParameter =pResFactor->GetValueVariable();
			if(pRet == NULL)
			{
				pRet =new CLBS_Variable(GetParent());
				pRet->Set(&pParameter,sizeof(CLBS_Variable*),LVT_VAR_ARRAY);
			}
			else
			{
				pRet->SetSuffixValue(pRet->GetMemberCount(),pParameter);
			}
			pParameter->Release();
			LBS_SafeRelease(pResFactor);
			continue;
		}
		else
		{
			if(fc==LBS_FC_DOUBLE_OPER)
			{//˫Ŀ��������������Ŀ�Ĳ�����
				pDstFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(index);
				index ++;
				if(index >= FactorCount)
					break;
			}
		}


		pRet =Calculate(pResFactor,pOper,pDstFactor,bSucess);

		//TRACE("0x%x:%s\r\n",pRet,GetString());

		LBS_SafeRelease(pResFactor);
		if(!bSucess)
		{
			LBS_SafeRelease(pRet);
			ShowDebugTrace("#���󣺱��ʽ[%s]ִ��ʧ��!",GetString());
			return FALSE;
		}

		//��Ϊ��һ�ֵ�Դ������
		if(pRet != NULL)
			pResFactor	=new CLBS_Statement_CalculateFactor(this,&pRet,sizeof(CLBS_Variable*),LBS_CT_VAR_OBJ);

	}

	////////////////
	pRet =Calculate(pResFactor,pOper,pDstFactor,bSucess);
	//TRACE("0x%x:%s\r\n",pRet,GetString());
	LBS_SafeRelease(pResFactor);
	if(!bSucess)
	{
		LBS_SafeRelease(pRet);
		ShowDebugTrace("#���󣺱��ʽ[%s]ִ��ʧ��!",GetString());
		return FALSE;
	}

	///////////////
	m_pReturnVariable	=pRet;
	return TRUE;
}
inline VOID			CLBS_Statement_Calculate::AddFactor(CLBS_ObjBase* IN pFactor)
{
	m_theFactorList.AddObj(pFactor);
}
VOID			CLBS_Statement_Calculate::AddFactor(LPSTR IN pFactorString,LBS_CT_TYPE IN type)
{
	CLBS_Statement_CalculateFactor*		pFactor =new CLBS_Statement_CalculateFactor(this,pFactorString,(DWORD)strlen(pFactorString),type);
	m_theFactorList.AddObj(pFactor);
	pFactor->Release();
}
VOID			CLBS_Statement_Calculate::AddFactor(VOID* IN pData,DWORD IN dwSize,LBS_CT_TYPE IN type)
{
	CLBS_Statement_CalculateFactor*		pFactor =new CLBS_Statement_CalculateFactor(this,pData,dwSize,type);
	m_theFactorList.AddObj(pFactor);
	pFactor->Release();
}
BOOL			CLBS_Statement_Calculate::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	Clear();
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;
	CHAR							Words[KEY_WORDS_MAX_LEN];
	LPSTR							pWordsStart		=NULL;
	PKEYWORDSSTRUCT					pKeywords		=NULL;
	LPSTR							pParsePos		=pCode;
	CLBS_Statement_CalculateFactor*	pFactor			=NULL;
	CLBS_CodeBase*					pCurrentArea	=this;

	while(pFile->GetNextWords(pParsePos,pEnd,pWordsStart,Words))
	{
		if(IsMatchKeywords(Words,pKeywords))
		{
			if(pKeywords->KeywordsType == m_EndKeywordType)
			{
				//����ԭʼ����
				SetOriginCode(pCode,(LONG)(pWordsStart - pCode));
				//////////////////////////
				pCode =pParsePos;
				if(IsChildCalculateStatement())
					return TRUE;
				return PrepareProcessStatement();
			}

			switch(pKeywords->KeywordsType)
			{
			case KWT_EQU:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_EQUAL);
				}
				break;
			case KWT_ADD:
				{
					AddFactor(Words,LBS_CT_ADD);
				}
				break;
			case KWT_SUB:
				{
					AddFactor(Words,LBS_CT_SUB);
				}
				break;
			case KWT_MUL:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_MUL);
				}
				break;
			case KWT_DIV:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������

					AddFactor(Words,LBS_CT_DIV);
				}
				break;
			case KWT_STRING_DOUBLE:
				{
					LPSTR	pStrEnd		=pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_DOUBLE);
					if(pStrEnd == NULL)
						return FALSE;
					AddFactor(pParsePos,(DWORD)(pStrEnd-pParsePos),LBS_CT_STRING);
					pParsePos = pStrEnd + GetKeywords(KWT_STRING_DOUBLE)->GetKeywordsLen();
                    
				}
				break;
			case KWT_STRING_SINGLE:
				{
					LPSTR	pStrEnd		=pFile->GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_SINGLE);
					if(pStrEnd == NULL)
						return FALSE;
					AddFactor(pParsePos,(DWORD)(pStrEnd-pParsePos),LBS_CT_STRING);
					pParsePos = pStrEnd + GetKeywords(KWT_STRING_SINGLE)->GetKeywordsLen();
				}
				break;
			case KWT_PARA_LIST_START:
				{
					CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(this,FALSE,KWT_PARA_LIST_END,m_pCallArea);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					CLBS_Statement_CalculateFactor*		pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
					AddFactor(pNewFactor);
					pNewFactor->Release();
				}
				break;
			case KWT_PARA_SEPERATOR:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					//����ԭʼ����
					SetOriginCode(pCode,(LONG)(pWordsStart - pCode));


					//ͬ�������Ӿ�
					CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(GetParent(),m_bVarDeclareStatement,m_EndKeywordType,m_pCallArea);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}

					if(GetParent()->GetType()==LOT_CODE_CALC_STATEMENT)
					{//�����Ӿ���ҪԤ����һ��
						if(!pStatement->PrepareProcessStatement())
						{
							pStatement->Release();
							return FALSE;
						}
					}

					GetParent()->AddExecuteCode(pStatement);
					pStatement->Release();

					//////////////////////////
					pCode =pParsePos;
					if(IsChildCalculateStatement())
						return TRUE;
					return PrepareProcessStatement();
				}
				break;
			case KWT_SUFFIX_START:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_SUFFIX_START);
					
					//�±����
					CLBS_Statement_Calculate*			pStatement =new CLBS_Statement_Calculate(this,FALSE,KWT_SUFFIX_END,m_pCallArea);
					if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
					{
						pStatement->Release();
						return FALSE;
					}
					CLBS_Statement_CalculateFactor*		pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
					AddFactor(pNewFactor);
					pNewFactor->Release();

					AddFactor(GetKeywords(KWT_SUFFIX_END)->Keywords,LBS_CT_SUFFIX_END);
				}
				break;
			case KWT_CHILD_AREA:
				{
					AddFactor(Words,LBS_CT_AREA_CHILD);
				}
				break;
			case KWT_ORIGIN_AREA:
				{
					AddFactor(Words,LBS_CT_AREA_ORIGIN);
				}
				break;
			case KWT_LOGIC_AND:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_LOGIC_AND);
				}
				break;
			case KWT_LOGIC_OR:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_LOGIC_OR);
				}
				break;
			case KWT_LOGIC_NOT:
				{
					AddFactor(Words,LBS_CT_LOGIC_NOT);
				}
				break;
			case KWT_AND:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_AND);
				}
				break;
			case KWT_OR:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_OR);
				}
				break;
			case KWT_XOR:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_XOR);
				}
				break;
			case KWT_LMB:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_LMB);
				}
				break;
			case KWT_RMB:
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_RMB);
				}
				break;

			case KWT_NOT:
				{
					AddFactor(Words,LBS_CT_NOT);
				}
				break;
			case KWT_EQUAL:		//����
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_LOGIC_EQUAL);
				}
				break;
			case KWT_MOD:		//ģ��
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_MOD);
				}
				break;
			case KWT_B:		
				{
					if(m_theFactorList.GetCount()==0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_LOGIC_B);
				}
				break;
			case KWT_BE:	
				{
					AddFactor(Words,LBS_CT_LOGIC_BE);
				}
				break;
			case KWT_S:		
				{
					AddFactor(Words,LBS_CT_LOGIC_S);
				}
				break;
			case KWT_SE:		
				{
					AddFactor(Words,LBS_CT_LOGIC_SE);
				}
				break;
			case KWT_NOTEQUAL:
				{
					AddFactor(Words,LBS_CT_LOGIC_NOTEQUAL);
				}
				break;
			case KWT_CONST_TRUE:
				{
					BOOL v	=TRUE;
					AddFactor(&v,sizeof(BOOL),LBS_CT_INTEGER);
				}
				break;
			case KWT_CONST_FALSE:
				{
					BOOL v	=FALSE;
					AddFactor(&v,sizeof(v),LBS_CT_INTEGER);
				}
				break;
			case KWT_CONST_NULL:
				{
					BOOL v=0;
					AddFactor(&v,sizeof(v),LBS_CT_INTEGER);
				}
				break;
			case KWT_CONST_CR:
				{
					AddFactor("\r",LBS_CT_STRING);
				}
				break;
			case KWT_CONST_LF:
				{
					AddFactor("\n",LBS_CT_STRING);
				}
				break;
			case KWT_CONST_TAB:
				{
					AddFactor("\t",LBS_CT_STRING);
				}
				break;
			case KWT_OPERATOR_NEW:
				{
					AddFactor(Words,LBS_CT_OPER_NEW);
				}
				break;
			case KWT_OPERATOR_DELETE:
				{
					if(m_theFactorList.GetCount()!=0)
						return FALSE;//����Ϊ��һ������
					AddFactor(Words,LBS_CT_OPER_DELETE);
				}
				break;
			default:
				{
					ShowDebugTrace("#���󣺹ؼ���[ %s ]ʹ�ô���",Words);
					return FALSE;
				}
				break;
			}
		}
		else
		{
			if(!AddFactorByWord(pParsePos,pEnd,pWordsStart,Words))
			{
				ShowDebugTrace("#�����޷������ĵ���[ %s ].",Words);
				return FALSE;
			}
		}
	}
	return FALSE;
}
BOOL			CLBS_Statement_Calculate::Save(CString& OUT strCode)
{
	return TRUE;
}
BOOL			CLBS_Statement_Calculate::IsHaveFactor(LBS_CT_TYPE IN FactorType)
{
	CLBS_Statement_CalculateFactor*		pFactor			=NULL;
	for(LONG i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor =(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		if(pFactor->GetFactorType() == FactorType)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL			CLBS_Statement_Calculate::PrepareCompile()
{

	if(!IsHaveFactor(LBS_CT_EQUAL))
		return TRUE;

	CLBS_Statement_CalculateFactor*		pFactor			=NULL;
	CLBS_ObjListBase					theStackList;


	//����Ⱥ����ȼ�
	LPSTR	pStartWords	=GetKeywords(KWT_PARA_LIST_START)->Keywords;
	LPSTR	pEndWords	=GetKeywords(KWT_PARA_LIST_END)->Keywords;

	CLBS_Statement_CalculateFactor*	pStartFactor	=new CLBS_Statement_CalculateFactor(this,pStartWords,(DWORD)strlen(pStartWords),LBS_CT_CHILD_START);
	CLBS_Statement_CalculateFactor*	pEndFactor		=new CLBS_Statement_CalculateFactor(this,pEndWords,(DWORD)strlen(pEndWords),LBS_CT_CHILD_END);

	theStackList.AddObj(pStartFactor);
	for(LONG i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor =(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		if(pFactor->GetFactorType() == LBS_CT_EQUAL)
		{
			theStackList.AddObj(pEndFactor);
			theStackList.AddObj(pFactor);
			theStackList.AddObj(pStartFactor);
		}
		else
		{
			theStackList.AddObj(pFactor);
		}
	}

	theStackList.AddObj(pEndFactor);
	m_theFactorList = theStackList;

	pStartFactor->Release();
	pEndFactor->Release();


	LONG				lStartIndex			=0;
	m_theFactorList.Clear();
	return PrepareCompile(theStackList,lStartIndex);
}

BOOL			CLBS_Statement_Calculate::PrepareCompile(CLBS_ObjListBase& IN FactorList,LONG& IN lStartIndex,BOOL IN bChild)
{
	CLBS_Statement_CalculateFactor*		pFactor			=NULL;
	BOOL								bAddSuffixChild	=FALSE;
	for(;lStartIndex<FactorList.GetCount();lStartIndex++)
	{

		pFactor			=(CLBS_Statement_CalculateFactor*)FactorList.GetObj(lStartIndex);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_CHILD_START:
			{
				lStartIndex++;
				CLBS_Statement_Calculate*	pChildStatement =new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				if(!pChildStatement->PrepareCompile(FactorList,lStartIndex,TRUE))
				{
					pChildStatement->Release();
					return FALSE;
				}
				AddFactor(&pChildStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				lStartIndex--;
			}
			break;
		case LBS_CT_CHILD_END:
			{
				if(bChild)
					return TRUE;
			}
			break;
		default:
			{
				AddFactor(pFactor);
			}
			break;
		}
	}
	return TRUE;
}
BOOL		CLBS_Statement_Calculate::Compile()
{
	CLBS_Statement_CalculateFactor*		pFactor			=NULL;
	LONG								i			=0;
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor =(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		if(pFactor->GetFactorType() == LBS_CT_CALC_STATEMENT)
		{
			if(!((CLBS_Statement_Calculate**)pFactor->GetFactorData())[0]->Compile())
				return FALSE;
		}
	}

	CLBS_ObjListBase					theTempStack;

	//����ȡ�±����ȼ�
	CLBS_Statement_CalculateFactor* pNextFactor;
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);

		if(pFactor->GetFactorType() !=LBS_CT_SUFFIX_END)
			theTempStack.AddObj(pFactor);
	}

	//////////////////////
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	////////////////////
	//�����������ȼ�
	CLBS_Statement_CalculateFactor* pNextNextFactor	=NULL;
	CLBS_Statement_Calculate*		pChildStatement =NULL;
	BOOL							bHaveChildArea	=FALSE;
	
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		if(bHaveChildArea)
		{
			pFactor			=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
			pNextFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
			pNextNextFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i+1);
		}
		else
		{
			pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
			pNextFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i+1);
			pNextNextFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i+2);
		}
		bHaveChildArea =FALSE;
		if(pNextNextFactor == NULL)
		{
			theTempStack.AddObj(pFactor);
			if(pNextFactor != NULL && pNextFactor != pFactor)
				theTempStack.AddObj(pNextFactor);
			break;
		}
		if(pNextFactor->GetFactorType() ==LBS_CT_AREA_CHILD || pNextFactor->GetFactorType() ==LBS_CT_SUFFIX_START)
		{
			bHaveChildArea =TRUE;

			if(pChildStatement == NULL)
			{
				pChildStatement =new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pChildStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();
			}			
			if(pFactor == pNextFactor)
			{
				pChildStatement->AddFactor(pNextFactor);
				pChildStatement->AddFactor(pNextNextFactor);
				i+=1;
			}
			else
			{
				pChildStatement->AddFactor(pFactor);
				pChildStatement->AddFactor(pNextFactor);
				pChildStatement->AddFactor(pNextNextFactor);
				i+=2;
			}

			//////////////////////////
		}
		else
		{
			pChildStatement =NULL;
			theTempStack.AddObj(pFactor);
		}
	}

	////////////////////
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	////////////////////
	
	



	////////////////////
	//����++,--�������
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		pNextFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i+1);
		pNextNextFactor	=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i+2);
		if(pNextNextFactor == NULL)
		{
			theTempStack.AddObj(pFactor);
			continue;
		}
		if( (pNextFactor->GetFactorType() ==LBS_CT_ADD&&pNextNextFactor->GetFactorType() ==LBS_CT_ADD) ||
			(pNextFactor->GetFactorType() ==LBS_CT_SUB&&pNextNextFactor->GetFactorType() ==LBS_CT_SUB) )
		{


			CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
			pStatement->AddFactor(pFactor);
			pStatement->AddFactor(GetKeywords(KWT_EQU)->Keywords,LBS_CT_EQUAL);

				//-----------------
				CLBS_Statement_Calculate*	pChildStatement	=new CLBS_Statement_Calculate(pStatement,FALSE,KWT_END_LINE,m_pCallArea);
				pChildStatement->AddFactor(pFactor);
				pChildStatement->AddFactor(pNextFactor);
				LONG adder=1;
				pChildStatement->AddFactor(&adder,sizeof(LONG),LBS_CT_INTEGER);
				//-----------------
			pStatement->AddFactor(&pChildStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);


			//////////////////////////
			CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
			theTempStack.AddObj(pNewFactor);
			pNewFactor->Release();



			i+=2;
		}
		else
		{
			theTempStack.AddObj(pFactor);
		}
	}

	////////////////////
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	////////////////////



	//�������������ȼ�
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_ADD:
		case LBS_CT_SUB:
			{
				//�ж��ǲ���������

				FACTOR_CLASS	fc ;
				CLBS_Statement_CalculateFactor*		pPrevFactor =NULL;
				if(i>0)
					pPrevFactor =(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i-1);
				
				BOOL	bSigned =FALSE;
				if(pPrevFactor == NULL)
				{
					bSigned =TRUE;
				}
				else
				{
					fc =pPrevFactor->GetFactorClass();
					if(fc == LBS_FC_DOUBLE_OPER || fc == LBS_FC_SEPERATOR || fc == LBS_FC_SINGLE_OPER)
					{
						bSigned =TRUE;
					}
				}

				if(bSigned)
				{
					CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
					pStatement->AddFactor(pFactor);
					i++;
					pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
					if(pFactor != NULL)
					{
						fc =pFactor->GetFactorClass();
						if(fc != LBS_FC_OPERFACTOR)
							return FALSE;
						pStatement->AddFactor(pFactor);

					}
					else
					{
						return FALSE;
					}
					

					//////////////////////////
					CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
					theTempStack.AddObj(pNewFactor);
					pNewFactor->Release();
				}
				else
				{
					theTempStack.AddObj(pFactor);
				}
			}
			break;
		default:
			{
				theTempStack.AddObj(pFactor);
			}
			break;
		}
	}
	////////////////////
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	////////////////////


	//�����߼��Ǻ�λ�ǵ����ȼ�
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_LOGIC_NOT:
		case LBS_CT_NOT:
			{
				CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				pStatement->AddFactor(pFactor);
				i++;
				pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
				FACTOR_CLASS	fc =pFactor->GetFactorClass();
				if(fc != LBS_FC_OPERFACTOR)
					return FALSE;
				pStatement->AddFactor(pFactor);

				//////////////////////////
				CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();
			}
			break;

		default:
			{
				theTempStack.AddObj(pFactor);
			}
			break;
		}
	}

	////////////////////
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	////////////////////

	//����λ�������ȼ�
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_AND:
		case LBS_CT_OR:
		case LBS_CT_XOR:
		case LBS_CT_LMB:
		case LBS_CT_RMB:
			{
				if(i<=0)
					return FALSE;
				i-=1;
				theTempStack.DelObj(theTempStack.GetCount()-1);

				BOOL						bEnd		=FALSE;
				CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				for(;i<m_theFactorList.GetCount()&&!bEnd;i++)
				{
					pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
					switch(pFactor->GetFactorType())
					{
					case LBS_CT_EQUAL:
					case LBS_CT_SEPERATOR:
					case LBS_CT_ADD:
					case LBS_CT_SUB:
					case LBS_CT_MUL:
					case LBS_CT_DIV:
					case LBS_CT_LOGIC_AND:
					case LBS_CT_LOGIC_OR:
					case LBS_CT_LOGIC_NOT:
					case LBS_CT_MOD:
					case LBS_CT_LOGIC_BE:
					case LBS_CT_LOGIC_B:
					case LBS_CT_LOGIC_S:
					case LBS_CT_LOGIC_SE:
						{
							bEnd =TRUE;
							i-=2;
						}
						break;
					default:
						{
							pStatement->AddFactor(pFactor);
						}
						break;
					}
				}

				CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();

			}
			break;
		default:
			{
				theTempStack.AddObj(pFactor);
			}
			break;
		}

	}

	//---------------------------------------------------
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	//---------------------------------------------------

	//�����,��,ģ���ȼ�
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_MUL:
		case LBS_CT_DIV:
		case LBS_CT_MOD:
			{
				if(i<=0)
					return FALSE;
				i-=1;
				theTempStack.DelObj(theTempStack.GetCount()-1);

				BOOL						bEnd		=FALSE;
				CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				for(;i<m_theFactorList.GetCount()&&!bEnd;i++)
				{
					pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
					switch(pFactor->GetFactorType())
					{
					case LBS_CT_ADD:
					case LBS_CT_SUB:
						{
							CLBS_Statement_CalculateFactor* pPrevFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i-1);
							if(pPrevFactor != NULL)
							{
								if(pPrevFactor->GetType() != LBS_CT_DIV &&
									pPrevFactor->GetType() != LBS_CT_MUL)
								{
									bEnd =TRUE;
									i-=2;
								}
								else
								{
									pStatement->AddFactor(pFactor);
								}
							}
						}
						break;
					case LBS_CT_LOGIC_AND:
					case LBS_CT_LOGIC_OR:
					case LBS_CT_SEPERATOR:
					case LBS_CT_EQUAL:
					case LBS_CT_LOGIC_BE:
					case LBS_CT_LOGIC_B:
					case LBS_CT_LOGIC_S:
					case LBS_CT_LOGIC_SE:
						{
							bEnd =TRUE;
							i-=2;
						}
						break;
					default:
						{
							pStatement->AddFactor(pFactor);
						}
						break;
					}
				}

				CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();

			}
			break;
		default:
			{
				theTempStack.AddObj(pFactor);
			}
			break;
		}

	}
	//---------------------------------------------------
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	//---------------------------------------------------

	//����Ӽ����ȼ�
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_ADD:
		case LBS_CT_SUB:
			{
				if(i<=0)
					return FALSE;
				i-=1;
				theTempStack.DelObj(theTempStack.GetCount()-1);

				BOOL						bEnd		=FALSE;
				CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
				for(;i<m_theFactorList.GetCount()&&!bEnd;i++)
				{
					pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
					switch(pFactor->GetFactorType())
					{
					case LBS_CT_LOGIC_AND:
					case LBS_CT_LOGIC_OR:
					case LBS_CT_LOGIC_EQUAL:
					case LBS_CT_LOGIC_NOTEQUAL:	//������
					case LBS_CT_LOGIC_BE:		//���ڵ���
					case LBS_CT_LOGIC_SE:		//С�ڵ���
					case LBS_CT_LOGIC_B:			//����
					case LBS_CT_LOGIC_S:			//С��
						{
							bEnd =TRUE;
							i-=2;
						}
						break;
					default:
						{
							pStatement->AddFactor(pFactor);
						}
						break;
					}
				}
				CLBS_Statement_CalculateFactor* pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();
			}
			break;
		default:
			{
				theTempStack.AddObj(pFactor);
			}
			break;
		}

	}

	//---------------------------------------------------
	m_theFactorList =theTempStack;
	theTempStack.Clear();
	ProcessLogicOperator();




	return TRUE;
}
VOID	CLBS_Statement_Calculate::ProcessLogicOperator()
{
	LONG								i				=0;
	BOOL								bNeedProcess	=FALSE;
	CLBS_Statement_CalculateFactor*		pFactor;
	CLBS_Statement_CalculateFactor*		pNewFactor;
	CLBS_ObjListBase					theTempStack;
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		if(pFactor->GetFactorType()== LBS_CT_LOGIC_AND || pFactor->GetFactorType() == LBS_CT_LOGIC_OR)
		{
			bNeedProcess =TRUE;
			break;
		}
	}
	if(!bNeedProcess)
		return;

	CLBS_Statement_Calculate*	pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
	for(i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		switch(pFactor->GetFactorType())
		{
		case LBS_CT_LOGIC_AND:
		case LBS_CT_LOGIC_OR:
			{
				pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
				theTempStack.AddObj(pNewFactor);
				pNewFactor->Release();

				theTempStack.AddObj(pFactor);
				pStatement	=new CLBS_Statement_Calculate(this,FALSE,KWT_END_LINE,m_pCallArea);
			}
			break;
		default:
			{
				pStatement->AddFactor(pFactor);
			}
			break;
		}
	}
	
	pNewFactor =new CLBS_Statement_CalculateFactor(this,&pStatement,sizeof(CLBS_Statement_Calculate*),LBS_CT_CALC_STATEMENT);
	theTempStack.AddObj(pNewFactor);
	pNewFactor->Release();

	m_theFactorList =theTempStack;
}

CString	CLBS_Statement_Calculate::GetString()
{

	CString		str;
	str.Empty();
	BOOL	bChild =(GetParent()->GetType()==LOT_CODE_CALC_STATEMENT);

	if(bChild)
		str+="{";
	CLBS_Statement_CalculateFactor* pFactor =NULL;
	for(LONG i=0;i<m_theFactorList.GetCount();i++)
	{
		pFactor		=(CLBS_Statement_CalculateFactor*)m_theFactorList.GetObj(i);
		str			+=pFactor->GetString();
	}
	if(bChild)
		str+="}";
	return str;
}

CLBS_Variable*			CLBS_Statement_Calculate::Calculate(
	CLBS_Statement_CalculateFactor* IN pResFactor,
	CLBS_Statement_CalculateFactor* IN pOperFactor,
	CLBS_Statement_CalculateFactor* IN pDstFactor,
	BOOL& OUT bSucess)
{
	bSucess =FALSE;
	if(pResFactor == NULL)
		return NULL;//Դ����������Ϊ��



	CLBS_Variable*		pReturnVariable		=NULL;
	CLBS_Variable*		pResVar				=NULL;
	CLBS_Variable*		pDstVar				=NULL;
    
    if (pResFactor == NULL&&pOperFactor != NULL&&pDstVar != NULL)
    {
        switch (pOperFactor->GetFactorType())
        {
            case LBS_CT_OPER_DELETE:
            {
                if (pOperFactor->GetFactorType() != LBS_CT_VAR_NAME)
                    return NULL;
                pDstVar = pOperFactor->GetValueVariable();
                if (pDstVar == NULL)
                    return NULL;
            
                if (pDstVar->GetVarType() != LVT_CLASS)
                {
                    ShowDebugTrace("#���󣺱���[ %s ]����һ��������޷��ͷ�!", pDstVar->GetName());
                    pDstVar->Release();
                    return NULL;
                }
                pDstVar->Reset();
                pDstVar->Release();
                bSucess = TRUE;
            }
            break;
           default:
                break;
        }
        return pReturnVariable;
    }
    else if(pDstFactor == NULL)
	{//���Ŀ�Ĳ�����Ϊ��
		if(pOperFactor == NULL)
		{//��������Ŀ�Ĳ�������Ϊ��ʱ��ֱ�ӷ���Դ������
			if(pResFactor->GetFactorClass()!=LBS_FC_OPERFACTOR)
				return NULL;
			switch(pResFactor->GetFactorType())
			{
			case LBS_CT_CALC_STATEMENT:
				{//�Ǳ��ʽ
					pReturnVariable =(CLBS_Variable*)((CLBS_Statement_Calculate**)pResFactor->GetFactorData())[0]->GetReturnVariable();
					if(pReturnVariable != NULL)
						pReturnVariable->AddRef();
					bSucess		=TRUE;
				}
				break;
			case LBS_CT_FUNC_CALL:
				{//�Ǻ�������
					CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pResFactor->GetFactorData())[0];
					pFuncCall->SetCallArea(GetCallArea());
					pFuncCall->SetParent(GetParent());
					if(!pFuncCall->Execute())
						return NULL;
					pReturnVariable		=(CLBS_Variable*)pFuncCall->GetReturnVariable();
					if(pReturnVariable != NULL)
						pReturnVariable->AddRef();
					bSucess		=TRUE;
				}
				break;
			default:
				{	//�Ǳ�����
					pResFactor->SetCallArea(GetCallArea());
					pReturnVariable		=pResFactor->GetValueVariable();
					if(pReturnVariable != NULL)
						bSucess		=TRUE;
				}
				break;
			}
			return pReturnVariable;
		}
		else
		{//��������Ϊ��ʱ

			switch(pResFactor->GetFactorType())
			{
			case LBS_CT_ADD:
				{
					if(pOperFactor->GetFactorClass()!=LBS_FC_OPERFACTOR)
						return NULL;
					switch(pOperFactor->GetFactorType())
					{
					case LBS_CT_CALC_STATEMENT:
						{//�Ǳ��ʽ
							pReturnVariable =(CLBS_Variable*)((CLBS_Statement_Calculate**)pOperFactor->GetFactorData())[0]->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					case LBS_CT_FUNC_CALL:
						{//�Ǻ�������
							CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pOperFactor->GetFactorData())[0];
							pFuncCall->SetCallArea(GetCallArea());
							pFuncCall->SetParent(GetParent());
							if(!pFuncCall->Execute())
								return NULL;
							pReturnVariable		=(CLBS_Variable*)pFuncCall->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					default:
						{	//�Ǳ�����
							pOperFactor->SetCallArea(GetCallArea());
							pReturnVariable		=pOperFactor->GetValueVariable();
							if(pReturnVariable != NULL)
								bSucess		=TRUE;
						}
						break;
					}

				}
				break;
			case LBS_CT_SUB:
				{
					if(pOperFactor->GetFactorClass()!=LBS_FC_OPERFACTOR)
						return NULL;
					switch(pOperFactor->GetFactorType())
					{
					case LBS_CT_CALC_STATEMENT:
						{//�Ǳ��ʽ
							pReturnVariable =(CLBS_Variable*)((CLBS_Statement_Calculate**)pOperFactor->GetFactorData())[0]->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					case LBS_CT_FUNC_CALL:
						{//�Ǻ�������
							CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pOperFactor->GetFactorData())[0];
							pFuncCall->SetCallArea(GetCallArea());
							pFuncCall->SetParent(GetParent());
							if(!pFuncCall->Execute())
								return NULL;
							pReturnVariable		=(CLBS_Variable*)pFuncCall->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					default:
						{	//�Ǳ�����
							pOperFactor->SetCallArea(GetCallArea());
							pReturnVariable		=pOperFactor->GetValueVariable();
							if(pReturnVariable != NULL)
								bSucess		=TRUE;
						}
						break;
					}
					if(pReturnVariable != NULL)
					{
						CLBS_Variable* pTmp =new CLBS_Variable(this);
						*pTmp =*pReturnVariable;
						pReturnVariable->Release();
						pReturnVariable =pTmp;

						bSucess =pReturnVariable->Minus();
					}
				}
				break;
			case LBS_CT_NOT:
				{
					if(pOperFactor->GetFactorClass()!=LBS_FC_OPERFACTOR)
						return NULL;
					switch(pOperFactor->GetFactorType())
					{
					case LBS_CT_CALC_STATEMENT:
						{//�Ǳ��ʽ
							pReturnVariable =(CLBS_Variable*)((CLBS_Statement_Calculate**)pOperFactor->GetFactorData())[0]->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					case LBS_CT_FUNC_CALL:
						{//�Ǻ�������
							CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pOperFactor->GetFactorData())[0];
							pFuncCall->SetCallArea(GetCallArea());
							pFuncCall->SetParent(GetParent());
							if(!pFuncCall->Execute())
								return NULL;
							pReturnVariable		=(CLBS_Variable*)pFuncCall->GetReturnVariable();
							if(pReturnVariable != NULL)
								pReturnVariable->AddRef();
							bSucess		=TRUE;
						}
						break;
					default:
						{	//�Ǳ�����
							pOperFactor->SetCallArea(GetCallArea());
							pReturnVariable		=pOperFactor->GetValueVariable();
							if(pReturnVariable != NULL)
								bSucess		=TRUE;
						}
						break;
					}
					
				
					if(pReturnVariable != NULL)
					{
						CLBS_Variable* pTmp =new CLBS_Variable(this);
						*pTmp =*pReturnVariable;
						pReturnVariable->Release();
						pReturnVariable =pTmp;

						bSucess =pReturnVariable->NOT();
					}
				

				}
				break;
			case LBS_CT_LOGIC_NOT:
				{
					if(pOperFactor->GetFactorClass()!=LBS_FC_OPERFACTOR)
						return NULL;
					switch(pOperFactor->GetFactorType())
					{
					case LBS_CT_CALC_STATEMENT:
						{//�Ǳ��ʽ
							pDstVar =(CLBS_Variable*)((CLBS_Statement_Calculate**)pOperFactor->GetFactorData())[0]->GetReturnVariable();
							if(pDstVar != NULL)
								pDstVar->AddRef();
						}
						break;
					case LBS_CT_FUNC_CALL:
						{//�Ǻ�������
							CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pOperFactor->GetFactorData())[0];
							pFuncCall->SetCallArea(GetCallArea());
							pFuncCall->SetParent(GetParent());
							if(!pFuncCall->Execute())
								return NULL;
							pDstVar		=(CLBS_Variable*)pFuncCall->GetReturnVariable();
							if(pDstVar != NULL)
								pDstVar->AddRef();
						}
						break;
					default:
						{	//�Ǳ�����
							pOperFactor->SetCallArea(GetCallArea());
							pDstVar		=pOperFactor->GetValueVariable();
						}
						break;
					}


					pReturnVariable		=new CLBS_Variable(this);
					if(pDstVar->LNOT())
					{
						*pReturnVariable	=(LONG)1;
					}
					else
					{
						*pReturnVariable	=(LONG)0;
					}
					bSucess=TRUE;
					pDstVar->Release();

				}
				break;
			case LBS_CT_OPER_NEW:
				{
					if(pOperFactor->GetFactorType() != LBS_CT_CLASS_NAME)
						return NULL;
					pDstVar		=pOperFactor->GetValueVariable();
					if(pDstVar == NULL)
						return NULL;

					CLBS_Script_ClassBase*	pClassPrototype =(CLBS_Script_ClassBase*)m_pScriptFile->GetParser()->GetClassPrototype((LPCSTR)*pDstVar);
					if(pClassPrototype == NULL)
					{
						pDstVar->Release();
						ShowDebugTrace("#������ԭ�� [ %s ]δ���壬�޷�����ʵ��!",(LPCSTR)*pDstVar);
						return NULL;
					}
					CLBS_Script_ClassBase*	pNewInstance	=pClassPrototype->CreateInstance(GetCallArea());
					if(pNewInstance == NULL)
					{
						pDstVar->Release();
						ShowDebugTrace("#������ԭ�� [ %s ]ʵ����ʧ��!",(LPCSTR)*pDstVar);
						return NULL;
					}
					pReturnVariable		=new CLBS_Variable(this);
					pReturnVariable->Set(&pNewInstance,sizeof(CLBS_Script_ClassBase*),LVT_CLASS);
					pNewInstance->Release();
					pDstVar->Release();
					bSucess		=TRUE;
				}
				break;

			default:
				break;
			}
			return pReturnVariable;
		}
	}

 

	//��������������Ϊ��ʱ
	if(pResFactor->GetFactorType() == LBS_CT_FUNC_CALL)
	{//���Դ�������Ǻ�������
		CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pResFactor->GetFactorData())[0];
		pFuncCall->SetCallArea(GetCallArea());
		pFuncCall->SetParent(GetParent());
		if(!pFuncCall->Execute())
			return NULL;
	}
	if(pDstFactor->GetFactorType() == LBS_CT_FUNC_CALL && pOperFactor->GetFactorType() != LBS_CT_AREA_CHILD)
	{//���Դ�������Ǻ�������
		CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pDstFactor->GetFactorData())[0];
		pFuncCall->SetCallArea(GetCallArea());
		pFuncCall->SetParent(GetParent());
		if(!pFuncCall->Execute())
			return NULL;
	}
	switch(pOperFactor->GetFactorType())
	{
	case LBS_CT_ADD:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);


			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->Add(*pDstVar);
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_SUB:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);

			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->Sub(*pDstVar);
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_MUL:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);

			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->Mul(*pDstVar);
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_DIV:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);


			if(pReturnVariable->Equal(*pResVar))
			{
				bSucess=pReturnVariable->Div(*pDstVar);
			}

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_EQUAL:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL )
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			if(pDstVar== NULL )
			{
				pResVar->Set(NULL,0,LVT_STRING);
			}
			else
			{
				if(!pResVar->Equal(*pDstVar))
				{
					pResVar->Release();
					pDstVar->Release();	
					break;
				}
				pDstVar->Release();		
			}

			pReturnVariable		=pResVar;
			pReturnVariable->AddRef();
			bSucess				=TRUE;

			pResVar->Release();
		}
		break;
	case LBS_CT_SUFFIX_START:
		{//ȡ�±�
			pReturnVariable		=NULL;

			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			if(pDstVar->GetVarType() == LVT_INTEGER)
			{
				pReturnVariable =pResVar->GetSuffixValue((DWORD)*pDstVar);
				if(pReturnVariable != NULL)
					bSucess			=TRUE;
			}
			pResVar->Release();
			pDstVar->Release();
			return pReturnVariable;		
		}
		break;
	case LBS_CT_AREA_CHILD:
		{//����

			pReturnVariable		=NULL;
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL)
				return NULL;
			if(pResVar->GetVarType() == LVT_CLASS)
			{
				CLBS_Script_ClassBase*	pClass =*(CLBS_Script_ClassBase**)pResVar->GetDataBuffer();
				pDstFactor->SetCallArea(pClass);
			}
			switch(pDstFactor->GetFactorType())
			{
			case LBS_CT_FUNC_CALL:
				{
					CLBS_Function_Call*		pFuncCall =((CLBS_Function_Call**)pDstFactor->GetFactorData())[0];

					if(pResVar->GetVarType() == LVT_CLASS)
					{
						CLBS_Script_ClassBase*	pClass =*(CLBS_Script_ClassBase**)pResVar->GetDataBuffer();
						pFuncCall->SetCallArea(pClass);
					}
					else
					{
						pFuncCall->SetCallArea(pResVar);
					}
					
					pFuncCall->SetParent(GetParent());

					if(!pFuncCall->Execute())
						return NULL;
				
					pDstVar		=pDstFactor->GetValueVariable();

					pReturnVariable	=pDstVar;
					if(pReturnVariable != NULL)
						pReturnVariable->AddRef();
					bSucess =TRUE;
				}
				break;
			case LBS_CT_VAR_NAME:
				{
					pDstVar		=pDstFactor->GetValueVariable();
					if(pDstVar == NULL)
					{
						pResVar->Release();
						return NULL;
					}
					pReturnVariable	=pDstVar;
					pReturnVariable->AddRef();
					bSucess =TRUE;
				}
				break;
			default:
				{
					pDstVar		=pDstFactor->GetValueVariable();
					if(pDstVar == NULL)
					{
						pResVar->Release();
						return NULL;
					}
					pReturnVariable	=pDstVar;
					pReturnVariable->AddRef();
					bSucess =TRUE;
				}
				break;
			}
			LBS_SafeRelease(pResVar);
			LBS_SafeRelease(pDstVar);
		
		}
		break;
	case LBS_CT_MOD:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}

			pReturnVariable		=new CLBS_Variable(this);
			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->Mod(*pDstVar);

			pResVar->Release();
			pDstVar->Release();


		}
		break;
	case LBS_CT_AND:
		{//��

			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			
			pReturnVariable		=new CLBS_Variable(this);
			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->AND(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_OR:
		{//��
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pReturnVariable->Equal(*pResVar))
				bSucess=pReturnVariable->OR(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_XOR:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			pReturnVariable->Equal(*pResVar);
			bSucess=pReturnVariable->XOR(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LMB:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			pReturnVariable->Equal(*pResVar);
			bSucess=pReturnVariable->LMB(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_RMB:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			pReturnVariable->Equal(*pResVar);
			bSucess=pReturnVariable->RMB(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_AND:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			pReturnVariable->Equal(*pResVar);
			bSucess= TRUE;
			*pReturnVariable	=(LONG)pReturnVariable->LAND(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_OR:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			pReturnVariable->Equal(*pResVar);
			bSucess=TRUE;
			*pReturnVariable	=(LONG)pReturnVariable->LOR(*pDstVar);

			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_EQUAL:
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pResVar->Logic_E(*pDstVar))
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_NOTEQUAL:	//������
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pResVar->Logic_NE(*pDstVar))
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_BE:		//���ڵ���
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pResVar->Logic_BE(*pDstVar))
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	case LBS_CT_LOGIC_SE:		//С�ڵ���
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pResVar->Logic_SE(*pDstVar))
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();

		}
		break;
	case LBS_CT_LOGIC_B:			//����
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(pResVar->Logic_B(*pDstVar))
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();

		}
		break;
	case LBS_CT_LOGIC_S:			//С��
		{
			pDstVar		=pDstFactor->GetValueVariable();
			pResVar		=pResFactor->GetValueVariable();
			if(pResVar == NULL || pDstVar== NULL)
			{
				LBS_SafeRelease(pResVar);
				LBS_SafeRelease(pDstVar);
				return NULL;
			}
			pReturnVariable		=new CLBS_Variable(this);
			if(*pResVar < *pDstVar)
				*pReturnVariable	=(LONG)1;
			else
				*pReturnVariable	=(LONG)0;
			bSucess =TRUE;
			pResVar->Release();
			pDstVar->Release();
		}
		break;
	default:
		{
			ShowDebugTrace("#�����޷���������������[ %d ]( %s ).\r\n",
				pOperFactor->GetType(),
				pOperFactor->GetString());
		}
		break;
	}
	return pReturnVariable;
}
BOOL					CLBS_Statement_Calculate::PrepareProcessStatement()
{
	//TRACE("ԭʼ��%s\r\n",GetString());
	
	if(!PrepareCompile())
		return FALSE;
	//TRACE("Ԥ�ࣺ%s\r\n",GetString());
	
	if(!Compile())
		return FALSE;
	//TRACE("���룺%s\r\n",GetString());


	return TRUE;
}
BOOL					CLBS_Statement_Calculate::AddFactorByWord(LPSTR& IN OUT pCode,LPSTR IN pEnd,LPSTR IN pWordStart,LPSTR IN pWord)
{
	LPSTR	pRealWords					=pWord;
	CLBS_ScriptMacroDefine* pMacro		=(CLBS_ScriptMacroDefine*)m_pScriptFile->GetParser()->GetMacro(pRealWords);
	while(pMacro != NULL)
	{
		pRealWords	=(LPSTR)pMacro->GetValue();
		if(pMacro->IsString())
			break;
		pMacro	=(CLBS_ScriptMacroDefine*)m_pScriptFile->GetParser()->GetMacro(pRealWords);
	}
	if(pMacro != NULL)
	{//�ҵ��˸ú�
		if(pMacro->IsString())
		{//�ú��Ǹ��ִ�
			AddFactor(pRealWords,LBS_CT_STRING);
			return TRUE;
		}	
	}

	//����ؼ���ƥ��
	PKEYWORDSSTRUCT			pKeywords=NULL;
	if(IsMatchKeywords(pRealWords,pKeywords))
	{//�ǹؼ���
		switch(pKeywords->KeywordsType)
		{
		case KWT_CONST_TRUE:
			{
				BOOL v	=TRUE;
				AddFactor(&v,sizeof(BOOL),LBS_CT_INTEGER);
				return TRUE;
			}
			break;
		case KWT_CONST_FALSE:
			{
				BOOL v	=FALSE;
				AddFactor(&v,sizeof(BOOL),LBS_CT_INTEGER);
				return TRUE;
			}
			break;
		case KWT_CONST_NULL:
			{
				AddFactor(NULL,0,LBS_CT_STRING);
				return TRUE;
			}
			break;
		case KWT_CONST_CR:
			{
				AddFactor("\r",LBS_CT_STRING);
			}
			break;
		case KWT_CONST_LF:
			{
				AddFactor("\n",LBS_CT_STRING);
			}
			break;
		case KWT_CONST_TAB:
			{
				AddFactor("\t",LBS_CT_STRING);
			}
			break;
		default:
			break;
		}
	}




	switch(GetStringType(pRealWords))
	{///����-1 ��Ч��0���ִ���1��10��������  2: ������ 3��16��������
	case 0:
		{
			if(m_bVarDeclareStatement)
			{//����Ǳ����������Ļ������жϵ�ǰ�ǲ��ǵ�һ�����ӣ�����ǣ����ʾ��˳����������
				if(m_theFactorList.GetCount()==0)
				{
					if(IsMatchKeywords(pRealWords,pKeywords))
					{
						ShowDebugTrace("#�ؼ���[ %s ]Ϊ�����֣����ܶ���Ϊ������",pRealWords);
						return FALSE;
					}
					//��������
					CLBS_Variable*	pNewVar		=new CLBS_Variable(GetParent());
					pNewVar->Load(m_pScriptFile,pCode,pEnd);//���ñ���������Ϣ
					pNewVar->SetName(pRealWords);
					if(!GetParent()->AddVariable(pNewVar))
					{
						pNewVar->Release();
						return FALSE;
					}
					pNewVar->Release();
					//�������
					AddFactor(pRealWords,LBS_CT_VAR_NAME);
					return TRUE;
				}
			}


			LPSTR				pPP		=pCode;
			LPSTR				pWS		=NULL;
			PKEYWORDSSTRUCT		pKey;
			CHAR				word[KEY_WORDS_MAX_LEN];
			if(m_pScriptFile->GetNextWords(pPP,pEnd,pWS,word))
			{
				if(IsMatchKeywords(word,pKey,TRUE))
				{
					if(pKey->KeywordsType == KWT_PARA_LIST_START)
					{//��������
						pCode =pWordStart;
						CLBS_Function_Call*		pFuncCall =new CLBS_Function_Call(this,this);
						if(!pFuncCall->Load(m_pScriptFile,pCode,pEnd))
						{
							pFuncCall->Release();
							return FALSE;
						}
						AddFactor(&pFuncCall,sizeof(CLBS_Function_Call*),LBS_CT_FUNC_CALL);
						return TRUE;
					}
				}
			}

			if(m_theFactorList.GetCount()>0)
			{//����ǲ�������
				CLBS_Statement_CalculateFactor*		pPrev =(CLBS_Statement_CalculateFactor*	)m_theFactorList.GetObj(m_theFactorList.GetCount()-1);
                if (pPrev->GetFactorType() == LBS_CT_OPER_NEW )
                {
                    AddFactor(pRealWords, LBS_CT_CLASS_NAME);
                    return TRUE;
                }
                else if (pPrev->GetFactorType() == LBS_CT_OPER_DELETE)
                {
                    AddFactor(pRealWords, LBS_CT_VAR_NAME);
                    return TRUE;
                }
                

			}
			
			//������
			AddFactor(pRealWords,LBS_CT_VAR_NAME);
			return TRUE;
		}
		break;
	case 1:
		{
			LONG	value		=atoll(pRealWords);
			AddFactor(&value,sizeof(value),LBS_CT_INTEGER);
			return TRUE;
		}
		break;
	case 2:
		{
			DOUBLE	value		=atof(pRealWords);
			AddFactor(&value,sizeof(value),LBS_CT_FLOAT);
			return TRUE;
		}
		break;
	case 3:
		{
			DWORD	value		=0;
			sscanf(pRealWords,"%x",&value);
			AddFactor(&value,sizeof(value),LBS_CT_INTEGER);
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}
