#pragma once
#include "../Base/LBS_ObjHashListBase.h"
#include "../Base/LBS_CodeBase.h"

/********************************************************************
	created:	2012-5-31   8:51
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_FunctionMap.h
	author:		LB
	
	purpose:	函数映射类
*********************************************************************/


class CLBS_Script_FunctionMap
{
protected:
	static CLBS_Script_FunctionMap*		m_pSingleton;

protected:
	CLBS_ObjHashListBase				m_theVariablePrivateFunctionMap;	//变量的私有函数映射
	CLBS_ObjHashListBase				m_theSystemApiFunctionMap;			//系统API函数映射
protected:
	LONG								m_lGetIndex;
	POSITION							m_GetPosition;
	BOOL								m_bGetVariableFunc;

public:
	CLBS_Script_FunctionMap(void);
	~CLBS_Script_FunctionMap(void);
public:
	static CLBS_Script_FunctionMap&		GetInstance();
	static VOID							ExitInstance();
public:
	//添加变量的私有函数
	BOOL								AddVariablePrivateFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//根据函数名获取变量的私有函数
	CLBS_ObjBase*						GetVariablePrivateFunction(LPCSTR IN pFuncName);

	//添加系统API函数
	BOOL								AddSystemApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//添加应用程序API函数
	BOOL								AddApplicationApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//根据函数名获取系统API函数
	CLBS_ObjBase*						GetFunction(LPCSTR IN pFuncName);
public:
	DWORD								GetKernelFunctionCount();
	CLBS_ObjBase*						GetFirstKernelFunction();
	CLBS_ObjBase*						GetNextKernelFunction();
};
