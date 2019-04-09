#include "StdAfx.h"
#include ".\lbs_codebase.h"
#include "../Script/LBS_Variable.h"
#include "../Script/LBS_Function.h"

CLBS_CodeBase::CLBS_CodeBase(CLBS_CodeBase* IN pParent)
{
	m_pParent			=pParent;
	m_pCallArea			=pParent;

	m_Type				=LOT_CODE_BASE;
	m_pScriptFile		=NULL;
	m_dwLineNo			=0;	
	m_pReturnVariable	=NULL;		//返回值变量

	m_pOriginCode		=NULL;
	m_bIsReturn			=FALSE;
	m_bIsBreak			=FALSE;
	m_bIsContinue		=FALSE;

	if(m_pParent != NULL)
	{
		m_pScriptFile	=m_pParent->GetScriptFile();
		m_dwLineNo		=m_pParent->GetLineNo();
	}
}

CLBS_CodeBase::~CLBS_CodeBase(void)
{
	LB_FREE((LPVoid&)m_pOriginCode);
	m_theExecuteCodeList.Clear();
	m_theVariableList.Clear();
	m_theMemberFunctionList.Clear();
	LBS_SafeRelease(m_pReturnVariable);
}
CLBS_CodeBase*	CLBS_CodeBase::GetRoot()
{
	CLBS_CodeBase*	pRoot	=this;
	CLBS_CodeBase*	pParent =GetParent();
	while(pParent != NULL)
	{
		pRoot	=pParent;
		pParent =pParent->GetParent();
	}
	return pRoot;
}
CLBS_SyntaxParser*		CLBS_CodeBase::GetParser()
{
	if(GetType() == LOT_SCRIPT_PARSER)
		return (CLBS_SyntaxParser*)this;
	return GetScriptFile()->GetParser();
}


VOID			CLBS_CodeBase::SetOriginCode(LPCSTR IN pCode)
{
	SetOriginCode(pCode,(LONG)strlen(pCode));
}
VOID			CLBS_CodeBase::SetOriginCode(LPCSTR IN pCode,LONG IN lSize)
{
    LB_FREE((LPVoid&)m_pOriginCode);
	m_pOriginCode	=(LPSTR)LB_ALLOC(lSize+1);
	strncpy(m_pOriginCode,pCode,lSize);
	m_pOriginCode[lSize]=0;
}

BOOL			CLBS_CodeBase::Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd)
{
	m_theExecuteCodeList.Clear();
	m_theVariableList.Clear();
	m_theMemberFunctionList.Clear();
	LBS_SafeRelease(m_pReturnVariable);

	m_pScriptFile	=pFile;
	if(m_pScriptFile == NULL)
		return FALSE;
	m_dwLineNo		=m_pScriptFile->GetCurrentLineNum();
	return TRUE;
}
BOOL			CLBS_CodeBase::Save(CString& OUT strCode)
{
	return TRUE;
}

inline CLBS_ObjBase*	CLBS_CodeBase::GetMemberVariable(LPCSTR IN pVarName)
{

	return m_theVariableList.FindObj(pVarName);
}

CLBS_ObjBase*	CLBS_CodeBase::GetVariable(LPCSTR IN pVarName,BOOL IN bIncludeParent)
{
	CLBS_ObjBase*	pVar	=GetMemberVariable(pVarName);
	if(pVar!=NULL)
		return pVar;

	
	if(m_pCallArea != NULL)
	{
		pVar	=m_pCallArea->GetVariable(pVarName,bIncludeParent);
		if(pVar != NULL)
			return pVar;
	}	
	if(bIncludeParent)
	{
		if(m_pParent != NULL && m_pParent!=m_pCallArea)
		{
			pVar	=m_pParent->GetVariable(pVarName,bIncludeParent);
			if(pVar != NULL)
				return pVar;
		}
	}	


	return NULL;
}
BOOL			CLBS_CodeBase::AddVariable(CLBS_ObjBase* IN pObj)
{
	if(pObj->GetType() != LOT_CODE_VARIABLE)
	{
		pObj->ShowDebugTrace("#错误：添加变量失败！对象[%s]不是变量类型!\r\n",pObj->GetName());
		return FALSE;
	}
	if(IsDigit(pObj->GetName()[0]))
	{
		pObj->ShowDebugTrace("#错误：添加变量失败！变量[ %s ]名字不合法，不能以数字为首字母！!\r\n",pObj->GetName());
		return FALSE;
	}	
	if(GetMemberVariable(pObj->GetName()) != NULL)
	{//变量重复定义
		pObj->ShowDebugTrace("#错误：添加变量失败！变量[ %s ]重复定义!\r\n",pObj->GetName());
		return FALSE;
	}

	//TRACE("添加变量：%s\r\n",pObj->GetName());

	m_theVariableList.AddObj(pObj);
	return TRUE;
}
VOID			CLBS_CodeBase::SetReturnVariable(CLBS_ObjBase* IN pRetVariable)
{
	LBS_SafeRelease(m_pReturnVariable);
	m_pReturnVariable =pRetVariable;
	if(m_pReturnVariable != NULL)
		m_pReturnVariable->AddRef();
}

BOOL			CLBS_CodeBase::Execute()
{
	CLBS_CodeBase*	pCode =NULL;
	SetIsReturn(FALSE);
	SetIsBreak(FALSE);
	SetIsContinue(FALSE);

	for(LONG i=0;i<m_theExecuteCodeList.GetCount();i++)
	{
		pCode	=(CLBS_CodeBase*)m_theExecuteCodeList.GetObj(i);
		pCode->SetCallArea(GetCallArea());
		if(!pCode->Execute())
			return FALSE;
		if(pCode->IsReturn())
		{
			if(GetParent()->GetType() != LOT_CODE_FUNCTION)
			{
				SetIsReturn(TRUE);
				SetReturnVariable(pCode->GetReturnVariable());
			}
			break;
		}
	}


	return TRUE;
}
CLBS_ObjBase*	CLBS_CodeBase::GetMemberFunction(LPCSTR IN pFuncName)
{
	return m_theMemberFunctionList.FindObj(pFuncName);
}
CLBS_ObjBase*	CLBS_CodeBase::GetMemberFunction(LONG IN lIndex)
{
	return m_theMemberFunctionList.GetObj(lIndex);
}
VOID	CLBS_CodeBase::RemoveMemberFunction(LPCSTR IN pFuncName)
{
	CLBS_ObjBase*	pObj	=m_theMemberFunctionList.FindObj(pFuncName);
	m_theMemberFunctionList.DelObj(pObj);
}

CLBS_ObjBase*	CLBS_CodeBase::GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent)
{
	CLBS_ObjBase*	pFunc	=GetMemberFunction(pFuncName);
	if(pFunc != NULL)
		return pFunc;
	if(m_pCallArea != NULL)
	{
		pFunc	=m_pCallArea->GetFunction(pFuncName,bIncludeParent);
		if(pFunc != NULL)
			return pFunc;
	}	
	if(bIncludeParent)
	{
		if(m_pParent != NULL && m_pParent!=m_pCallArea)
		{
			pFunc	=m_pParent->GetFunction(pFuncName,bIncludeParent);
			if(pFunc != NULL)
				return pFunc;
		}
	}
	return NULL;
}
BOOL			CLBS_CodeBase::AddMemberFunction(CLBS_ObjBase* IN pFunc)
{
	if(pFunc->GetType() != LOT_CODE_FUNCTION)
		return FALSE;
	if(GetMemberFunction(pFunc->GetName())!=NULL)
		return FALSE;
	m_theMemberFunctionList.AddObj(pFunc);
	return TRUE;
}

VOID			CLBS_CodeBase::ShowDebugTrace(LPCSTR IN pszFormat,...)
{
	CString		info;
	va_list argList;
	va_start( argList, pszFormat );
	info.FormatV( pszFormat, argList );
	va_end( argList );

	CString str;
	if(m_pScriptFile == NULL)
	{
		str.Format("%s  .\r\n",info);
		//TRACE(str);
		CErrorSystem::GetInstance().AddErrorInfo(NULL,0,str);
	}
	else
	{
		str.Format("%s  >> FILE:%s  LINE: %d .\r\n",info,m_pScriptFile->GetFileName(),GetLineNo());
		//TRACE(str);
		
		CErrorSystem::GetInstance().AddErrorInfo(m_pScriptFile->GetFileName(),GetLineNo(),str);
	}
	/////////////////////
	/////////////////////
}
