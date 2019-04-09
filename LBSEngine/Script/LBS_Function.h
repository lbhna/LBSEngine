#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Function.h
	created:	2012/05/11
	author:		LB
	purpose:	函数类
\*********************************************************************/
typedef enum LBS_EXPORT _LBS_FUNCTION_TYPE
{
	LFT_USER_FUNCTION	=0,			//用户定义函数
	LFT_SYS_FUNCTION,				//系统提供的函数
	LFT_APP_FUNCTION,				//应用层扩展的函数
	LFT_MEMBER_FUNCTION,			//对象成员函数

	LFT_FORCE_DWORD		=0x7fffffff,
}LBS_FUNCTION_TYPE;




class CLBS_Function :
	public CLBS_CodeBase
{
protected:
	LBS_FUNCTION_TYPE		m_FunctionType;
	CLBS_ObjListBase		m_theParameterVariableList;		//参数变量列表
	LBS_API_FUNCTION		m_pSysFuncAddr;					//
	LPSTR					m_pOriginBodyCode;				//原始函数体代码

public:
	CLBS_Function(CLBS_CodeBase* IN pParent,LBS_FUNCTION_TYPE IN funcType=LFT_USER_FUNCTION,LBS_API_FUNCTION IN pSysFuncAddr=NULL);
	virtual ~CLBS_Function(void);
public:
	LBS_FUNCTION_TYPE		GetFunctionType(){return m_FunctionType;}
	VOID					SetSysFuncAddr(LBS_API_FUNCTION IN pSysFuncAddr){m_pSysFuncAddr = pSysFuncAddr;}
	
	LPSTR					GetOriginBodyCode(){return m_pOriginBodyCode;}
	VOID					SetOriginBodyCode(LPSTR IN pCode,LONG IN lSize);


public:
	BOOL					AddParameterVariable(LPCSTR IN pVarName);	//添加参数变量
	VOID					AddParameterVariable(CLBS_ObjBase* IN pVar);
	CLBS_ObjBase*			GetParameterVariable(LONG IN lIndex);		//根据索引获取参数变量
	LONG					GetParameterVariableCount();				//得到形参数量
	VOID					ClearParameterVariable(){m_theParameterVariableList.Clear();}


public:
	virtual CLBS_ObjBase*	GetMemberVariable(LPCSTR IN pVarName);
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadFunctionHeader(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadFunctionBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	VOID					TraceCallFailureInfo(DWORD IN code);
};
