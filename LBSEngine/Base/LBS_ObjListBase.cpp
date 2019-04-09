#include "StdAfx.h"
#include ".\LBS_objlistbase.h"



LONG	CLBS_ObjListBase::m_lOverflowIncreaseStep		=16;
CLBS_ObjListBase::CLBS_ObjListBase(void)
{
	m_lMaxSize			=0;
	m_ppObj				=NULL;
	m_lCount			=0;
	m_Type				=LOT_OBJ_LIST;
}

CLBS_ObjListBase::~CLBS_ObjListBase(void)
{
	Clear();
}
inline LONG			CLBS_ObjListBase::GetCount()
{
	return m_lCount;
}
VOID			CLBS_ObjListBase::Clear()
{
	for(LONG i=0;i<m_lCount;i++)
	{
		m_ppObj[i]->Release();
	}
    LB_FREE((LPVoid&)m_ppObj);
	m_lCount			=0;
	m_lMaxSize			=0;
}
inline CLBS_ObjBase*	CLBS_ObjListBase::GetObj(LONG IN lIndex)
{
	if(lIndex < 0 || lIndex >= m_lCount || m_ppObj == NULL)
		return NULL;

	return m_ppObj[lIndex];
}
inline LONG			CLBS_ObjListBase::FindObj(CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return -1;
	LONG lIndex =0;
	for(lIndex =0 ;lIndex<m_lCount;lIndex++)
	{
		if(m_ppObj[lIndex]==pObj)
			return lIndex;
	}
	return -1;
}
inline CLBS_ObjBase*CLBS_ObjListBase::FindObj(LPCSTR IN pName)
{
	if(pName == NULL)
		return NULL;
	LONG			lIndex		=0;
	for(lIndex =0 ;lIndex<m_lCount;lIndex++)
	{
		if(strcmp(m_ppObj[lIndex]->GetName(),pName)==0)
			return m_ppObj[lIndex];
	}
	return NULL;
}

inline BOOL			CLBS_ObjListBase::DelObj(LONG IN lIndex)
{
	if(lIndex < 0 || lIndex >= m_lCount || m_ppObj == NULL)
		return FALSE;

	m_ppObj[lIndex]->Release();
	--m_lCount;

	LONG i=0;
	for(i=lIndex;i<m_lCount;i++)
	{
		m_ppObj[i]=m_ppObj[i+1];
	}
	m_ppObj[i]=0;

	return TRUE;
}

inline BOOL			CLBS_ObjListBase::DelObj(CLBS_ObjBase* IN pObj)
{
	return DelObj(FindObj(pObj));
}

inline VOID			CLBS_ObjListBase::AddObj(CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return;
	OverflowCheck();
	m_ppObj[m_lCount]=pObj;
	pObj->AddRef();
	m_lCount++;
}
inline BOOL			CLBS_ObjListBase::InsertObj(LONG IN lIndex,CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return FALSE;
	if(lIndex > m_lCount || lIndex < 0)
		return FALSE;
	OverflowCheck();

	for(LONG i=m_lCount;i>lIndex;i--)
	{
		m_ppObj[i]	=m_ppObj[i-1];
	}
	m_ppObj[lIndex]=pObj;
	pObj->AddRef();
	m_lCount++;
	return TRUE;
}
inline BOOL			CLBS_ObjListBase::SetAt(LONG IN lIndex,CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return FALSE;
	if(lIndex >= m_lCount || lIndex < 0)
		return FALSE;

	m_ppObj[lIndex]->Release();
	m_ppObj[lIndex]=pObj;
	pObj->AddRef();
	return TRUE;
}


BOOL			CLBS_ObjListBase::Swap(LONG IN lIndex1,LONG IN lIndex2)
{
	if(lIndex1 <0 || lIndex1 >= m_lCount||
		lIndex2 <0 || lIndex2 >= m_lCount)
		return FALSE;

	CLBS_ObjBase*	pTmp	=m_ppObj[lIndex1];

	m_ppObj[lIndex1]		=m_ppObj[lIndex2];

	m_ppObj[lIndex2]		=pTmp;

	return TRUE;
}

BOOL			CLBS_ObjListBase::Swap(CLBS_ObjBase* IN pObj1,CLBS_ObjBase* IN pObj2)
{
	return Swap(FindObj(pObj1),FindObj(pObj2));
}

VOID			CLBS_ObjListBase::OverflowCheck(LONG IN lStep)
{
	if( (m_lCount+lStep) >= m_lMaxSize || m_ppObj == NULL)
	{
		m_lMaxSize += max(lStep,CLBS_ObjListBase::m_lOverflowIncreaseStep);

		CLBS_ObjBase**	ppObj	= (CLBS_ObjBase**)LB_ALLOC( sizeof(CLBS_ObjBase*) * m_lMaxSize );

		LONG i=0;
		for( i=0;i<m_lCount;i++)
			ppObj[i] = m_ppObj[i];
		for(i=m_lCount;i<m_lMaxSize;i++)
			ppObj[i] = 0;

        LB_FREE((LPVoid&)m_ppObj);
		m_ppObj		= ppObj;
	}

}

CLBS_ObjListBase&CLBS_ObjListBase::operator=(CLBS_ObjListBase& v)
{
	Clear();
	for(LONG i=0;i<v.m_lCount;i++)
		AddObj(v.GetObj(i));
	return *this;
}

