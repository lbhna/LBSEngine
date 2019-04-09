#pragma once
/********************************************************************
	created:	2012-6-6   14:40
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Base\LBS_ObjBase.h
	author:		LB
	
	purpose:	对象基类
*********************************************************************/

#include "GlobalDefine.h"

typedef enum LBS_EXPORT _LBS_OBJ_TYPE
{
	LOT_OBJ_BASE	=0,		//基类
	LOT_OBJ_LIST,
	LOT_OBJ_HASH_LIST,

	LOT_CODE_BASE,
	LOT_SCRIPT_FILE,
	LOT_SCRIPT_DRIVER,
	LOT_SCRIPT_PARSER,

	LOT_CODE_ROOT,			//根
	LOT_CODE_VARIABLE,		//变量
	LOT_CODE_FUNCTION,		//函数
	LOT_CODE_BLOCK,			//代码块
	LOT_CODE_RETURN_STATEMENT,//返回表达式
	LOT_CODE_BREAK_STATEMENT,//跳出循环的表达式
	LOT_CODE_CONTINUE_STATEMENT,//继续循环的表达式

	
	LOT_CODE_CALC_STATEMENT,//运算表达式
	LOT_CODE_CALC_FACTOR,//算子
	LOT_CODE_IFELSE_STATEMENT,//if else 语句
	LOT_CODE_WHILE_STATEMENT,//while 语句
	LOT_CODE_FOR_STATEMENT,			//FOR循环语句
	LOT_CODE_SWITCH_STATEMENT,//switch语句
	LOT_CODE_CASE_STATEMENT,//case 语句

	LOT_CODE_CLASS,			//类
	LOT_CODE_STRUCT,		//结构体
	LOT_CODE_FUNC_CALL,		//函数调用
	LOT_CODE_MACRO,			//宏定义

	LOT_FUNCTION_PARAMETER,
	LOT_SCRIPT_TIMER,		//脚本定时器
	LOT_SCRIPT_UDP,			//UDP网络通讯接口
	LOT_SCRIPT_TCP,			//TCP网络通讯接口



	LCT_FORCE_DWORD =0x7fffffff,
}LBS_OBJ_TYPE;
//----------------------------------------------------

class LBS_EXPORT CLBS_ObjBase
{
public:
	static LONG			m_lTotalShareObjectCount;
protected:
	LBS_OBJ_TYPE		m_Type;
	LPSTR				m_pName;
private:
	LONG				m_lRefCount;
public:
	CLBS_ObjBase(void);
	CLBS_ObjBase(LPCSTR IN pName);
	virtual ~CLBS_ObjBase(void);
public:
	LBS_OBJ_TYPE		GetType(){return m_Type;}
	LONG				AddRef();
	VOID				Release();
	LONG				GetRefCount();
	LPCSTR				GetName(){return m_pName;}
	VOID				SetName(LPCSTR IN pName);
	VOID				SetName(LPCSTR IN pBuf,LONG IN lSize);
public:
	void*				operator new(size_t sz, const char* file, int line);
	void*				operator new(size_t sz, const char* file, int line, const char* func);
	void*				operator new(size_t sz);
	void*				operator new(size_t sz, void* ptr);
	void*				operator new[] ( size_t sz, const char* file, int line);
	void*				operator new[] ( size_t sz, const char* file, int line, const char* func );
	void*				operator new[] ( size_t sz );
	void				operator delete( void* ptr );
	void				operator delete( void* ptr, void* );
	void				operator delete( void* ptr, const char* , int);
	void				operator delete( void* ptr, const char* , int , const char*  );
	void				operator delete[] ( void* ptr );
	void				operator delete[] ( void* ptr, const char* , int);
	void				operator delete[] ( void* ptr, const char* , int , const char*  );


public:
	static	DWORD		MakeStringHashKey(LPCSTR IN pStr);
public:
	virtual DWORD		GetNameHashKey();
	virtual VOID		ShowDebugTrace(LPCSTR IN pszFormat,...);

protected:
#ifdef _DEBUG
	VOID				ObjCounterIncrease();
	VOID				ObjCounterDecrease();
#endif
};

