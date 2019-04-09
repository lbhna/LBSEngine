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
	case LBS_CT_INTEGER:		//整数数据
		{
			str.Format("%d",*((DWORD*)m_pData));
		}
		break;
	case LBS_CT_FLOAT:		//浮点数据
		{
			str.Format("%f",*((DOUBLE*)m_pData));
		}
		break;
	case LBS_CT_STRING:		//字串数据
		{
			str.Format("\"%s\"",(CHAR*)m_pData);
		}
		break;
	case LBS_CT_VAR_NAME:
	case LBS_CT_CHILD_START:	//(//子式
	case LBS_CT_CHILD_END:	//)
	case LBS_CT_SEPERATOR:	//:
	case LBS_CT_SUFFIX_START://[//下标
	case LBS_CT_SUFFIX_END:	//]
	case LBS_CT_ADD:			//加法+
	case LBS_CT_SUB:			//减法-
	case LBS_CT_MUL:			//乘法*
	case LBS_CT_DIV:			//除法/
	case LBS_CT_EQUAL:		//赋值=
	case LBS_CT_LOGIC_AND:	//逻辑与
	case LBS_CT_LOGIC_OR: 	//逻辑或
	case LBS_CT_LOGIC_NOT:	//逻辑非
	case LBS_CT_AREA_CHILD:	//.	//子域
	case LBS_CT_AREA_ORIGIN:	//:://根域
	case LBS_CT_AND:
	case LBS_CT_OR:
	case LBS_CT_NOT:
	case LBS_CT_XOR:
	case LBS_CT_LMB:
	case LBS_CT_RMB:
	case LBS_CT_LOGIC_EQUAL:		//等于
	case LBS_CT_LOGIC_NOTEQUAL:	//不等于
	case LBS_CT_LOGIC_BE:		//大于等于
	case LBS_CT_LOGIC_SE:		//小于等于
	case LBS_CT_LOGIC_B:			//大于
	case LBS_CT_LOGIC_S:			//小于
	case LBS_CT_MOD:
	case LBS_CT_CLASS_NAME:
	case LBS_CT_OPER_DELETE:
	case LBS_CT_OPER_NEW:
		{
			str.Format(" %s ",(CHAR*)m_pData);
		}
		break;
	case LBS_CT_VAR_OBJ:			//变量对象地址
		{
			str.Format("%s",((CLBS_Variable**)m_pData)[0]->GetName());
		}
		break;
	case LBS_CT_CALC_STATEMENT://表达式
		{
			return ((CLBS_Statement_Calculate**)m_pData)[0]->GetString();
		}
		break;
	case LBS_CT_FUNC_CALL:	//函数调用
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
	case LBS_CT_VAR_NAME:	//变量名
	case LBS_CT_VAR_OBJ:	//变量对象地址
	case LBS_CT_INTEGER:	//整数数据
	case LBS_CT_FLOAT:		//浮点数据
	case LBS_CT_STRING:		//字串数据
	case LBS_CT_FUNC_CALL:	//函数调用
	case LBS_CT_CALC_STATEMENT://表达式
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
	case LBS_CT_ADD:			//加法+
	case LBS_CT_SUB:			//减法-
	case LBS_CT_MUL:			//乘法*
	case LBS_CT_DIV:			//除法/
	case LBS_CT_EQUAL:		//赋值=
	case LBS_CT_LOGIC_AND:	//逻辑与
	case LBS_CT_LOGIC_OR:	//逻辑或
	case LBS_CT_AND:	//与
	case LBS_CT_OR:	//或
	case LBS_CT_XOR:	//异或
	case LBS_CT_LMB:
	case LBS_CT_RMB:
	case LBS_CT_LOGIC_EQUAL:		//等于
	case LBS_CT_LOGIC_NOTEQUAL:	//不等于
	case LBS_CT_LOGIC_BE:		//大于等于
	case LBS_CT_LOGIC_SE:		//小于等于
	case LBS_CT_LOGIC_B:			//大于
	case LBS_CT_LOGIC_S:			//小于
	case LBS_CT_MOD:
	case LBS_CT_SUFFIX_START://[//下标
	case LBS_CT_SUFFIX_END:	//]
	case LBS_CT_AREA_CHILD:	//.	//子域
	case LBS_CT_AREA_ORIGIN:	//:://根域

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
				ShowDebugTrace("#错误：变量[%s]未定义!",(LPCSTR)m_pData);
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

	//添加一个引用记数，用于外部释放
	if(m_pReturnVariable != NULL)
		m_pReturnVariable->AddRef();
	return ((CLBS_Variable*)m_pReturnVariable);
}
