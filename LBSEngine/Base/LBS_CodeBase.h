#pragma once
#include "LBS_ObjBase.h"
#include "LBS_ObjHashListBase.h"
#include "../Script/LBS_ScriptFile.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Base\LBS_CodeBase.h
	created:	2012/05/08
	author:		LB
	purpose:	代码基类
\*********************************************************************/
typedef enum LBS_EXPORT _LBS_CER
{
	CER_FAILURE			=0x00,	//代码执行失败
	CER_SUCESS			=0x01,	//代码执行成功
	CER_BREAK	,				//需要中断当前循环					//
	CER_RETURN	,				//返回当前函数调用
	CER_CONTINUE,				//重新继续执行

	////////////////////////////////////////////
	CER_forcedword	=0x7fffffff,
}LBS_CER;
typedef enum _LBS_API_ERRORCODE
{
	LBS_AEC_SUCESS				=0,		//执行成功
	LBS_AEC_FAILURE,
	LBS_AEC_PARA_INVALID,				//参数非法
	LBS_AEC_PARA_TYPE_ERROR,			//参数变量类型不正确
	LBS_AEC_PARA_COUNT_ERROR,			//参数数量不正确
	LBS_AEC_PARA_NOT_FOUND,				//参数未找到
	LBS_AEC_SYSTEM_CALL_ERROR,			//系统调用失败
	LBS_AEC_FUNCTION_NOT_DEFINATION,	//函数未定义
	LBS_AEC_NOT_SUPPORT_METHOD,			//对象不支持该方法
	LBS_AEC_SCRIPT_NOT_VALID,			//脚本当前无效
	
	//////////////////////////////
	LBS_AEC_forcedword	=0x7fffffff,
}LBS_API_ERRORCODE;

typedef CLBS_ObjBase* ( *LBS_API_FUNCTION)(CLBS_ObjListBase& IN theParaList,CLBS_ObjBase* IN pThis,DWORD& OUT dwErrorCode);


class LBS_EXPORT CLBS_ScriptFile;
class LBS_EXPORT CLBS_CodeBase :
	public CLBS_ObjBase
{
protected:
	CLBS_CodeBase*			m_pParent;		//父对象	
	CLBS_CodeBase*			m_pCallArea;	//调用域对象

	CLBS_ScriptFile*		m_pScriptFile;	//属于哪个脚本文件
	DWORD					m_dwLineNo;		//起始行号
	LPSTR					m_pOriginCode;	//原始脚本代码

protected:
	CLBS_ObjListBase		m_theVariableList;		//变量列表
	CLBS_ObjBase*			m_pReturnVariable;		//返回值变量
protected:
	CLBS_ObjListBase		m_theExecuteCodeList;	//可执行代码列表
	CLBS_ObjListBase		m_theMemberFunctionList;//成员函数列表
protected:
	BOOL					m_bIsReturn;
	BOOL					m_bIsBreak;
	BOOL					m_bIsContinue;
public:
	CLBS_CodeBase(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_CodeBase(void);
public:
	CLBS_CodeBase*			GetParent(){return m_pParent;}
	VOID					SetParent(CLBS_CodeBase* IN pParent){m_pParent=pParent;}

	CLBS_CodeBase*			GetCallArea(){return m_pCallArea;}
	VOID					SetCallArea(CLBS_CodeBase* IN pArea){m_pCallArea=pArea;}

	DWORD					GetLineNo(){return m_dwLineNo;}
	VOID					SetScriptFile(CLBS_ScriptFile* IN pFile){m_pScriptFile=pFile;}
	CLBS_ScriptFile*		GetScriptFile(){return m_pScriptFile;}
	CLBS_SyntaxParser*		GetParser();

	CLBS_CodeBase*			GetRoot();

	LPCSTR					GetOriginCode(){return (LPCSTR)m_pOriginCode;}
	VOID					SetOriginCode(LPCSTR IN pCode);
	VOID					SetOriginCode(LPCSTR IN pCode,LONG IN lSize);


	BOOL					IsReturn(){return m_bIsReturn;}
	VOID					SetIsReturn(BOOL IN bReturn){m_bIsReturn=bReturn;}

	BOOL					IsBreak(){return m_bIsBreak;}
	VOID					SetIsBreak(BOOL IN bBreak){m_bIsBreak=bBreak;}

	BOOL					IsContinue(){return m_bIsContinue;}
	VOID					SetIsContinue(BOOL IN bContinue){m_bIsContinue=bContinue;}



public:
	virtual CLBS_ObjBase*	GetVariable(LPCSTR IN pVarName,BOOL IN bIncludeParent=TRUE);
	virtual CLBS_ObjBase*	GetMemberVariable(LPCSTR IN pVarName);
	virtual CLBS_ObjBase*	GetMemberVariable(LONG IN lIndex){return m_theVariableList.GetObj(lIndex);}
	virtual LONG			GetMemberVariableCount(){return m_theVariableList.GetCount();}
	virtual BOOL			AddVariable(CLBS_ObjBase* IN pObj);
	virtual VOID			ClearVariable(){m_theVariableList.Clear();}
	virtual CLBS_ObjBase*	GetReturnVariable(){return m_pReturnVariable;}
	virtual VOID			SetReturnVariable(CLBS_ObjBase* IN pRetVariable);

public:
	virtual LONG			GetExecuteCodeCount(){return m_theExecuteCodeList.GetCount();}
	virtual CLBS_ObjBase*	GetExecuteCode(LONG IN lIndex){return m_theExecuteCodeList.GetObj(lIndex);}
	virtual VOID			ClearExecuteCode(){m_theExecuteCodeList.Clear();}
	virtual VOID			AddExecuteCode(CLBS_ObjBase* IN pExecuteCode){m_theExecuteCodeList.AddObj(pExecuteCode);}
public:
	virtual LONG			GetMemberFunctionCount(){return m_theMemberFunctionList.GetCount();}
	virtual VOID			ClearMemberFunction(){m_theMemberFunctionList.Clear();}
	virtual CLBS_ObjBase*	GetMemberFunction(LPCSTR IN pFuncName);
	virtual CLBS_ObjBase*	GetMemberFunction(LONG IN lIndex);
	virtual CLBS_ObjBase*	GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent=TRUE);
	virtual BOOL			AddMemberFunction(CLBS_ObjBase* IN pFunc);
	virtual VOID			RemoveMemberFunction(LPCSTR IN pFuncName);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
	virtual VOID			ShowDebugTrace(LPCSTR IN pszFormat,...);

};
