#include "StdAfx.h"
#include ".\LBS_objbase.h"

LONG			CLBS_ObjBase::m_lTotalShareObjectCount	=0;
CLBS_ObjBase::CLBS_ObjBase(void)
{
	m_lRefCount			=1;
	m_Type				=LOT_OBJ_BASE;
	m_pName				=(LPSTR)LB_ALLOC(32);
	m_pName[0]='n';
	m_pName[1]='u';
	m_pName[2]='l';
	m_pName[3]='l';
	m_pName[4]=0;

#ifdef _DEBUG
	ObjCounterIncrease();
#endif	
}
CLBS_ObjBase::CLBS_ObjBase(LPCSTR IN pName)
{
	m_lRefCount			=1;
	m_Type				=LOT_OBJ_BASE;
	m_pName				=NULL;
	SetName(pName);

#ifdef _DEBUG
	ObjCounterIncrease();
#endif		
}

CLBS_ObjBase::~CLBS_ObjBase(void)
{
#ifdef _DEBUG
	ObjCounterDecrease();
#endif	
    LB_FREE((LPVoid&)m_pName);
}
#ifdef _DEBUG
VOID				CLBS_ObjBase::ObjCounterIncrease()
{
	CLBS_ObjBase::m_lTotalShareObjectCount++;

	CHAR buf[256];
	if(CLBS_ObjBase::m_lTotalShareObjectCount<10)
	{
		sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
		OutputDebugString(buf);
	}
	else if(CLBS_ObjBase::m_lTotalShareObjectCount<100)
	{
		if(CLBS_ObjBase::m_lTotalShareObjectCount%10==0)
		{	
			sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
			OutputDebugString(buf);
		}
	}
	else
	{
		if(CLBS_ObjBase::m_lTotalShareObjectCount%100==0)
		{
			sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
			OutputDebugString(buf);
		}
	}
}
VOID				CLBS_ObjBase::ObjCounterDecrease()
{
	CLBS_ObjBase::m_lTotalShareObjectCount--;

	CHAR buf[256];
	if(CLBS_ObjBase::m_lTotalShareObjectCount<10)
	{
		sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
		OutputDebugString(buf);
	}
	else if(CLBS_ObjBase::m_lTotalShareObjectCount<100)
	{
		if(CLBS_ObjBase::m_lTotalShareObjectCount%10==0)
		{	
			sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
			OutputDebugString(buf);
		}
	}
	else
	{
		if(CLBS_ObjBase::m_lTotalShareObjectCount%100==0)
		{
			sprintf(buf,"#LBS_Object(%d,%s) Count: %d \r\n",m_Type,m_pName,CLBS_ObjBase::m_lTotalShareObjectCount);
			OutputDebugString(buf);
		}
	}
}
#endif


VOID			CLBS_ObjBase::SetName(LPCSTR IN pName)
{
	SetName(pName,(LONG)strlen(pName));
}
VOID			CLBS_ObjBase::SetName(LPCSTR IN pBuf,LONG IN lSize)
{
    LB_FREE((LPVoid&)m_pName);
	m_pName		=(LPSTR)LB_ALLOC(lSize+1);
	strncpy(m_pName,pBuf,lSize);
	m_pName[lSize]=0;
}
inline LONG				CLBS_ObjBase::AddRef()
{
	return (++m_lRefCount);
}
inline VOID				CLBS_ObjBase::Release()
{
	--m_lRefCount;

	if(m_lRefCount ==0 )
	{
		delete this;
	}
//#ifdef _DEBUG
//	else
//	{
//		if( m_lRefCount < 0 )
//			_asm int 3;
//	}
//#endif	
}
inline LONG				CLBS_ObjBase::GetRefCount()
{
	return m_lRefCount;
}

inline void* CLBS_ObjBase::operator new(size_t sz, const char* file, int line)
{
	return LB_ALLOC((INT)sz);
}
inline void* CLBS_ObjBase::operator new(size_t sz, const char* file, int line, const char* func)
{
	return LB_ALLOC((INT)sz);
}
inline void* CLBS_ObjBase::operator new(size_t sz)
{
	return LB_ALLOC((INT)sz);
}
inline void* CLBS_ObjBase::operator new(size_t sz, void* ptr)
{
	(void) sz;
	return ptr;
}
inline void* CLBS_ObjBase::operator new[] ( size_t sz, const char* file, int line)
{
	return LB_ALLOC((INT)sz);
}
inline void* CLBS_ObjBase::operator new[] ( size_t sz, const char* file, int line, const char* func )
{
	return LB_ALLOC((INT)sz);
}
inline void* CLBS_ObjBase::operator new[] ( size_t sz )
{
	return LB_ALLOC((INT)sz);
}
inline void CLBS_ObjBase::operator delete( void* ptr )
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete( void* ptr, void* )
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete( void* ptr, const char* , int )
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete( void* ptr, const char* , int , const char*  )
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete[] ( void* ptr )
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete[] ( void* ptr, const char* , int)
{
	LB_FREE(ptr);
}
inline void CLBS_ObjBase::operator delete[] ( void* ptr, const char* , int , const char*  )
{
	LB_FREE(ptr);
}
DWORD		CLBS_ObjBase::MakeStringHashKey(LPCSTR IN pStr)
{
	if (pStr == NULL)
		return 0;
	DWORD nHash = 0;
	while (*pStr)
		nHash = (nHash<<5) + nHash + *pStr++;
	return nHash;
}	
inline DWORD		CLBS_ObjBase::GetNameHashKey()
{
	return CLBS_ObjBase::MakeStringHashKey(GetName());
}
VOID		CLBS_ObjBase::ShowDebugTrace(LPCSTR IN pszFormat,...)
{
	CString		info;
	va_list argList;
	va_start( argList, pszFormat );
	info.FormatV( pszFormat, argList );
	va_end( argList );

	/////////////////////
	//TRACE("%s\r\n",info);
	/////////////////////
}

