#include "stdafx.h"
#include "./MemoryPool.h"



#ifdef MEM_POOL_ENABLED

CMemoryPoolPage::CMemoryPoolPage()
{
	m_iMemPoolSize			=MEM_POOL_BLOCK_COUNT*MEM_POOL_BLOCK_SIZE;				
	m_pMemPool				=::new unsigned char[m_iMemPoolSize];		
	m_pBlockArray			=::new unsigned char[MEM_POOL_BLOCK_COUNT];
	for(int	i=0;i<MEM_POOL_BLOCK_COUNT;i++)
		m_pBlockArray[i]	=0;
	m_iLastFindBlockIndex	=0;
	m_pLastBlockAddr		=m_pMemPool+m_iMemPoolSize;
	m_iLeftBlockCount		=MEM_POOL_BLOCK_COUNT;		
}

CMemoryPoolPage::~CMemoryPoolPage()
{
	if(m_pMemPool!=NULL)
	{
		::delete []m_pMemPool;
		m_pMemPool=NULL;
	}

	if(m_pBlockArray!=NULL)
	{
		::delete []m_pBlockArray;
		m_pBlockArray=NULL;
	}
	m_iLeftBlockCount=0;



}
LPVoid				CMemoryPoolPage::Alloc(int	iSize)
{
	int		alloc_block_count= (iSize/MEM_POOL_BLOCK_SIZE) + ( ( iSize % MEM_POOL_BLOCK_SIZE)?1:0 );
	if(alloc_block_count > 0x7f)
		return NULL;

	LPVoid		pAddr=NULL;
	int			index=-1;
	if(m_iLeftBlockCount >= alloc_block_count)
		index=FindSequenceBlock(alloc_block_count);
	if(index < 0 )
		return NULL;

	unsigned char* pStart	=m_pBlockArray	+index;
	unsigned char* pEnd		=pStart			+alloc_block_count;
	*pStart++ = 1|(alloc_block_count<<1);	
	while(pStart < pEnd)
		*pStart++=1;
	pAddr=BlockIndexToAddr(index);
	m_iLeftBlockCount -=alloc_block_count;


	return pAddr;
}

bool				CMemoryPoolPage::Free(LPVoid pAddr)
{
	if(!IsValidAddr(pAddr))
		return false;



	int				index	=AddrToBlockIndex(pAddr);
	unsigned char*	pStart	=m_pBlockArray +index ;
	int				count	=(*pStart>>1);
	unsigned char*	pEnd	=pStart+count;
	while(pStart < pEnd)*pStart++ =0;
	m_iLeftBlockCount += count;


	if( (index+count) == m_iLastFindBlockIndex)
	{
		for(--index; index>=0; --index)
		{
			if( (m_pBlockArray[index]&1) !=0)
			{
				++index;
				break;
			}
		}
		m_iLastFindBlockIndex =index;
	}
    
	return true;
}
bool				CMemoryPoolPage::IsEmpty()
{
	return (m_iLeftBlockCount == MEM_POOL_BLOCK_COUNT);
}
inline int		CMemoryPoolPage::FindSequenceBlock(int iCount)
{
	int		iStartIndex	=-1;
	int		count		=0;
	int		i			=0;
	for(i=m_iLastFindBlockIndex;i<MEM_POOL_BLOCK_COUNT;i++)
	{
		if((m_pBlockArray[i]&1) !=0)
		{
			iStartIndex=-1;
		}
		else
		{
			if(iStartIndex==-1)
			{
				iStartIndex=i;
				count=1;
			}
			else
			{
				count++;
			}
			if(count>=iCount)
			{
				m_iLastFindBlockIndex =iStartIndex+iCount;
				return iStartIndex;
			}
		}
	}

	iStartIndex	=-1;
	count		=0;
	int endCount =m_iLastFindBlockIndex + iCount;

	for(i=0;i<endCount;i++)
	{
		if( (m_pBlockArray[i]&1) != 0)
		{
			iStartIndex=-1;
		}
		else
		{
			if(iStartIndex==-1)
			{
				iStartIndex=i;
				count=1;
			}
			else
			{
				count++;
			}
			if(count>=iCount)
			{
				m_iLastFindBlockIndex =iStartIndex+iCount;
				return iStartIndex;
			}
		}
	}
	return -1;
}


inline bool			CMemoryPoolPage::IsValidAddr(LPVoid	pAddr)
{
	if(pAddr < m_pMemPool || pAddr >= m_pLastBlockAddr)
		return false;
	return true;
}

inline LPVoid		CMemoryPoolPage::BlockIndexToAddr(int	iIndex)
{
	return (m_pMemPool+(MEM_POOL_BLOCK_SIZE*iIndex));
}
inline int		CMemoryPoolPage::AddrToBlockIndex(LPVoid	pAddr)
{
	return (int)((long)((unsigned char*)pAddr-(unsigned char*)m_pMemPool)/(long)MEM_POOL_BLOCK_SIZE);
}
////////////////////////////////////////////////////////////////////////////////////


//请求分配iSize大小的内存，返回地址
LPVoid	CMemoryPoolManager::Alloc(int	iSize)
{
    LPVoid			pMem	=NULL;
	int blockCount =(iSize/MEM_POOL_BLOCK_SIZE)+ ( (iSize%MEM_POOL_BLOCK_SIZE)?1:0 );
	if(  blockCount > 0x7f || blockCount >= MEM_POOL_BLOCK_COUNT/100 )
    {
        pMem	=::new unsigned char[iSize];
        return pMem;
    }

#ifdef MEM_POOL_MULTITHREAD
	Lock();
#endif
    {
    	for(int i=0;i<MEM_POOL_PAGE_COUNT;i++)
	    {
		    if(m_ppPageArray[i])
		    {
			    pMem = m_ppPageArray[i]->Alloc(iSize);
			    if(pMem)
			    {
                #ifdef MEM_POOL_MULTITHREAD
				    Unlock();
                #endif
				    return pMem;
			    }
		    }		
	    }
	    if(m_iPageCount < MEM_POOL_PAGE_COUNT)
	    {
		    LPMemoryPoolPage pNewPage =AddNewPage();
		    if(pNewPage != NULL)
			    pMem =pNewPage->Alloc(iSize);
	    }

    }
#ifdef MEM_POOL_MULTITHREAD
	Unlock();
#endif

	if(pMem == NULL)
    {
        pMem = ::new unsigned char[iSize];
    }

	return pMem;
}

void	CMemoryPoolManager::Free(LPVoid pAddr)
{
	if(pAddr == NULL)
		return;
#ifdef MEM_POOL_MULTITHREAD
	Lock();
#endif
	for(int i=0;i<MEM_POOL_PAGE_COUNT;i++)
	{
		if(m_ppPageArray[i] == NULL)
			continue;
		if(m_ppPageArray[i]->Free(pAddr))
		{
			pAddr =	NULL;
			//if(m_iPageCount > 1)
			//{
			//	if(m_ppPageArray[i]->IsEmpty())					
			//		RemovePage(i);
			//}
#ifdef MEM_POOL_MULTITHREAD
			Unlock();
#endif
			return;
		}		
	}
#ifdef MEM_POOL_MULTITHREAD
	Unlock();
#endif
	if(pAddr != NULL)
        ::delete []((unsigned char*)pAddr);
}
void	CMemoryPoolManager::Clear()
{
#ifdef MEM_POOL_MULTITHREAD
	Lock();
#endif
	for(int i=0;i<MEM_POOL_PAGE_COUNT;i++)
	{
		if(m_ppPageArray[i] != NULL)
		{
			::delete m_ppPageArray[i];
			m_ppPageArray[i]	=NULL;
		}	
	}
	m_iPageCount =0;

#ifdef MEM_POOL_MULTITHREAD
	Unlock();
#endif
}
void					CMemoryPoolManager::RemovePage(int index)
{
	::delete m_ppPageArray[index];
	m_ppPageArray[index] =NULL;
	m_iPageCount--;
}

LPMemoryPoolPage		CMemoryPoolManager::AddNewPage()
{
	for(int i=0;i<MEM_POOL_PAGE_COUNT;i++)
	{
		if(m_ppPageArray[i] == NULL)
		{
			m_ppPageArray[i]	=::new CMemoryPoolPage();
			m_iPageCount		++;
			return m_ppPageArray[i];	
		}	
	}
	return NULL;
}

CMemoryPoolManager		gm_theMemPool;
#endif
//---------------------------------------------------------------------------

LPVoid		LB_ALLOC(int iSize)
{
#ifdef MEM_POOL_ENABLED
	return (LPVoid)gm_theMemPool.Alloc(iSize);
#else
	return (LPVoid)::new unsigned char[iSize];
#endif
}
void		LB_FREE(LPVoid& pAddr)
{
#ifdef MEM_POOL_ENABLED
	{gm_theMemPool.Free(pAddr);pAddr=NULL;}
#else
	{if(pAddr!=NULL){::delete pAddr;pAddr=NULL;}}
#endif
}

