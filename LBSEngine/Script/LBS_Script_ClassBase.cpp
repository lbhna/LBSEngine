#include "StdAfx.h"
#include ".\lbs_script_classbase.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Function.h"

CLBS_Script_ClassBase::CLBS_Script_ClassBase(CLBS_Script_ClassBase* IN pPrototype,CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_pPrototype		=pPrototype;
	m_Type				=LOT_CODE_CLASS;
}

CLBS_Script_ClassBase::~CLBS_Script_ClassBase(void)
{

}
BOOL			CLBS_Script_ClassBase::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	if(!CLBS_CodeBase::Load(pFile,pCode,pEnd))
		return FALSE;
	CHAR				Word[KEY_WORDS_MAX_LEN];
	LPSTR				pParsePos	=pCode;
	LPSTR				pWordStart	=NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;

	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(IsMatchKeywords(Word,pKeywords))
	{
		ShowDebugTrace("#错误：关键字[ %s ]不能定义为类名！",Word);
		return FALSE;
	}
	SetName(Word);


	if(!m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
		return FALSE;
	if(!IsMatchKeywords(Word,pKeywords))
	{
		ShowDebugTrace("#错误：类定义不合法！",Word);
		return FALSE;
	}
	if(pKeywords->KeywordsType != KWT_CODE_BLOCK_START)
	{
		ShowDebugTrace("#错误：类定义不合法！",Word);
		return FALSE;
	}
	
	while(m_pScriptFile->GetNextWords(pParsePos,pEnd,pWordStart,Word))
	{
		if(!IsMatchKeywords(Word,pKeywords))
		{
			ShowDebugTrace("#错误：无法解释的单词[ %s ]！",Word);
			return FALSE;
		}
		switch(pKeywords->KeywordsType)
		{
		case KWT_CODE_BLOCK_END:
			{
				SetOriginCode(pCode,(LONG)(pParsePos-pCode));
				pCode =pParsePos;
				return TRUE;
			}
			break;
		case KWT_VARIABLE_DEFINE:
			{
				CLBS_Statement_Calculate*		pStatement =new CLBS_Statement_Calculate(this,TRUE);
				if(!pStatement->Load(m_pScriptFile,pParsePos,pEnd))
				{
					pStatement->Release();
					return FALSE;
				}
				AddExecuteCode(pStatement);
				pStatement->Release();
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
		default:
			{
				ShowDebugTrace("#错误：无法在类定义中处理关键字[ %s ]！",Word);
				return FALSE;
			}
			break;
		}
	}


	return FALSE;
}
BOOL			CLBS_Script_ClassBase::Save(CString& OUT strCode)
{
	return TRUE;
}
LONG					CLBS_Script_ClassBase::GetMemberFunctionCount()
{
	if(m_pPrototype == NULL)
		return m_theMemberFunctionList.GetCount();
	else
		return m_pPrototype->GetMemberFunctionCount();
}
CLBS_ObjBase*			CLBS_Script_ClassBase::GetMemberFunction(LPCSTR IN pFuncName)
{
	if(m_pPrototype == NULL)
		return m_theMemberFunctionList.FindObj(pFuncName);
	else
		return m_pPrototype->GetMemberFunction(pFuncName);	
}
LPCSTR							CLBS_Script_ClassBase::GetClassName()
{
	if(m_pPrototype != NULL)
		return m_pPrototype->GetName();
	return GetName();
}

CLBS_Script_ClassBase*			CLBS_Script_ClassBase::CreateInstance(CLBS_CodeBase* IN pCurrentArea,BOOL IN bInit)//创建一个实例对象
{
	CLBS_Script_ClassBase*		pNewInstance =new CLBS_Script_ClassBase(this,pCurrentArea);
	//新的实例有独立的数据，但是共享原型的方法
	
	CLBS_Variable*				pMemberVariable;
	CLBS_Variable*				pVar;
	for(LONG i=0;i<m_theVariableList.GetCount();i++)
	{
		pVar				=(CLBS_Variable*)m_theVariableList.GetObj(i);
		pMemberVariable		=new CLBS_Variable(pNewInstance);
		*pMemberVariable	=*pVar;
		
		pMemberVariable->SetName(pVar->GetName());
		pNewInstance->m_theVariableList.AddObj(pMemberVariable);
		pMemberVariable->Release();
	}

	if(bInit)
	{
		CLBS_CodeBase*	pCode =NULL;
		for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
		{
			pCode =(CLBS_CodeBase*)m_theExecuteCodeList.GetObj(i);
			pCode->SetCallArea(pNewInstance);
			if(!pCode->Execute())
			{
				pNewInstance->Release();
				return NULL;
			}

		}
		CLBS_Function*			pFunc	=(CLBS_Function*)GetMemberFunction(GetName());
		//得到构造函数
		if(pFunc != NULL)
		{
			pFunc->SetCallArea(pNewInstance);
			if(!pFunc->Execute())
			{
				pNewInstance->Release();
				return NULL;
			}
		}
	}


	return pNewInstance;
}
CLBS_Script_ClassBase&			CLBS_Script_ClassBase::operator=(CLBS_Script_ClassBase& v)
{
	if(IsPrototypeClass())
		return *this;
	if(strcmp(GetClassName(),v.GetClassName()) != 0)
		return *this;

	CLBS_Variable	*pRes,*pDst;
	for(LONG i=0;i<m_theVariableList.GetCount();i++)
	{
		pRes =(CLBS_Variable*)m_theVariableList.GetObj(i);
		pDst =(CLBS_Variable*)v.GetMemberVariable(i);

		*pRes =*pDst;
	}
	return *this;
}
CLBS_Script_ClassBase*			CLBS_Script_ClassBase::GetClone()
{
	if(GetPrototype() == NULL)
		return NULL;
	CLBS_Script_ClassBase*	pClone	=GetPrototype()->CreateInstance(GetParent(),FALSE);
	if(pClone == NULL)
		return NULL;
	*pClone		=*this;
	return pClone;
}
void            CLBS_Script_ClassBase::CallDestructFunction()
{
    if (m_pPrototype != NULL)
    {
        CLBS_Function*	pFinalizeFunc = (CLBS_Function*)m_pPrototype->GetMemberFunction("Finalize");
        if (pFinalizeFunc != NULL)
        {
            pFinalizeFunc->Execute();
        }
    }
}
BOOL					CLBS_Script_ClassBase::ConstructVariableFromMem(LPVOID IN pMem,DWORD& IN OUT dwSize)
{
	DWORD			pos			=0;
	DWORD			size		=0;
	CLBS_Variable*	pVar		=NULL;

	for(LONG i=0;i<m_theVariableList.GetCount();i++)
	{
		pVar	=(CLBS_Variable*)m_theVariableList.GetObj(i);
		size	=pVar->GetDataSize();
		
		if(size == 0)
			return FALSE;
		if( size > dwSize)
			return FALSE;

		if(pVar->GetVarType() == LVT_CLASS)
		{
			CLBS_Script_ClassBase*	pClass =*(CLBS_Script_ClassBase**)pVar->GetDataBuffer();
			DWORD oldSize =dwSize;
			pClass->ConstructVariableFromMem((BYTE*)pMem + pos,dwSize);
			size =oldSize-dwSize;
		}
		else
		{
			pVar->Set((BYTE*)pMem + pos,size,pVar->GetVarType());
			dwSize	-=size;
		}
		pos		+=size;
	}
	

	return TRUE;
}
