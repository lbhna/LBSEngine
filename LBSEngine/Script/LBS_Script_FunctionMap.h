#pragma once
#include "../Base/LBS_ObjHashListBase.h"
#include "../Base/LBS_CodeBase.h"

/********************************************************************
	created:	2012-5-31   8:51
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_FunctionMap.h
	author:		LB
	
	purpose:	����ӳ����
*********************************************************************/


class CLBS_Script_FunctionMap
{
protected:
	static CLBS_Script_FunctionMap*		m_pSingleton;

protected:
	CLBS_ObjHashListBase				m_theVariablePrivateFunctionMap;	//������˽�к���ӳ��
	CLBS_ObjHashListBase				m_theSystemApiFunctionMap;			//ϵͳAPI����ӳ��
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
	//��ӱ�����˽�к���
	BOOL								AddVariablePrivateFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//���ݺ�������ȡ������˽�к���
	CLBS_ObjBase*						GetVariablePrivateFunction(LPCSTR IN pFuncName);

	//���ϵͳAPI����
	BOOL								AddSystemApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//���Ӧ�ó���API����
	BOOL								AddApplicationApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr);
	//���ݺ�������ȡϵͳAPI����
	CLBS_ObjBase*						GetFunction(LPCSTR IN pFuncName);
public:
	DWORD								GetKernelFunctionCount();
	CLBS_ObjBase*						GetFirstKernelFunction();
	CLBS_ObjBase*						GetNextKernelFunction();
};
