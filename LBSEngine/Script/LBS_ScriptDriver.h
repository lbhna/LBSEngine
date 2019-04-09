#pragma once
/********************************************************************
	created:	2012-6-6   14:39
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_ScriptDriver.h
	author:		LB
	
	purpose:	脚本驱动入口类
*********************************************************************/

#include "./Base/GlobalDefine.h"
#include "..\Base\LBS_ObjBase.h"
#include "..\Syntax\LBS_SyntaxParser.h"
#include "./LBS_Variable.h"
#include "./LBS_Script_CallFunctionPara.h"
class LBS_EXPORT CLBS_SyntaxParser;
class LBS_EXPORT CLBS_ScriptDriver :
	public CLBS_ObjBase
{
protected:
	static CLBS_ScriptDriver*	m_pSingleton;
protected:
	CLBS_SyntaxParser*			m_pScriptParser;
public:
	CLBS_ScriptDriver(void);
	virtual ~CLBS_ScriptDriver(void);
public:
	static CLBS_ScriptDriver&	GetInstance();
	static VOID					ExitInstance();
public:
	CLBS_SyntaxParser*			GetScriptParser(){return m_pScriptParser;}


	BOOL						LoadScript(LPCSTR IN pFileName);
	BOOL						LoadScriptFromMemory(VOID* IN pMem,DWORD IN dwLen);

	BOOL						AddSystemFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);

	BOOL						Execute();
	CLBS_Variable*				CallScriptFunction(LPCSTR IN pFuncName,CLBS_Script_CallFunctionPara& IN Parameter,DWORD* OUT pErrorCode=NULL);
	CLBS_Variable*				ExecuteScriptFunction(CLBS_CodeBase* IN pFunction,CLBS_Script_CallFunctionPara& IN Parameter,DWORD* OUT pErrorCode=NULL);

public:
	CHAR*						GetEncCode(LPCSTR IN pCode);
	CHAR*						GetDecCode(LPCSTR IN pEncCode);
	VOID						FreeEncOrDecCode(CHAR* IN pCode);
public:
	CLBS_ObjBase*				GetFirstKernelFunction();
	CLBS_ObjBase*				GetNextKernelFunction();
	DWORD						GetKernelFunctionCount();
	CString						GetLastError();
public:
	virtual VOID				OnInitialize(){};
};
