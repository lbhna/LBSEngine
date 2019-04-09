#ifndef _MEMORY_POOL_HEADER_FILE
#define _MEMORY_POOL_HEADER_FILE
/*====================================================================\
		#��������:	2016/04/11
		#�ļ���  :	MemoryPool
		#����    :	LBHNA   
		#˵��    :	�ڴ�ع�����
\====================================================================*/



#define		MEM_POOL_ENABLED							//�������������ʹ���ڴ��
#define		MEM_POOL_PAGE_COUNT				4			    //�ڴ������ҳ����
#define		MEM_POOL_BLOCK_COUNT			4096000		    //�ڴ�ؿ��� ÿ���ڴ��ҳ(409600 * 128 = 50MB)
#define		MEM_POOL_BLOCK_SIZE				128			    //�ڴ���С
//#define		MEM_POOL_MULTITHREAD						//�Ƿ�������߳�



typedef		void*  LPVoid;
	
//--------------------------------------------------------------------
#ifdef		MEM_POOL_ENABLED

#include	<new>

#pragma		pack(push)
#pragma		pack(1)

class CMemoryPoolPage
{
protected:
	unsigned char*			m_pMemPool;				
	unsigned char*			m_pLastBlockAddr;		
	int						m_iMemPoolSize;			
	unsigned char*			m_pBlockArray;			
	int						m_iLastFindBlockIndex;	
	int						m_iLeftBlockCount;			
public:
	CMemoryPoolPage();	
	~CMemoryPoolPage();
public:
	LPVoid				Alloc(int	iSize);	
	bool				Free(LPVoid pAddr);	
	bool				IsEmpty();
private:
	bool				IsValidAddr(LPVoid pAddr);
	int					FindSequenceBlock(int iCount);
	LPVoid				BlockIndexToAddr(int	iIndex);
	int					AddrToBlockIndex(LPVoid	pAddr);				
};
typedef CMemoryPoolPage* LPMemoryPoolPage;
class  CMemoryPoolManager
{
protected:
#ifdef MEM_POOL_MULTITHREAD
	CriticalSection			m_Section;
#endif
	LPMemoryPoolPage		m_ppPageArray[MEM_POOL_PAGE_COUNT];
	int						m_iPageCount;
public:
	CMemoryPoolManager()
	{
		for(int i=0;i<MEM_POOL_PAGE_COUNT;i++)
			m_ppPageArray[i]=NULL;
		m_iPageCount=0;
#ifdef MEM_POOL_MULTITHREAD
		AGO_InitializeCriticalSection(m_Section);
#endif
	}
	~CMemoryPoolManager()
	{
		Clear();
#ifdef MEM_POOL_MULTITHREAD
		AGO_DeleteCriticalSection(m_Section);
#endif
	}
public:
	LPVoid					Alloc(int	iSize);
	void					Free(LPVoid pAddr);
	void					Clear();
protected:
#ifdef MEM_POOL_MULTITHREAD
	void					Lock()		{AGO_EnterCriticalSection(m_Section);}
	void					Unlock()	{AGO_LeaveCriticalSection(m_Section);}
#endif
	void					RemovePage(int index);
	LPMemoryPoolPage		AddNewPage();

};
#pragma  pack(pop)
#endif// MEM_POOL_ENABLED
//--------------------------------------------------------------------

LPVoid			LB_ALLOC(int iSize);
void			LB_FREE(LPVoid& pAddr);

template<class _Type,class ... ArgTypes>
_Type*			LB_NEW(ArgTypes&& ... args)
{
	_Type*	ptr =(_Type*)LB_ALLOC(sizeof(_Type));
	return (_Type*)::new(ptr)_Type(args...);
}

template<class _Type>
void			LB_DELETE(_Type* pObj)
{
	if(pObj != NULL)
	{
		pObj->~_Type();
		LB_FREE((LPVoid&)pObj);
	}
}
#endif