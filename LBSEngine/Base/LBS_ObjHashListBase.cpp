#include "StdAfx.h"
#include ".\lbs_objhashlistbase.h"


CLBS_ObjHashListBase::CLBS_ObjHashListBase(DWORD IN dwHashArraySize)
{
	m_Type				=LOT_OBJ_HASH_LIST;
	m_dwHashArraySize		=dwHashArraySize;
	if(m_dwHashArraySize < 10)
		m_dwHashArraySize =10;
	if(m_dwHashArraySize > 100000)
		m_dwHashArraySize =100000;
	m_pHashArray			=NULL;
	m_dwCount				=0;
}

CLBS_ObjHashListBase::~CLBS_ObjHashListBase(void)
{
	Clear();
	LBS_SafeDeleteArray(m_pHashArray);
}

VOID					CLBS_ObjHashListBase::AddObj(CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return;

	if(	m_pHashArray == NULL)
	{
		m_pHashArray			=new PLBS_OBJ_HASH_NODE[m_dwHashArraySize];
		for(DWORD i=0;i<m_dwHashArraySize;i++)
			m_pHashArray[i] =NULL;
	}

	DWORD	dwIndex =pObj->GetNameHashKey()%m_dwHashArraySize;

	PLBS_OBJ_HASH_NODE		pPrevNode=NULL;
	PLBS_OBJ_HASH_NODE		pNode	 =m_pHashArray[dwIndex];
	while(pNode != NULL)
	{
		pPrevNode	=pNode;
		pNode		=pNode->pNext;
	}

	PLBS_OBJ_HASH_NODE		pNewNode =(PLBS_OBJ_HASH_NODE)LB_ALLOC(sizeof(LBS_OBJ_HASH_NODE));
	pNewNode->pNext =NULL;
	pNewNode->pObj	=pObj;
	pObj->AddRef();
	if(pPrevNode == NULL)
		m_pHashArray[dwIndex]	=pNewNode;
	else
		pPrevNode->pNext		=pNewNode;
	m_dwCount++;
}
inline LONG				CLBS_ObjHashListBase::GetCount()
{
	return m_dwCount;
}
CLBS_ObjBase*			CLBS_ObjHashListBase::GetObj(LPCSTR IN pObjName)
{
	if(m_pHashArray == NULL)
		return NULL;

	DWORD index =CLBS_ObjBase::MakeStringHashKey(pObjName)%m_dwHashArraySize;
	PLBS_OBJ_HASH_NODE pNode =m_pHashArray[index];
	if(pNode == NULL)
		return NULL;
	while(pNode != NULL)
	{
		if(pNode->pObj!=NULL)
		{
			if(strcmp(pNode->pObj->GetName(),pObjName)==0)
				return pNode->pObj;
		}
		pNode =pNode->pNext;
	}
	return NULL;
}

VOID					CLBS_ObjHashListBase::DelObj(CLBS_ObjBase* IN pObj)
{
	if(pObj == NULL)
		return;
	DelObj(pObj->GetName());
}
VOID					CLBS_ObjHashListBase::DelObj(LPCSTR IN pObjName)
{
	if(m_pHashArray == NULL)
		return;
	DWORD				index	=CLBS_ObjBase::MakeStringHashKey(pObjName)%m_dwHashArraySize;
	PLBS_OBJ_HASH_NODE	pNode	=m_pHashArray[index];
	if(pNode == NULL)
		return;

	CLBS_ObjBase*		pObj	=NULL;
	PLBS_OBJ_HASH_NODE	pParent =pNode;
	while(pNode != NULL)
	{
		if(pNode->pObj!=NULL)
		{
			if(strcmp(pNode->pObj->GetName(),pObjName)==0)
			{
				pObj =pNode->pObj;
				if(pParent == pNode)
					m_pHashArray[index] =NULL;
				else
					pParent->pNext =pNode->pNext;
				break;
			}
		}
		pParent		=pNode;
		pNode		=pNode->pNext;
	}

	if(pObj!=NULL)
		pObj->Release();
	m_dwCount --;
}
VOID					CLBS_ObjHashListBase::Clear()
{
	if(m_pHashArray != NULL)
	{
		PLBS_OBJ_HASH_NODE		pNode,pTemp;
		for(DWORD i=0;i<m_dwHashArraySize;i++)
		{
			if(m_pHashArray[i]== NULL)
				continue;
			pNode=m_pHashArray[i];
			while(pNode!=NULL)
			{
				pTemp=pNode;
				pNode=pNode->pNext;
				LBS_SafeRelease(pTemp->pObj);
                LB_FREE((LPVoid&)pTemp);
			}
			m_pHashArray[i] =NULL;
		}
	}

	m_dwCount	=0;
}
CLBS_ObjBase*			CLBS_ObjHashListBase::GetFirstObj(LONG& IN OUT index,POSITION& IN OUT pos)
{
	if(m_pHashArray == NULL)
		return NULL;
	index	=0;
	pos		=NULL;
	PLBS_OBJ_HASH_NODE	pNode =NULL;
	for(index=0;index<(LONG)m_dwHashArraySize;index++)
	{
		pNode	=m_pHashArray[index];
		if(pNode == NULL)
			continue;
		pos		=(POSITION)pNode->pNext;
		return	pNode->pObj;

	}
	return NULL;
}
CLBS_ObjBase*			CLBS_ObjHashListBase::GetNextObj(LONG& IN OUT index,POSITION& IN OUT pos)
{
	if(m_pHashArray == NULL)
		return NULL;
	if(index <0)
		return NULL;

	PLBS_OBJ_HASH_NODE	pNode =NULL;
	if(pos != NULL)
	{
		pNode	=(PLBS_OBJ_HASH_NODE)pos;
		pos		=(POSITION)pNode->pNext;
		return pNode->pObj;
	}
	index ++;
	for(;index<(LONG)m_dwHashArraySize;index++)
	{
		pNode	=m_pHashArray[index];
		if(pNode == NULL)
			continue;
		pos		=(POSITION)pNode->pNext;
		return	pNode->pObj;

	}
	return NULL;
}

