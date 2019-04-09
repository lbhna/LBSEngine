#pragma once
#include "lbs_objbase.h"

class LBS_EXPORT CLBS_ObjHashListBase :
	public CLBS_ObjBase
{
protected:
	typedef struct _LBS_OBJ_HASH_NODE
	{
		CLBS_ObjBase*		pObj;
		_LBS_OBJ_HASH_NODE* pNext;
	}LBS_OBJ_HASH_NODE,*PLBS_OBJ_HASH_NODE;
protected:
	PLBS_OBJ_HASH_NODE*		m_pHashArray;
	DWORD					m_dwHashArraySize;
	DWORD					m_dwCount;
public:
	CLBS_ObjHashListBase(DWORD IN dwHashArraySize=200);
	virtual ~CLBS_ObjHashListBase(void);
public:
	VOID					AddObj(CLBS_ObjBase* IN pObj);
	LONG					GetCount();
	CLBS_ObjBase*			GetFirstObj(LONG& IN OUT index,POSITION& IN OUT pos);
	CLBS_ObjBase*			GetNextObj(LONG& IN OUT index,POSITION& IN OUT pos);
	CLBS_ObjBase*			GetObj(LPCSTR IN pObjName);
	
	VOID					DelObj(CLBS_ObjBase* IN pObj);
	VOID					DelObj(LPCSTR IN pObjName);
	VOID					Clear();

};
