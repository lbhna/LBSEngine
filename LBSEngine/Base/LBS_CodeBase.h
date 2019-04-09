#pragma once
#include "LBS_ObjBase.h"
#include "LBS_ObjHashListBase.h"
#include "../Script/LBS_ScriptFile.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Base\LBS_CodeBase.h
	created:	2012/05/08
	author:		LB
	purpose:	�������
\*********************************************************************/
typedef enum LBS_EXPORT _LBS_CER
{
	CER_FAILURE			=0x00,	//����ִ��ʧ��
	CER_SUCESS			=0x01,	//����ִ�гɹ�
	CER_BREAK	,				//��Ҫ�жϵ�ǰѭ��					//
	CER_RETURN	,				//���ص�ǰ��������
	CER_CONTINUE,				//���¼���ִ��

	////////////////////////////////////////////
	CER_forcedword	=0x7fffffff,
}LBS_CER;
typedef enum _LBS_API_ERRORCODE
{
	LBS_AEC_SUCESS				=0,		//ִ�гɹ�
	LBS_AEC_FAILURE,
	LBS_AEC_PARA_INVALID,				//�����Ƿ�
	LBS_AEC_PARA_TYPE_ERROR,			//�����������Ͳ���ȷ
	LBS_AEC_PARA_COUNT_ERROR,			//������������ȷ
	LBS_AEC_PARA_NOT_FOUND,				//����δ�ҵ�
	LBS_AEC_SYSTEM_CALL_ERROR,			//ϵͳ����ʧ��
	LBS_AEC_FUNCTION_NOT_DEFINATION,	//����δ����
	LBS_AEC_NOT_SUPPORT_METHOD,			//����֧�ָ÷���
	LBS_AEC_SCRIPT_NOT_VALID,			//�ű���ǰ��Ч
	
	//////////////////////////////
	LBS_AEC_forcedword	=0x7fffffff,
}LBS_API_ERRORCODE;

typedef CLBS_ObjBase* ( *LBS_API_FUNCTION)(CLBS_ObjListBase& IN theParaList,CLBS_ObjBase* IN pThis,DWORD& OUT dwErrorCode);


class LBS_EXPORT CLBS_ScriptFile;
class LBS_EXPORT CLBS_CodeBase :
	public CLBS_ObjBase
{
protected:
	CLBS_CodeBase*			m_pParent;		//������	
	CLBS_CodeBase*			m_pCallArea;	//���������

	CLBS_ScriptFile*		m_pScriptFile;	//�����ĸ��ű��ļ�
	DWORD					m_dwLineNo;		//��ʼ�к�
	LPSTR					m_pOriginCode;	//ԭʼ�ű�����

protected:
	CLBS_ObjListBase		m_theVariableList;		//�����б�
	CLBS_ObjBase*			m_pReturnVariable;		//����ֵ����
protected:
	CLBS_ObjListBase		m_theExecuteCodeList;	//��ִ�д����б�
	CLBS_ObjListBase		m_theMemberFunctionList;//��Ա�����б�
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
