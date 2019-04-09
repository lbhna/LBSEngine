#pragma once
/********************************************************************
	created:	2012-6-6   14:40
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Base\LBS_ObjBase.h
	author:		LB
	
	purpose:	�������
*********************************************************************/

#include "GlobalDefine.h"

typedef enum LBS_EXPORT _LBS_OBJ_TYPE
{
	LOT_OBJ_BASE	=0,		//����
	LOT_OBJ_LIST,
	LOT_OBJ_HASH_LIST,

	LOT_CODE_BASE,
	LOT_SCRIPT_FILE,
	LOT_SCRIPT_DRIVER,
	LOT_SCRIPT_PARSER,

	LOT_CODE_ROOT,			//��
	LOT_CODE_VARIABLE,		//����
	LOT_CODE_FUNCTION,		//����
	LOT_CODE_BLOCK,			//�����
	LOT_CODE_RETURN_STATEMENT,//���ر��ʽ
	LOT_CODE_BREAK_STATEMENT,//����ѭ���ı��ʽ
	LOT_CODE_CONTINUE_STATEMENT,//����ѭ���ı��ʽ

	
	LOT_CODE_CALC_STATEMENT,//������ʽ
	LOT_CODE_CALC_FACTOR,//����
	LOT_CODE_IFELSE_STATEMENT,//if else ���
	LOT_CODE_WHILE_STATEMENT,//while ���
	LOT_CODE_FOR_STATEMENT,			//FORѭ�����
	LOT_CODE_SWITCH_STATEMENT,//switch���
	LOT_CODE_CASE_STATEMENT,//case ���

	LOT_CODE_CLASS,			//��
	LOT_CODE_STRUCT,		//�ṹ��
	LOT_CODE_FUNC_CALL,		//��������
	LOT_CODE_MACRO,			//�궨��

	LOT_FUNCTION_PARAMETER,
	LOT_SCRIPT_TIMER,		//�ű���ʱ��
	LOT_SCRIPT_UDP,			//UDP����ͨѶ�ӿ�
	LOT_SCRIPT_TCP,			//TCP����ͨѶ�ӿ�



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

