#pragma once
#include "../Base/LBS_CodeBase.h"
#include "./LBS_Variable.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_CalculateFactor.h
	created:	2012/05/11
	author:		LB
	purpose:	������������
\*********************************************************************/

typedef enum LBS_EXPORT _LBS_CALCULATE_FACTOR_TYPE
{
	LBS_CT_UNKNOWN	=0,//δ֪

	LBS_CT_VAR_NAME,	//������
	LBS_CT_INTEGER,		//��������
	LBS_CT_FLOAT,		//��������
	LBS_CT_STRING,		//�ִ�����
	LBS_CT_FUNC_CALL,	//��������
	LBS_CT_CHILD_START,	//(//��ʽ
	LBS_CT_CHILD_END,	//)
	LBS_CT_SEPERATOR,	//,
	LBS_CT_SUFFIX_START,//[//�±�
	LBS_CT_SUFFIX_END,	//]

	LBS_CT_ADD,			//�ӷ�+
	LBS_CT_SUB,			//����-
	
	LBS_CT_MUL,			//�˷�*
	LBS_CT_DIV,			//����/
	LBS_CT_MOD,			//ģ��
	
	LBS_CT_EQUAL,		//��ֵ=
	
	LBS_CT_LOGIC_AND,	//�߼���
	LBS_CT_LOGIC_OR,	//�߼���
	LBS_CT_LOGIC_NOT,	//�߼���
	
	LBS_CT_LOGIC_EQUAL,		//����
	LBS_CT_LOGIC_NOTEQUAL,	//������
	LBS_CT_LOGIC_BE,		//���ڵ���
	LBS_CT_LOGIC_SE,		//С�ڵ���
	LBS_CT_LOGIC_B,			//����
	LBS_CT_LOGIC_S,			//С��

	
	LBS_CT_AND,	//��
	LBS_CT_OR,	//��
	LBS_CT_NOT,	//��
	LBS_CT_XOR,	//���
	LBS_CT_LMB,	//����
	LBS_CT_RMB,	//����

	LBS_CT_AREA_CHILD,	//.	//����
	LBS_CT_AREA_ORIGIN,	//:://����

	LBS_CT_OPER_NEW,		//new
	LBS_CT_OPER_DELETE,		//delete
	LBS_CT_CLASS_NAME,		//����
	////////////////////////////////
	LBS_CT_VAR_OBJ,			//���������ַ
	LBS_CT_CALC_STATEMENT,//���ʽ
	LBS_CT_FORCEDWORD =0x7fffffff,

}LBS_CT_TYPE;
typedef enum LBS_EXPORT _FACTOR_CLASS
{
	LBS_FC_INVALID =0,
	LBS_FC_DOUBLE_OPER,		//˫Ŀ��������������Դ������
	LBS_FC_SINGLE_OPER,		//��Ŀ������������Դ������
	LBS_FC_OPERFACTOR,		//������
	LBS_FC_SEPERATOR,		//�ָ��
	LBS_FC_FORCEDWORD	=0x7fffffff,
}FACTOR_CLASS;
class CLBS_Statement_CalculateFactor :
	public CLBS_CodeBase
{
protected:
	VOID*			m_pData;
	DWORD			m_dwDataLen;
	LBS_CT_TYPE		m_FactorType;		//����

public:
	CLBS_Statement_CalculateFactor(CLBS_CodeBase* IN pParent);
	CLBS_Statement_CalculateFactor(CLBS_CodeBase* IN pParent,VOID* IN pData,DWORD IN dwLen,LBS_CT_TYPE IN type);
	virtual ~CLBS_Statement_CalculateFactor(void);
public:
	LBS_CT_TYPE		GetFactorType(){return m_FactorType;}
	DWORD			GetFactorDataLen(){return m_dwDataLen;}
	VOID*			GetFactorData(){return m_pData;}

	CLBS_Variable*	GetValueVariable();

	FACTOR_CLASS	GetFactorClass();
public:
	VOID			Reset();
	VOID			Set(VOID* IN pData,DWORD IN dwLen,LBS_CT_TYPE IN type);
	CLBS_Statement_CalculateFactor&	operator=(CLBS_Statement_CalculateFactor& IN v);
public:
	CString			GetString();
};
