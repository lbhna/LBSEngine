#include "StdAfx.h"
#include ".\lbs_statement_calculatefactor.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Variable.h"
#include "./LBS_Function_Call.h"

CLBS_Statement_CalculateFactor::CLBS_Statement_CalculateFactor(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_CALC_FACTOR;
	m_dwDataLen		=0;
	m_FactorType	=LBS_CT_UNKNOWN;
	m_pData			=NULL;
}

CLBS_Statement_CalculateFactor::CLBS_Statement_CalculateFactor(CLBS_CodeBase* IN pParent,VOID* IN pData,DWORD IN dwLen,LBS_CT_TYPE IN type):
CLBS_CodeBase(pParent)
{
	m_Type			=LOT_CODE_CALC_FACTOR;
	m_dwDataLen		=dwLen;
	m_FactorType	=type;
	m_pData			=LB_ALLOC(m_dwDataLen+1);
	memcpy(m_pData,pData,m_dwDataLen);
	((CHAR*)m_pData)[m_dwDataLen]=0;
}
CLBS_Statement_CalculateFactor::~CLBS_Statement_CalculateFactor(void)
{
	Reset();
}
VOID			CLBS_Statement_CalculateFactor::Reset()
{
	if(m_pData != NULL)
	{
		switch(m_FactorType)
		{
		case LBS_CT_VAR_OBJ:
			{
				((CLBS_Variable**)m_pData)[0]->Release();
			}
			break;
		case LBS_CT_CALC_STATEMENT:
			{
				((CLBS_Statement_Calculate**)m_pData)[0]->Release();
			}
			break;
		case LBS_CT_FUNC_CALL:
			{
				((CLBS_Function_Call**)m_pData)[0]->Release();
			}
			break;

		}
		LB_FREE(m_pData);
	}

	m_dwDataLen		=0;
	m_FactorType	=LBS_CT_UNKNOWN;
	LBS_SafeRelease(m_pReturnVariable);
}
VOID			CLBS_Statement_CalculateFactor::Set(VOID* IN pData,DWORD IN dwLen,LBS_CT_TYPE IN type)
{
	Reset();
	m_dwDataLen	= dwLen;
	m_FactorType= type;
	m_pData		= LB_ALLOC(m_dwDataLen+1);
	memcpy(m_pData,pData,m_dwDataLen);
	((CHAR*)m_pData)[m_dwDataLen]=0;
}
CLBS_Statement_CalculateFactor&	CLBS_Statement_CalculateFactor::operator=(CLBS_Statement_CalculateFactor& IN v)
{
	Set(v.m_pData,v.m_dwDataLen,v.m_FactorType);
	return *this;
}
CString			CLBS_Statement_CalculateFactor::GetString()
{
	CString		str;
	switch(m_FactorType)
	{
	case LBS_CT_INTEGER:		//��������
		{
			str.Format("%d",*((DWORD*)m_pData));
		}
		break;
	case LBS_CT_FLOAT:		//��������
		{
			str.Format("%f",*((DOUBLE*)m_pData));
		}
		break;
	case LBS_CT_STRING:		//�ִ�����
		{
			str.Format("\"%s\"",(CHAR*)m_pData);
		}
		break;
	case LBS_CT_VAR_NAME:
	case LBS_CT_CHILD_START:	//(//��ʽ
	case LBS_CT_CHILD_END:	//)
	case LBS_CT_SEPERATOR:	//:
	case LBS_CT_SUFFIX_START://[//�±�
	case LBS_CT_SUFFIX_END:	//]
	case LBS_CT_ADD:			//�ӷ�+
	case LBS_CT_SUB:			//����-
	case LBS_CT_MUL:			//�˷�*
	case LBS_CT_DIV:			//����/
	case LBS_CT_EQUAL:		//��ֵ=
	case LBS_CT_LOGIC_AND:	//�߼���
	case LBS_CT_LOGIC_OR: 	//�߼���
	case LBS_CT_LOGIC_NOT:	//�߼���
	case LBS_CT_AREA_CHILD:	//.	//����
	case LBS_CT_AREA_ORIGIN:	//:://����
	case LBS_CT_AND:
	case LBS_CT_OR:
	case LBS_CT_NOT:
	case LBS_CT_XOR:
	case LBS_CT_LMB:
	case LBS_CT_RMB:
	case LBS_CT_LOGIC_EQUAL:		//����
	case LBS_CT_LOGIC_NOTEQUAL:	//������
	case LBS_CT_LOGIC_BE:		//���ڵ���
	case LBS_CT_LOGIC_SE:		//С�ڵ���
	case LBS_CT_LOGIC_B:			//����
	case LBS_CT_LOGIC_S:			//С��
	case LBS_CT_MOD:
	case LBS_CT_CLASS_NAME:
	case LBS_CT_OPER_DELETE:
	case LBS_CT_OPER_NEW:
		{
			str.Format(" %s ",(CHAR*)m_pData);
		}
		break;
	case LBS_CT_VAR_OBJ:			//���������ַ
		{
			str.Format("%s",((CLBS_Variable**)m_pData)[0]->GetName());
		}
		break;
	case LBS_CT_CALC_STATEMENT://���ʽ
		{
			return ((CLBS_Statement_Calculate**)m_pData)[0]->GetString();
		}
		break;
	case LBS_CT_FUNC_CALL:	//��������
		{
			CLBS_Function_Call* pFuncCall =(((CLBS_Function_Call**)m_pData)[0]);
			str.Format("%s( ",pFuncCall->GetName());

			CString			tmp;
			CLBS_ObjBase*	pExeCode =NULL;
			for(LONG i=pFuncCall->GetExecuteCodeCount()-1;i>=0;i--)
			{
				pExeCode =pFuncCall->GetExecuteCode(i);
				if(pExeCode->GetType() == LOT_CODE_CALC_STATEMENT)
				{
					tmp = ((CLBS_Statement_Calculate*)pExeCode)->GetString();
					if(i!=0)tmp+=" , ";
					str+=tmp;
				}
			}
			str+=" )";
		}
		break;
	default:
		{
			str = " null ";
		}
		break;
	}
	return str;
}
FACTOR_CLASS	CLBS_Statement_CalculateFactor::GetFactorClass()
{
	switch(m_FactorType)
	{
	case LBS_CT_VAR_NAME:	//������
	case LBS_CT_VAR_OBJ:	//���������ַ
	case LBS_CT_INTEGER:	//��������
	case LBS_CT_FLOAT:		//��������
	case LBS_CT_STRING:		//�ִ�����
	case LBS_CT_FUNC_CALL:	//��������
	case LBS_CT_CALC_STATEMENT://���ʽ
	case LBS_CT_CLASS_NAME:
		{
			return LBS_FC_OPERFACTOR;
		}
		break;
	case LBS_CT_SEPERATOR:	//:
		{
			return LBS_FC_SEPERATOR;
		}
		break;
	case LBS_CT_ADD:			//�ӷ�+
	case LBS_CT_SUB:			//����-
	case LBS_CT_MUL:			//�˷�*
	case LBS_CT_DIV:			//����/
	case LBS_CT_EQUAL:		//��ֵ=
	case LBS_CT_LOGIC_AND:	//�߼���
	case LBS_CT_LOGIC_OR:	//�߼���
	case LBS_CT_AND:	//��
	case LBS_CT_OR:	//��
	case LBS_CT_XOR:	//���
	case LBS_CT_LMB:
	case LBS_CT_RMB:
	case LBS_CT_LOGIC_EQUAL:		//����
	case LBS_CT_LOGIC_NOTEQUAL:	//������
	case LBS_CT_LOGIC_BE:		//���ڵ���
	case LBS_CT_LOGIC_SE:		//С�ڵ���
	case LBS_CT_LOGIC_B:			//����
	case LBS_CT_LOGIC_S:			//С��
	case LBS_CT_MOD:
	case LBS_CT_SUFFIX_START://[//�±�
	case LBS_CT_SUFFIX_END:	//]
	case LBS_CT_AREA_CHILD:	//.	//����
	case LBS_CT_AREA_ORIGIN:	//:://����

		{
			return LBS_FC_DOUBLE_OPER;
		}
		break;
	case LBS_CT_NOT:
	case LBS_CT_LOGIC_NOT:
	case LBS_CT_OPER_NEW:
	case LBS_CT_OPER_DELETE:
		{
			return LBS_FC_SINGLE_OPER;
		}
		break;
	default:
		{
			return LBS_FC_INVALID;
		}
		break;
	}
	return LBS_FC_INVALID;
}
CLBS_Variable*	CLBS_Statement_CalculateFactor::GetValueVariable()
{
	
	switch(m_FactorType)
	{
	case LBS_CT_INTEGER:
		{
			if(m_pReturnVariable == NULL)
				m_pReturnVariable		=new CLBS_Variable(m_pParent);
			((CLBS_Variable*)m_pReturnVariable)->Set(m_pData,m_dwDataLen,LVT_INTEGER);
		}
		break;
	case LBS_CT_STRING:
		{
			if(m_pReturnVariable == NULL)
				m_pReturnVariable		=new CLBS_Variable(m_pParent);
			((CLBS_Variable*)m_pReturnVariable)->Set(m_pData,m_dwDataLen,LVT_STRING);
		}
		break;
	case LBS_CT_CLASS_NAME:
		{
			if(m_pReturnVariable == NULL)
				m_pReturnVariable		=new CLBS_Variable(m_pParent);
			((CLBS_Variable*)m_pReturnVariable)->Set(m_pData,m_dwDataLen,LVT_STRING);
		}
		break;
	case LBS_CT_FLOAT:
		{
			if(m_pReturnVariable == NULL)
				m_pReturnVariable		=new CLBS_Variable(m_pParent);
			((CLBS_Variable*)m_pReturnVariable)->Set(m_pData,m_dwDataLen,LVT_FLOAT);
		}
		break;
	case LBS_CT_VAR_NAME:
		{
			LBS_SafeRelease(m_pReturnVariable);
			if(m_pReturnVariable==NULL)
				m_pReturnVariable		=GetVariable((LPCSTR)m_pData);
			if(m_pReturnVariable != NULL)
			{
				m_pReturnVariable->AddRef();
			}
			else
			{
				ShowDebugTrace("#���󣺱���[%s]δ����!",(LPCSTR)m_pData);
			}
		}
		break;
	case LBS_CT_VAR_OBJ:
		{
			LBS_SafeRelease(m_pReturnVariable);
			m_pReturnVariable		=((CLBS_Variable**)m_pData)[0];
			if(m_pReturnVariable != NULL)
			{
				m_pReturnVariable->AddRef();
			}
		}
		break;
	case LBS_CT_CALC_STATEMENT:
		{
			LBS_SafeRelease(m_pReturnVariable);
			m_pReturnVariable		=(CLBS_Variable*) ((CLBS_Statement_Calculate**)m_pData)[0]->GetReturnVariable();
			if(m_pReturnVariable != NULL)
				m_pReturnVariable->AddRef();
		}
		break;
	case LBS_CT_FUNC_CALL:
		{
			LBS_SafeRelease(m_pReturnVariable);
			m_pReturnVariable		=(CLBS_Variable*) ((CLBS_Function_Call**)m_pData)[0]->GetReturnVariable();
			if(m_pReturnVariable != NULL)
				m_pReturnVariable->AddRef();
		}
		break;
	default:
		break;
	}

	//���һ�����ü����������ⲿ�ͷ�
	if(m_pReturnVariable != NULL)
		m_pReturnVariable->AddRef();
	return ((CLBS_Variable*)m_pReturnVariable);
}
