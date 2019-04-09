#pragma once
#include "../Base/LBS_CodeBase.h"
#include "./LBS_Variable.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_CalculateFactor.h
	created:	2012/05/11
	author:		LB
	purpose:	运算行算子类
\*********************************************************************/

typedef enum LBS_EXPORT _LBS_CALCULATE_FACTOR_TYPE
{
	LBS_CT_UNKNOWN	=0,//未知

	LBS_CT_VAR_NAME,	//变量名
	LBS_CT_INTEGER,		//整数数据
	LBS_CT_FLOAT,		//浮点数据
	LBS_CT_STRING,		//字串数据
	LBS_CT_FUNC_CALL,	//函数调用
	LBS_CT_CHILD_START,	//(//子式
	LBS_CT_CHILD_END,	//)
	LBS_CT_SEPERATOR,	//,
	LBS_CT_SUFFIX_START,//[//下标
	LBS_CT_SUFFIX_END,	//]

	LBS_CT_ADD,			//加法+
	LBS_CT_SUB,			//减法-
	
	LBS_CT_MUL,			//乘法*
	LBS_CT_DIV,			//除法/
	LBS_CT_MOD,			//模除
	
	LBS_CT_EQUAL,		//赋值=
	
	LBS_CT_LOGIC_AND,	//逻辑与
	LBS_CT_LOGIC_OR,	//逻辑或
	LBS_CT_LOGIC_NOT,	//逻辑非
	
	LBS_CT_LOGIC_EQUAL,		//等于
	LBS_CT_LOGIC_NOTEQUAL,	//不等于
	LBS_CT_LOGIC_BE,		//大于等于
	LBS_CT_LOGIC_SE,		//小于等于
	LBS_CT_LOGIC_B,			//大于
	LBS_CT_LOGIC_S,			//小于

	
	LBS_CT_AND,	//与
	LBS_CT_OR,	//或
	LBS_CT_NOT,	//非
	LBS_CT_XOR,	//异或
	LBS_CT_LMB,	//左移
	LBS_CT_RMB,	//右移

	LBS_CT_AREA_CHILD,	//.	//子域
	LBS_CT_AREA_ORIGIN,	//:://根域

	LBS_CT_OPER_NEW,		//new
	LBS_CT_OPER_DELETE,		//delete
	LBS_CT_CLASS_NAME,		//类名
	////////////////////////////////
	LBS_CT_VAR_OBJ,			//变量对象地址
	LBS_CT_CALC_STATEMENT,//表达式
	LBS_CT_FORCEDWORD =0x7fffffff,

}LBS_CT_TYPE;
typedef enum LBS_EXPORT _FACTOR_CLASS
{
	LBS_FC_INVALID =0,
	LBS_FC_DOUBLE_OPER,		//双目操作符，必需有源操作数
	LBS_FC_SINGLE_OPER,		//单目操作符，无需源操作数
	LBS_FC_OPERFACTOR,		//操作数
	LBS_FC_SEPERATOR,		//分割符
	LBS_FC_FORCEDWORD	=0x7fffffff,
}FACTOR_CLASS;
class CLBS_Statement_CalculateFactor :
	public CLBS_CodeBase
{
protected:
	VOID*			m_pData;
	DWORD			m_dwDataLen;
	LBS_CT_TYPE		m_FactorType;		//类型

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
