#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Function.h
	created:	2012/05/11
	author:		LB
	purpose:	������
\*********************************************************************/
typedef enum LBS_EXPORT _LBS_FUNCTION_TYPE
{
	LFT_USER_FUNCTION	=0,			//�û����庯��
	LFT_SYS_FUNCTION,				//ϵͳ�ṩ�ĺ���
	LFT_APP_FUNCTION,				//Ӧ�ò���չ�ĺ���
	LFT_MEMBER_FUNCTION,			//�����Ա����

	LFT_FORCE_DWORD		=0x7fffffff,
}LBS_FUNCTION_TYPE;




class CLBS_Function :
	public CLBS_CodeBase
{
protected:
	LBS_FUNCTION_TYPE		m_FunctionType;
	CLBS_ObjListBase		m_theParameterVariableList;		//���������б�
	LBS_API_FUNCTION		m_pSysFuncAddr;					//
	LPSTR					m_pOriginBodyCode;				//ԭʼ���������

public:
	CLBS_Function(CLBS_CodeBase* IN pParent,LBS_FUNCTION_TYPE IN funcType=LFT_USER_FUNCTION,LBS_API_FUNCTION IN pSysFuncAddr=NULL);
	virtual ~CLBS_Function(void);
public:
	LBS_FUNCTION_TYPE		GetFunctionType(){return m_FunctionType;}
	VOID					SetSysFuncAddr(LBS_API_FUNCTION IN pSysFuncAddr){m_pSysFuncAddr = pSysFuncAddr;}
	
	LPSTR					GetOriginBodyCode(){return m_pOriginBodyCode;}
	VOID					SetOriginBodyCode(LPSTR IN pCode,LONG IN lSize);


public:
	BOOL					AddParameterVariable(LPCSTR IN pVarName);	//��Ӳ�������
	VOID					AddParameterVariable(CLBS_ObjBase* IN pVar);
	CLBS_ObjBase*			GetParameterVariable(LONG IN lIndex);		//����������ȡ��������
	LONG					GetParameterVariableCount();				//�õ��β�����
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
