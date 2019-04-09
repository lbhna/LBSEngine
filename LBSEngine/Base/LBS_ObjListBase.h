#pragma once
#include "./LBS_ObjBase.h"


class LBS_EXPORT CLBS_ObjListBase :
	public CLBS_ObjBase
{
public:
	static LONG		m_lOverflowIncreaseStep;
protected:
	LONG			m_lMaxSize;
	LONG			m_lCount;
	CLBS_ObjBase**	m_ppObj;

public:
	CLBS_ObjListBase(void);
	virtual ~CLBS_ObjListBase(void);

public:
	LONG			GetCount();
	VOID			Clear();

	CLBS_ObjBase*	GetObj(LONG IN lIndex);
	CLBS_ObjBase*	operator[](LONG IN lIndex){return GetObj(lIndex);}

	LONG			FindObj(CLBS_ObjBase* IN pObj);
	CLBS_ObjBase*	FindObj(LPCSTR IN pName);

	BOOL			DelObj(LONG IN lIndex);
	BOOL			DelObj(CLBS_ObjBase* IN pObj);
	VOID			AddObj(CLBS_ObjBase* IN pObj);
	BOOL			InsertObj(LONG IN lIndex,CLBS_ObjBase* IN pObj);
	BOOL			SetAt(LONG IN lIndex,CLBS_ObjBase* IN pObj);

	BOOL			Swap(LONG IN lIndex1,LONG IN lIndex2);
	BOOL			Swap(CLBS_ObjBase* IN pObj1,CLBS_ObjBase* IN pObj2);

	CLBS_ObjListBase&operator=(CLBS_ObjListBase& v);
protected:
	VOID			OverflowCheck(LONG IN lStep=1);


};

