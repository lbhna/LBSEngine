#include "StdAfx.h"
#include ".\lbs_variable.h"
#include "LBS_Function.h"
#include "LBS_Script_FunctionMap.h"
#include "LBS_Script_ClassBase.h"
#include "LBS_ScriptMacroDefine.h"
#include <math.h>

CLBS_Variable::CLBS_Variable(CLBS_CodeBase* IN pParent):
CLBS_CodeBase(pParent)
{
	m_Type				=LOT_CODE_VARIABLE;
	m_VarType			=LVT_UNKNOWN;
	m_pVarBuffer		=NULL;
	m_dwVarBufferSize	=0;
	m_dwVarSize			=0;

	m_bFullRef			=TRUE;
	m_dwSuffix			=0;		//下标值
	m_pRefVariable		=NULL;	//属于哪个变量

}

CLBS_Variable::~CLBS_Variable(void)
{
	Reset();
	LB_FREE(m_pVarBuffer);
}
inline VOID			CLBS_Variable::AdjustVarBuffer(DWORD IN dwNewSize)
{
	if(m_pVarBuffer == NULL)
	{
		m_dwVarBufferSize	=dwNewSize;
		m_pVarBuffer		=LB_ALLOC(m_dwVarBufferSize);
		return;
	}

	if(m_dwVarBufferSize < dwNewSize)
	{
		m_dwVarBufferSize	=dwNewSize;
		LB_FREE(m_pVarBuffer);
		m_pVarBuffer		=LB_ALLOC(m_dwVarBufferSize);
	}
}
BOOL			CLBS_Variable::ReSize(DWORD IN dwSize)
{
	if(dwSize > 0x40000000)
	{
		ShowDebugTrace("变量[ %s ] ReSize 失败。请求的内存太大！",GetName());
		return FALSE;
	}
	switch(m_VarType)
	{
	case LVT_STRING:
	case LVT_BINARY_DATA:
	case LVT_UNKNOWN:
		{
			if(m_dwVarBufferSize <= dwSize)
			{
				BYTE*	pNewBuf		=(BYTE*)LB_ALLOC(dwSize+1);

				if(m_pVarBuffer != NULL)
				{
					memcpy(pNewBuf,m_pVarBuffer,m_dwVarBufferSize);
					LB_FREE(m_pVarBuffer);
				}
				m_pVarBuffer		=pNewBuf;
				m_dwVarBufferSize	=dwSize+1;
			}
			
			if(dwSize > m_dwVarSize)
				ZeroMemory((BYTE*)m_pVarBuffer+m_dwVarSize,dwSize-m_dwVarSize);
			else
				ZeroMemory((BYTE*)m_pVarBuffer+dwSize,m_dwVarSize-dwSize);
			m_dwVarSize =dwSize;

			return TRUE;			
		}
		break;
	default:
		{
			ShowDebugTrace("变量[ %s ] ReSize 失败。类型不支持该操作！",GetName());
		}
		break;
	}
	return FALSE;
}

BOOL			CLBS_Variable::Save(CString& OUT strCode)
{
	return TRUE;
}
inline CString		CLBS_Variable::GetTypeName()
{
	CString		name;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			name ="integer";
		}
		break;
	case LVT_STRING:
		{
			name ="string";
		}
		break;
	case LVT_FLOAT:
		{
			name ="float";
		}
		break;
	case LVT_BINARY_DATA:
		{
			name ="data";
		}
		break;
	case LVT_POINTER:
		{
			name ="pointer";
		}
		break;
	case LVT_CLASS:
		{
			name ="class";
		}
		break;
	case LVT_VAR_ARRAY:
		{
			name ="combineArray";
		}
		break;
	default:
		{
			name="unknown";
		}
		break;
	}
	return name;
}

inline VOID			CLBS_Variable::Clear()
{
	switch(m_VarType)
	{
	case LVT_VAR_ARRAY:
		{
			CLBS_Variable**	pVar =(CLBS_Variable**)m_pVarBuffer;
			for(DWORD i=0;i<GetMemberCount();i++)
			{
				LBS_SafeRelease(pVar[i]);
			}
		}
		break;
	case LVT_CLASS:
		{
			CLBS_Script_ClassBase*	pClass =*(CLBS_Script_ClassBase**)m_pVarBuffer;
			LBS_SafeRelease(pClass);
		}
		break;
	default:
		break;
	}
	m_VarType			=LVT_UNKNOWN;
	m_dwVarSize			=0;
}

inline VOID			CLBS_Variable::Reset()
{
	Clear();
	m_dwSuffix			=0;		
	LBS_SafeRelease(m_pRefVariable);
}
BOOL			CLBS_Variable::SetByOriginString(LPSTR IN pString,BOOL IN bEnableString)
{
	Clear();	
	INT		type	=GetStringType(pString);
	///返回-1 无效，0：字串，1：10进制整数  2: 浮点数 3：16进制整数
	switch(type)
	{
	case 0:
		{
			CLBS_ScriptMacroDefine* pMacro =(CLBS_ScriptMacroDefine*)m_pScriptFile->GetParser()->GetMacro(pString);
			if(pMacro != NULL)
			{//如果是宏定义
				return SetByOriginString((LPSTR)pMacro->GetValue(),TRUE);
			}
			CLBS_Variable*		pVar	=(CLBS_Variable*)GetVariable(pString);
			if(pVar != NULL)
			{//是变量
				*this = *pVar;
				break;
			}

			if(bEnableString)
			{
				m_VarType			=LVT_STRING;
				m_dwVarSize			=(DWORD)strlen(pString);
				
				AdjustVarBuffer(m_dwVarSize+1);

				strncpy((CHAR*)m_pVarBuffer,pString,m_dwVarSize);
				((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;
				break;
			}
			CErrorSystem::GetInstance().AddErrorInfo(NULL,0,"#错误：不能识别的标识符[ %s ].\n",pString);
			return FALSE;
		}
		break;
	case 1:
		{
			DWORD	value		=atoll(pString);
			m_dwVarSize			=sizeof(DWORD);

			AdjustVarBuffer(m_dwVarSize+1);

			memcpy(m_pVarBuffer,&value,m_dwVarSize);
			((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;
			m_VarType			=LVT_INTEGER;

		}
		break;
	case 2:
		{
			DOUBLE	value		=atof(pString);
			m_dwVarSize			=sizeof(DOUBLE);
			AdjustVarBuffer(m_dwVarSize+1);
			memcpy(m_pVarBuffer,&value,m_dwVarSize);
			((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;
			m_VarType			=LVT_FLOAT;
		}
		break;
	case 3:
		{
			DWORD	value		=0;
			sscanf(pString,"%x",&value);
			m_dwVarSize			=sizeof(DWORD);
			AdjustVarBuffer(m_dwVarSize+1);
			memcpy(m_pVarBuffer,&value,m_dwVarSize);
			((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;
			m_VarType			=LVT_INTEGER;
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	SyncRefVariable();
	return TRUE;
}	
inline BOOL			CLBS_Variable::Set(VOID* IN pData,DWORD IN dwSize,LBS_VARIABEL_TYPE IN type)
{
	Clear();
	m_dwVarSize			=dwSize;
	AdjustVarBuffer(m_dwVarSize+1);
	memcpy(m_pVarBuffer,pData,m_dwVarSize);
	((BYTE*)m_pVarBuffer)[m_dwVarSize]=0;

	m_VarType			=type;
	
	switch(type)
	{
	case LVT_VAR_ARRAY:
		{
			CLBS_Variable**	pV =(CLBS_Variable**)m_pVarBuffer;
			for(DWORD i=0;i<GetMemberCount();i++)
			{
				if(pV[i]!=NULL)
					pV[i]->AddRef();
			}
		}
		break;
	case LVT_CLASS:
		{
			CLBS_Script_ClassBase*	pClass	=*(CLBS_Script_ClassBase**)m_pVarBuffer;
			CLBS_Script_ClassBase*	pClone	=pClass->GetClone();
			if(pClone ==NULL)
				return FALSE;
			memcpy(m_pVarBuffer,&pClone,dwSize);
		}
		break;
	default:
		break;

	}
	return SyncRefVariable();
}
inline DWORD			CLBS_Variable::GetMemberCount()
{
	if(m_pVarBuffer == NULL)
		return 0;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			return (m_dwVarSize/(DWORD)sizeof(DWORD));
		}
		break;
	case LVT_STRING:
		{
			return m_dwVarSize;
		}
		break;
	case LVT_FLOAT:
		{
			return (m_dwVarSize/(DWORD)sizeof(DOUBLE));
		}
		break;
	case LVT_POINTER:
		{
			return (m_dwVarSize/(DWORD)sizeof(LPVOID));
		}
		break;
	case LVT_BINARY_DATA:
		{
			return m_dwVarSize;
		}
		break;
	case LVT_VAR_ARRAY:
		{
			return (m_dwVarSize/(DWORD)sizeof(CLBS_Variable*));
		}
		break;
	case LVT_CLASS:
		{
			return (m_dwVarSize/(DWORD)sizeof(CLBS_Script_ClassBase*));
		}
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
inline VOID*			CLBS_Variable::operator[](DWORD IN dwIndex)
{
	if(m_pVarBuffer == NULL)
		return NULL;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(dwIndex >= m_dwVarSize/(DWORD)sizeof(DWORD))
				return NULL;
			return &((DWORD*)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_STRING:
		{
			if(dwIndex >= m_dwVarSize)
				return NULL;
			return &((BYTE*)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_FLOAT:
		{
			if(dwIndex >= m_dwVarSize/(DWORD)sizeof(DOUBLE))
				return NULL;
			return &((DOUBLE*)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_POINTER:
		{
			if(dwIndex >= m_dwVarSize/(DWORD)sizeof(LPVOID))
				return NULL;
			return &((LPVOID*)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_BINARY_DATA:
		{
			if(dwIndex >= m_dwVarSize)
				return NULL;
			return &((BYTE*)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_VAR_ARRAY:
		{
			if(dwIndex >= m_dwVarSize/(DWORD)sizeof(CLBS_Variable*))
				return NULL;
			return &((CLBS_Variable**)m_pVarBuffer)[dwIndex];
		}
		break;
	case LVT_CLASS:
		{
			
			if(dwIndex >= m_dwVarSize/(DWORD)sizeof(CLBS_Script_ClassBase*))
				return NULL;
			return &((CLBS_Script_ClassBase**)m_pVarBuffer)[dwIndex];
			
		}
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}


inline CLBS_Variable&	CLBS_Variable::operator=(LPSTR v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %s ",GetName(),v);
		return *this;
	}

	
	m_VarType			=LVT_STRING;
	m_dwVarSize			=(DWORD)strlen(v);
	AdjustVarBuffer(m_dwVarSize+1);
	strncpy((CHAR*)m_pVarBuffer,v,m_dwVarSize);
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(LPCSTR v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %s ",GetName(),v);
		return *this;
	}

	m_VarType			=LVT_STRING;
	m_dwVarSize			=(DWORD)strlen(v);
	AdjustVarBuffer(m_dwVarSize+1);
	strncpy((CHAR*)m_pVarBuffer,v,m_dwVarSize);
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(CString v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %s ",GetName(),v);
		return *this;
	}

	m_VarType			=LVT_STRING;
	m_dwVarSize			=(DWORD)v.GetLength();
	AdjustVarBuffer(m_dwVarSize+1);
	strncpy((CHAR*)m_pVarBuffer,(CHAR*)(LPCSTR)v,m_dwVarSize);
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(LPVOID v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = ",GetName());
		return *this;
	}

	
	m_VarType			=LVT_POINTER;
	m_dwVarSize			=(DWORD)sizeof(v);
	AdjustVarBuffer(m_dwVarSize+1);
	*((LPVOID*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}

inline CLBS_Variable&	CLBS_Variable::operator=(LONG v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %d ",GetName(),v);
		return *this;
	}

	m_VarType			=LVT_INTEGER;
	m_dwVarSize			=sizeof(v);
	AdjustVarBuffer(m_dwVarSize+1);
	*((LONG*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;


	SyncRefVariable();

	return *this;
}

inline CLBS_Variable&	CLBS_Variable::operator=(DWORD v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %d ",GetName(),v);
		return *this;
	}


	m_VarType			=LVT_INTEGER;
	m_dwVarSize			=sizeof(v);
	AdjustVarBuffer(m_dwVarSize+1);
	
	*((DWORD*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(bool v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %d ",GetName(),v);
		return *this;
	}

	DWORD		val		=v;


	m_VarType			=LVT_INTEGER;
	m_dwVarSize			=sizeof(val);
	AdjustVarBuffer(m_dwVarSize+1);

	*((DWORD*)m_pVarBuffer) = val;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(INT v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %d ",GetName(),v);
		return *this;
	}

	m_VarType			=LVT_INTEGER;
	m_dwVarSize			=sizeof(v);
	AdjustVarBuffer(m_dwVarSize+1);

	*((LONG*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}
inline CLBS_Variable&	CLBS_Variable::operator=(FLOAT v)
{
	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %f ",GetName(),v);
		return *this;
	}

	m_VarType			=LVT_FLOAT;
	m_dwVarSize			=sizeof(DOUBLE);
	AdjustVarBuffer(m_dwVarSize+1);
	
	*((DOUBLE*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;


	SyncRefVariable();

	return *this;
}

inline CLBS_Variable&	CLBS_Variable::operator=(DOUBLE v)
{

	if(m_VarType == LVT_CLASS)
	{
		ShowDebugTrace("无效的赋值: %s = %f ",GetName(),v);
		return *this;
	}


	m_VarType			=LVT_FLOAT;
	m_dwVarSize			=sizeof(DOUBLE);
	AdjustVarBuffer(m_dwVarSize+1);

	*((DOUBLE*)m_pVarBuffer) = v;
	((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;

	SyncRefVariable();

	return *this;
}

inline CLBS_Variable&	CLBS_Variable::operator=(CLBS_Variable& v)
{
	Equal(v);
	return *this;
}
inline BOOL			CLBS_Variable::operator==(CLBS_Variable& v)
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(v.GetVarType())
			{
			case LVT_INTEGER:
				{
					if(GetMemberCount() != v.GetMemberCount())
					{
						if(GetMemberCount()==1)
						{
							if((DWORD)*this == 0 && v.GetMemberCount() == 0)//==null
								return TRUE;
						}
						return FALSE;
					}
					DWORD*	pSrc	=(DWORD*)GetDataBuffer();
					DWORD* pDst		=(DWORD*)v.GetDataBuffer();
					for(DWORD i=0;i<GetMemberCount();i++)
					{
						if(pSrc[i]!=pDst[i])
							return FALSE;
					}
				}
				break;
			case LVT_FLOAT:
				{
					if(GetMemberCount() != v.GetMemberCount())
					{
						if(GetMemberCount()==1)
						{
							if((DWORD)*this == 0 && v.GetMemberCount() == 0)//==null
								return TRUE;
						}
						return FALSE;
					}
					DWORD*	pSrc	=(DWORD*)GetDataBuffer();
					DOUBLE* pDst	=(DOUBLE*)v.GetDataBuffer();
					for(DWORD i=0;i<GetMemberCount();i++)
					{
						if(pSrc[i]!=pDst[i])
							return FALSE;
					}
				}
				break;
			case LVT_POINTER:
				{
					if(GetMemberCount() != v.GetMemberCount())
					{
						if(GetMemberCount()==1)
						{
							if((DWORD)*this == 0 && v.GetMemberCount() == 0)//==null
								return TRUE;
						}
						return FALSE;
					}
					DWORD*	pSrc	=(DWORD*)GetDataBuffer();
					DWORD* pDst		=(DWORD*)v.GetDataBuffer();
					for(DWORD i=0;i<GetMemberCount();i++)
					{
						if(pSrc[i]!=pDst[i])
							return FALSE;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			default:
				{
					if(GetMemberCount() != v.GetMemberCount())
					{
						if(GetMemberCount()==1)
						{
							if((DWORD)*this == 0 && v.GetMemberCount() == 0)//==null
								return TRUE;
						}
						return FALSE;
					}
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_FLOAT:
		{
			switch(v.GetVarType())
			{
			case LVT_INTEGER:
				{
					if(GetMemberCount() != v.GetMemberCount())
						return FALSE;
					DOUBLE*	pSrc	=(DOUBLE*)GetDataBuffer();
					DWORD* pDst		=(DWORD*)v.GetDataBuffer();
					for(DWORD i=0;i<GetMemberCount();i++)
					{
						if(pSrc[i]!=pDst[i])
							return FALSE;
					}
				}
				break;
			case LVT_FLOAT:
				{
					if(GetMemberCount() != v.GetMemberCount())
						return FALSE;

					DOUBLE*	pSrc	=(DOUBLE*)GetDataBuffer();
					DOUBLE* pDst	=(DOUBLE*)v.GetDataBuffer();
					for(DWORD i=0;i<GetMemberCount();i++)
					{
						if(fabs(pSrc[i]-pDst[i]) < 0.000000001)
							return FALSE;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_STRING:
		{
			switch(v.GetVarType())
			{
			case LVT_INTEGER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_POINTER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			case LVT_STRING:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_POINTER:
		{
			switch(v.GetVarType())
			{
			case LVT_INTEGER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_POINTER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_BINARY_DATA:
		{
			switch(v.GetVarType())
			{
			case LVT_INTEGER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_POINTER:
				{
					if(v.GetMemberCount()==1)
					{
						if((DWORD)v == 0 && GetMemberCount() == 0)//==null
							return TRUE;
					}
					return FALSE;
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			case LVT_STRING:
				{
					if(GetDataSize() != v.GetDataSize())
						return FALSE;
					if(memcmp(m_pVarBuffer,v.m_pVarBuffer,GetDataSize())!=0)
						return FALSE;
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			if(m_VarType != v.m_VarType)
				return FALSE;
			if(GetMemberCount() != v.GetMemberCount())
				return FALSE;
			if(memcmp(m_pVarBuffer,v.m_pVarBuffer,m_dwVarSize)!=0)
				return FALSE;
		}
		break;
	}
	return TRUE;
}
inline BOOL			CLBS_Variable::operator!=(CLBS_Variable& v)
{
	return !(*this==v);
}
BOOL			CLBS_Variable::operator>(CLBS_Variable& v)
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(v.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG	this_value =(PLONG)m_pVarBuffer;
					PLONG	oper_value =(PLONG)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			case LVT_FLOAT:
				{
					PLONG	this_value =(PLONG)m_pVarBuffer;
					PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value > oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value > oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{

			switch(v.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
					PLONG	oper_value =(PLONG)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
					PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 > count2);
			}
			
			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )>0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::operator<(CLBS_Variable& v)
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value < oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value < oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 < count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )<0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::operator>=(CLBS_Variable& v)
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value >= oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value >= oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 > count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )>=0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::operator<=(CLBS_Variable& v)
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value <= oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value <= oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 < count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )<=0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}


inline BOOL			CLBS_Variable::Equal(CLBS_Variable& IN Oper)
{
	if(m_VarType == LVT_CLASS)
	{
		CLBS_Script_ClassBase*	pClass1 =*(CLBS_Script_ClassBase**)m_pVarBuffer;
		switch(Oper.m_VarType)
		{
		case LVT_CLASS:
			{
				CLBS_Script_ClassBase*	pClass2 =*(CLBS_Script_ClassBase**)Oper.m_pVarBuffer;
				*pClass1	=*pClass2;

				return TRUE;
			}
			break;
		case LVT_INTEGER:
		case LVT_STRING:
		case LVT_FLOAT:	
		case LVT_BINARY_DATA:
			{
				LPVOID	pMem	=Oper.GetDataBuffer();
				DWORD	size	=Oper.GetDataSize();
				if(!pClass1->ConstructVariableFromMem(pMem,size))
				{
					ShowDebugTrace("赋值操作失败，大小不一致: %s = %s ",GetName(),Oper.GetName());
					return FALSE;
				}
				return TRUE;
			}
			break;

		case LVT_POINTER:
			{
				LPVOID	pMem	=(LPVOID)Oper;
				DWORD	size	=Oper.GetDataSize();
				if(!pClass1->ConstructVariableFromMem(pMem,size))
				{
					ShowDebugTrace("赋值操作失败，大小不一致: %s = %s ",GetName(),Oper.GetName());
					return FALSE;
				}
				return TRUE;
			}
			break;
		default:
			{
				ShowDebugTrace("无效的赋值: %s = %s ",GetName(),Oper.GetName());
				return FALSE;
			}
			break;
		}
	}

	Set(Oper.m_pVarBuffer,Oper.m_dwVarSize,Oper.m_VarType);
	return TRUE;
}
BOOL			CLBS_Variable::Logic_B(CLBS_Variable& IN v)//>
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(v.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG	this_value =(PLONG)m_pVarBuffer;
					PLONG	oper_value =(PLONG)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			case LVT_FLOAT:
				{
					PLONG	this_value =(PLONG)m_pVarBuffer;
					PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value > oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value > oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{

			switch(v.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
					PLONG	oper_value =(PLONG)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
					PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
					return  (this_value[0]>oper_value[0]);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 > count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )>0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::Logic_BE(CLBS_Variable& IN v)//>=
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value >= oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value >= oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]>=oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 > count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )>=0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::Logic_S(CLBS_Variable& IN v)//<
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value < oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value < oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 < count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )<0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::Logic_SE(CLBS_Variable& IN v)//<=
{
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PLONG	this_value =(PLONG)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else
			{
				return FALSE;
			}

		}
		break;
	case LVT_POINTER:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)(LONG)v;
				return (this_value <= oper_value);
			}
			else if(v.m_VarType == LVT_POINTER)
			{
				LPVOID	this_value =(LPVOID)*this;
				LPVOID	oper_value =(LPVOID)v;
				return (this_value <= oper_value);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_FLOAT:
		{
			if(v.m_VarType == LVT_INTEGER)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PLONG	oper_value =(PLONG)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else if(v.m_VarType == LVT_FLOAT)
			{
				PDOUBLE	this_value =(PDOUBLE)m_pVarBuffer;
				PDOUBLE	oper_value =(PDOUBLE)v.m_pVarBuffer;
				return  (this_value[0]<=oper_value[0]);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LVT_STRING:
		{
			DWORD count1=GetMemberCount();
			DWORD count2=v.GetMemberCount();
			if(count1 != count2)
			{
				return (count1 < count2);
			}

			return (strncmp( (LPCSTR)m_pVarBuffer , (LPCSTR)v.m_pVarBuffer , count1 )<=0);
		}
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL			CLBS_Variable::Logic_E(CLBS_Variable& IN v)//==
{
	return *this==v;
}
BOOL			CLBS_Variable::Logic_NE(CLBS_Variable& IN v)//!=
{
	return *this != v;

}







BOOL			CLBS_Variable::Add(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(self_member_count != oper_member_count)
			{
				if( (self_member_count !=1 && oper_member_count != 1) ||
					(self_member_count ==0 || oper_member_count == 0))
				{
					ShowDebugTrace("操作的变量维数不相同！");
					return FALSE;
				}
			}
			DWORD	maxCount	=self_member_count>oper_member_count?self_member_count:oper_member_count;

			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG	v1=(PLONG)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] +=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					if(self_member_count != oper_member_count)
						return FALSE;
					PLONG	v1=(PLONG)	m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] +=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持加法运算！%s",GetName());
					return FALSE;
				}
				break;
			}			
		}
		break;
	case LVT_FLOAT:
		{
			if(self_member_count != oper_member_count)
			{
				if( (self_member_count !=1 && oper_member_count != 1) ||
					(self_member_count ==0 || oper_member_count == 0))
				{
					ShowDebugTrace("操作的变量维数不相同！");
					return FALSE;
				}
			}
			DWORD	maxCount	=self_member_count>oper_member_count?self_member_count:oper_member_count;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)	Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] +=(DOUBLE)v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] +=(DOUBLE)v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持加法运算！%s",GetName());;
					return FALSE;
				}
				break;
			}		
		}
		break;
	case LVT_STRING:
		{
			switch(Oper.m_VarType)
			{
			case LVT_STRING:
				{
					DWORD	size		=m_dwVarSize+Oper.m_dwVarSize;

					if(m_dwVarBufferSize <= size)
					{
						m_dwVarBufferSize	=size+1;

						VOID*	pBuffer	=LB_ALLOC(m_dwVarBufferSize);
						ZeroMemory(pBuffer,m_dwVarBufferSize);
						strncpy((CHAR*)pBuffer,(CHAR*)m_pVarBuffer,m_dwVarSize);
						LB_FREE(m_pVarBuffer);

						m_pVarBuffer	=pBuffer;

					}


					strncat((CHAR*)m_pVarBuffer,(CHAR*)Oper.m_pVarBuffer,Oper.m_dwVarSize);
					m_dwVarSize		=strlen((CHAR*)m_pVarBuffer);

					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持加法运算！%s",GetName());;
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_BINARY_DATA:
		{
			switch(Oper.m_VarType)
			{
			case LVT_BINARY_DATA:
				{
					DWORD	size		=m_dwVarSize+Oper.m_dwVarSize;
					m_dwVarBufferSize	=size+1;

					VOID*	pBuffer	=LB_ALLOC(m_dwVarBufferSize);
					memcpy((CHAR*)pBuffer,(CHAR*)m_pVarBuffer,m_dwVarSize);
					memcpy((CHAR*)pBuffer+m_dwVarSize,(CHAR*)Oper.m_pVarBuffer,Oper.m_dwVarSize);

					LB_FREE(m_pVarBuffer);
					m_dwVarSize		=size;
					m_pVarBuffer	=pBuffer;
					((CHAR*)m_pVarBuffer)[m_dwVarSize]=0;
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持加法运算！%s",GetName());;
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			ShowDebugTrace("操作的变量类型不支持加法运算！%s",GetName());;
			return FALSE;
		}
		break;
	}

	return TRUE;
}
BOOL			CLBS_Variable::Sub(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	if(self_member_count != oper_member_count)
	{
		if( (self_member_count !=1 && oper_member_count != 1) ||
			(self_member_count ==0 || oper_member_count == 0))
		{
			ShowDebugTrace("操作的变量维数不相同！");
			return FALSE;
		}
	}
	DWORD	maxCount	=self_member_count>oper_member_count?self_member_count:oper_member_count;

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG	v1=(PLONG)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] -=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					PLONG	v1=(PLONG)	m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] -=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持减法运算！");
					return FALSE;
				}
				break;
			}			
		}
		break;
	case LVT_FLOAT:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)	Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] -=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] -=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持减法运算！");
					return FALSE;
				}
				break;
			}		
		}
		break;
	default:
		{
			ShowDebugTrace("操作的变量类型不支持减法运算！");
			return FALSE;
		}
		break;
	}

	return TRUE;
}

BOOL			CLBS_Variable::Mul(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	if(self_member_count != oper_member_count)
	{
		if( (self_member_count !=1 && oper_member_count != 1) ||
			(self_member_count ==0 || oper_member_count == 0))
		{
			ShowDebugTrace("操作的变量维数不相同！");
			return FALSE;
		}
	}
	DWORD	maxCount	=self_member_count>oper_member_count?self_member_count:oper_member_count;

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG v1,v2;
					v1=(PLONG)m_pVarBuffer;
					v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count]*=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					ToFloat();

					PDOUBLE v1,v2;
					v1 =(PDOUBLE)m_pVarBuffer;
					v2 =(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count]*=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持乘法运算！");
					return FALSE;
				}
				break;
			}			
		}
		break;
	case LVT_FLOAT:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)	Oper.m_pVarBuffer;

					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count]*=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE v1,v2;
					v1 =(PDOUBLE)m_pVarBuffer;
					v2 =(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count]*=v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持乘法运算！");
					return FALSE;
				}
				break;
			}		
		}
		break;
	default:
		{
			ShowDebugTrace("操作的变量类型不支持乘法运算！");
			return FALSE;
		}
		break;
	}

	return TRUE;
}
BOOL			CLBS_Variable::Div(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();

	if(self_member_count != oper_member_count)
	{
		if( (self_member_count !=1 && oper_member_count != 1) ||
			(self_member_count ==0 || oper_member_count == 0))
		{
			ShowDebugTrace("操作的变量维数不相同！");
			return FALSE;
		}
	}
	DWORD	maxCount	=self_member_count>oper_member_count?self_member_count:oper_member_count;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PLONG v1,v2;
					v1=(PLONG)m_pVarBuffer;
					v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] /= v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					ToFloat();
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PDOUBLE v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] /= v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持除法运算！");
					return FALSE;
				}
				break;
			}			
		}
		break;
	case LVT_FLOAT:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)	Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] /= v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					PDOUBLE v1,v2;
					v1 =(PDOUBLE)m_pVarBuffer;
					v2 =(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<maxCount;i++)
					{
						v1[i%self_member_count] /= v2[i%oper_member_count];
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持除法运算！");
					return FALSE;
				}
				break;
			}		
		}
		break;
	default:
		{
			ShowDebugTrace("操作的变量类型不支持除法运算！");
			return FALSE;
		}
		break;
	}

	return TRUE;
}
BOOL			CLBS_Variable::Mod(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					if(self_member_count != oper_member_count)
						return FALSE;
					PLONG	v1=(PLONG)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						v1[i]%=v2[i];
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					if(self_member_count != oper_member_count)
						return FALSE;
					PLONG	v1=(PLONG)m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						v1[i] =(LONG)fmod(v1[i],v2[i]);
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持除法运算！");
					return FALSE;
				}
				break;
			}			
		}
		break;
	case LVT_FLOAT:
		{
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					if(self_member_count != oper_member_count)
						return FALSE;
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						v1[i] =fmod(v1[i],v2[i]);
					}
					return TRUE;
				}
				break;
			case LVT_FLOAT:
				{
					if(self_member_count != oper_member_count)
						return FALSE;
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						v1[i] =fmod(v1[i],v2[i]);
					}
					return TRUE;
				}
				break;
			default:
				{
					ShowDebugTrace("操作的变量类型不支持除法运算！");
					return FALSE;
				}
				break;
			}		
		}
		break;
	default:
		{
			ShowDebugTrace("操作的变量类型不支持除法运算！");
			return FALSE;
		}
		break;
	}

	return TRUE;
}

BOOL			CLBS_Variable::AND(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(Oper.m_VarType != LVT_INTEGER )
				return FALSE;
			if(self_member_count != oper_member_count)
				return FALSE;
			PDWORD v1=(PDWORD)m_pVarBuffer;
			PDWORD v2=(PDWORD)Oper.m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] &= v2[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			DWORD	index =0;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PDWORD	v2 =(PDWORD)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] &= v2[index];
						index++;
					}
				}
				break;
			case LVT_STRING:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] &= v2[index];
						index++;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] &= v2[index];
						index++;
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL			CLBS_Variable::OR(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(Oper.m_VarType != LVT_INTEGER )
				return FALSE;
			if(self_member_count != oper_member_count)
				return FALSE;
			PDWORD v1=(PDWORD)m_pVarBuffer;
			PDWORD v2=(PDWORD)Oper.m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] |= v2[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			DWORD	index =0;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PDWORD	v2 =(PDWORD)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] |= v2[index];
						index++;
					}
				}
				break;
			case LVT_STRING:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] |= v2[index];
						index++;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] |= v2[index];
						index++;
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL			CLBS_Variable::NOT()
{
	DWORD	self_member_count	=GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			PDWORD v1=(PDWORD)m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] = ~v1[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			PBYTE v1=(PBYTE)m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] = ~v1[i];
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL			CLBS_Variable::XOR(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(Oper.m_VarType != LVT_INTEGER )
				return FALSE;
			if(self_member_count != oper_member_count)
				return FALSE;
			PDWORD v1=(PDWORD)m_pVarBuffer;
			PDWORD v2=(PDWORD)Oper.m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] ^= v2[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			DWORD	index =0;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PDWORD	v2 =(PDWORD)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] ^= v2[index];
						index++;
					}
				}
				break;
			case LVT_STRING:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] ^= v2[index];
						index++;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] ^= v2[index];
						index++;
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL		CLBS_Variable::LMB(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(Oper.m_VarType != LVT_INTEGER )
				return FALSE;
			if(self_member_count != oper_member_count)
				return FALSE;
			PDWORD v1=(PDWORD)m_pVarBuffer;
			PDWORD v2=(PDWORD)Oper.m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] <<= v2[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			DWORD	index =0;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PDWORD	v2 =(PDWORD)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] <<= v2[index];
						index++;
					}
				}
				break;
			case LVT_STRING:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] <<= v2[index];
						index++;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] <<= v2[index];
						index++;
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL		CLBS_Variable::RMB(CLBS_Variable& IN Oper)
{
	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper.GetMemberCount();
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			if(Oper.m_VarType != LVT_INTEGER )
				return FALSE;
			if(self_member_count != oper_member_count)
				return FALSE;
			PDWORD v1=(PDWORD)m_pVarBuffer;
			PDWORD v2=(PDWORD)Oper.m_pVarBuffer;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] >>= v2[i];
			}
			return TRUE;
		}
		break;
	case LVT_BINARY_DATA:
		{
			DWORD	index =0;
			switch(Oper.m_VarType)
			{
			case LVT_INTEGER:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PDWORD	v2 =(PDWORD)Oper.m_pVarBuffer;

					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] >>= v2[index];
						index++;
					}
				}
				break;
			case LVT_STRING:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] >>= v2[index];
						index++;
					}
				}
				break;
			case LVT_BINARY_DATA:
				{
					PBYTE	v1 =(PBYTE)m_pVarBuffer;
					PBYTE	v2 =(PBYTE)Oper.m_pVarBuffer;
					for(DWORD i=0;i<self_member_count;i++)
					{
						if(index>=oper_member_count)
							index =0;
						v1[i] >>= v2[index];
						index++;
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

BOOL			CLBS_Variable::LAND(CLBS_Variable& IN Oper)
{
	return (!LNOT())&&(!Oper.LNOT());
}
BOOL			CLBS_Variable::LOR(CLBS_Variable& IN Oper)
{
	return (!LNOT())||(!Oper.LNOT());
}
BOOL			CLBS_Variable::LNOT()
{//逻辑非
	if(m_pVarBuffer==NULL)
		return TRUE;
	return IsNull();
}
BOOL		CLBS_Variable::Minus()//取负
{
	if(m_pVarBuffer==NULL)
		return FALSE;
	DWORD	self_member_count	=GetMemberCount();
	if(self_member_count==0)
		return FALSE;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			PLONG	v1=(PLONG)*this;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] = -v1[i];
			}
		}
		break;
	case LVT_FLOAT:
		{
			PDOUBLE	v1=(PDOUBLE)*this;
			for(DWORD i=0;i<self_member_count;i++)
			{
				v1[i] = -v1[i];
			}
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
BOOL			CLBS_Variable::SetSuffixValue(DWORD IN dwIndex,CLBS_Variable* IN Oper)
{
	if(m_pVarBuffer==NULL)
		return FALSE;
	if(dwIndex > 0x40000000)
	{
		ShowDebugTrace("对变量[%s]引用了无效的下标值[%d].",GetName(),dwIndex);
		return NULL;
	}


	DWORD	self_member_count	=GetMemberCount();
	DWORD	oper_member_count	=Oper->GetMemberCount();

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			switch(Oper->m_VarType)
			{
			case LVT_INTEGER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					PLONG	v1=(PLONG)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =v2[i];
					}
				}
				break;
			case LVT_FLOAT:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					PLONG	v1=(PLONG)	m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(LONG)v2[i];
					}				
				}
				break;
			case LVT_STRING:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					
					PLONG	v1=(PLONG)	m_pVarBuffer;
					v1[dwIndex] =atoll((LPCSTR)Oper->m_pVarBuffer);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_POINTER:
		{
			switch(Oper->m_VarType)
			{
			case LVT_INTEGER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					LPVOID* v1=(LPVOID*)m_pVarBuffer;
					LPDWORD v2=(LPDWORD)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(LPVOID)v2[i];
					}
				}
				break;
			case LVT_POINTER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					LPVOID* v1=(LPVOID*)m_pVarBuffer;
					LPVOID* v2=(LPVOID*)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(LPVOID)v2[i];
					}
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_FLOAT:
		{
			switch(Oper->m_VarType)
			{
			case LVT_INTEGER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
				
					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PLONG	v2=(PLONG)	Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(DOUBLE)v2[i];
					}				
					
				}
				break;
			case LVT_FLOAT:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;

					PDOUBLE	v1=(PDOUBLE)m_pVarBuffer;
					PDOUBLE	v2=(PDOUBLE)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =v2[i];
					}				
				}
				break;
			case LVT_STRING:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;

					PLONG	v1=(PLONG)	m_pVarBuffer;
					v1[dwIndex] =atof((LPCSTR)Oper->m_pVarBuffer);
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_STRING:
		{
			switch(Oper->m_VarType)
			{
			case LVT_INTEGER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					
					PCHAR	v1=(PCHAR)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(CHAR)v2[i];
					}				
				}
				break;
			case LVT_STRING:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					PCHAR	v1=(PCHAR)m_pVarBuffer;
					PCHAR	v2=(PCHAR)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =v2[i];
					}				
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_BINARY_DATA:
		{
			switch(Oper->m_VarType)
			{
			case LVT_INTEGER:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;

					PBYTE	v1=(PBYTE)m_pVarBuffer;
					PLONG	v2=(PLONG)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =(BYTE)v2[i];
					}				
				}
				break;
			case LVT_BINARY_DATA:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					PBYTE	v1=(PBYTE)m_pVarBuffer;
					PBYTE	v2=(PBYTE)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =v2[i];
					}				

				}
				break;
			case LVT_STRING:
				{
					if(!Expand(oper_member_count+dwIndex,Oper->IsFullRef()))
						return FALSE;
					PBYTE	v1=(PBYTE)m_pVarBuffer;
					PBYTE	v2=(PBYTE)Oper->m_pVarBuffer;
					for(DWORD i=0;i<oper_member_count;i++)
					{
						v1[i+dwIndex] =v2[i];
					}				
				}
				break;
			default:
				{
					return FALSE;
				}
				break;
			}
		}
		break;
	case LVT_VAR_ARRAY:
		{
			if(!Expand(1+dwIndex,Oper->IsFullRef()))
				return FALSE;

			CLBS_Variable** v1=(CLBS_Variable**)m_pVarBuffer;
			LBS_SafeRelease(v1[dwIndex]);
			v1[dwIndex] =Oper;
			if(Oper != NULL)
				Oper->AddRef();
		}
		break;
	default:
		{
            Equal(*Oper);
            return TRUE;
		}
		break;
	}

	return TRUE;
}

CLBS_Variable*			CLBS_Variable::GetSuffixValue(DWORD IN dwIndex)
{
	if(m_pVarBuffer==NULL)
		return NULL;

	if(dwIndex > 0x40000000)
	{
		ShowDebugTrace("对变量[%s]引用了无效的下标值[%d].",GetName(),dwIndex);
		return NULL;
	}

	if(!Expand(dwIndex+1))
		return NULL;

	CLBS_Variable*	pRet	=new CLBS_Variable(GetParent());

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			pRet->Set((*this)[dwIndex],sizeof(LONG),m_VarType);
		}
		break;
	case LVT_POINTER:
		{
			pRet->Set((*this)[dwIndex],sizeof(LPVOID),m_VarType);
		}
		break;
	case LVT_FLOAT:
		{
			pRet->Set((*this)[dwIndex],sizeof(DOUBLE),m_VarType);
		}
		break;
	case LVT_STRING:
		{
			pRet->Set((*this)[dwIndex],sizeof(BYTE),m_VarType);
		}
		break;
	case LVT_BINARY_DATA:
		{
			pRet->Set((*this)[dwIndex],sizeof(BYTE),m_VarType);
		}
		break;
	case LVT_VAR_ARRAY:
		{
			CLBS_Variable*	pVar =*(CLBS_Variable**)(*this)[dwIndex];
			if(pVar == NULL)
			{
				return NULL;
			}
			*pRet =*pVar;
		}
		break;
	default:
		{
			LBS_SafeRelease(pRet);
			return pRet;
		}
		break;
	}

	pRet->SetRefVaraibleInfo(dwIndex,this,FALSE);
	return pRet;
}	

inline BOOL					CLBS_Variable::SyncRefVariable()	//同步数值到引用的变量
{
	if(m_pRefVariable == NULL)
		return TRUE;
	if(m_pRefVariable->m_VarType == LVT_UNKNOWN)
	{
		*m_pRefVariable =*this;
	}
	return m_pRefVariable->SetSuffixValue(m_dwSuffix,this);
}
	
VOID					CLBS_Variable::SetRefVaraibleInfo(DWORD IN dwSuffix,CLBS_Variable* IN pRefVar,BOOL IN bFullRef)
{
	LBS_SafeRelease(m_pRefVariable);
	m_dwSuffix		=dwSuffix;
	m_bFullRef		=bFullRef;
	if(pRefVar != NULL)
	{
		if(pRefVar->m_VarType != m_VarType)
			return;
	}

	m_pRefVariable	=pRefVar;

	if(m_pRefVariable != NULL)
		m_pRefVariable->AddRef();
}
BOOL					CLBS_Variable::Expand(DWORD IN dwMemberCount,BOOL IN bCut)
{
	if(dwMemberCount > 0x40000000)
		return FALSE;
	DWORD	count	=GetMemberCount();
	if(count == dwMemberCount)
		return TRUE;
	if(count > dwMemberCount && !bCut)
		return TRUE;

	DWORD	dwBufferSize=0,dwVarSize=0;
	LPSTR	pBuffer=NULL;
	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			dwVarSize			=dwMemberCount*sizeof(LONG);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;

			ZeroMemory(pBuffer,dwBufferSize);
			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;


		}
		break;
	case LVT_POINTER:
		{
			dwVarSize			=dwMemberCount*sizeof(LPVOID);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;

			ZeroMemory(pBuffer,dwBufferSize);
			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;
		}
		break;
	case LVT_FLOAT:
		{
			dwVarSize			=dwMemberCount*sizeof(DOUBLE);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;

			ZeroMemory(pBuffer,dwBufferSize);
			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;

		}
		break;
	case LVT_STRING:
		{
			dwVarSize			=dwMemberCount*sizeof(BYTE);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;

			ZeroMemory(pBuffer,dwBufferSize);
			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;

		}
		break;
	case LVT_BINARY_DATA:
		{
			dwVarSize			=dwMemberCount*sizeof(BYTE);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;
			ZeroMemory(pBuffer,dwBufferSize);

			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;
		}
		break;
	case LVT_VAR_ARRAY:
		{
			dwVarSize			=dwMemberCount*sizeof(CLBS_Variable*);
			dwBufferSize		=dwVarSize+1;
			pBuffer				=(LPSTR)LB_ALLOC(dwBufferSize);
			if(pBuffer==NULL)
				return FALSE;

			ZeroMemory(pBuffer,dwBufferSize);
			if(m_pVarBuffer != NULL)
			{
				memcpy(pBuffer,m_pVarBuffer,m_dwVarSize);
				LB_FREE(m_pVarBuffer);
			}
			m_dwVarSize			=dwVarSize;
			m_dwVarBufferSize	=dwBufferSize;
			m_pVarBuffer		=pBuffer;

		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}
VOID					CLBS_Variable::ReverseOrder()
{
	DWORD	member_count =GetMemberCount();
	if(member_count<2)
		return;

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			PLONG	p		=(PLONG)m_pVarBuffer;
			LONG	tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}
		}
		break;
	case LVT_POINTER:
		{
			LPVOID*	p		=(LPVOID*)m_pVarBuffer;
			LPVOID	tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}
		}
		break;
	case LVT_FLOAT:
		{
			PDOUBLE	p		=(PDOUBLE)m_pVarBuffer;
			DOUBLE	tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}	

		}
		break;
	case LVT_STRING:
		{
			LPSTR p =(LPSTR)m_pVarBuffer;
			CHAR	tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}	
		}
		break;
	case LVT_BINARY_DATA:
		{
			PBYTE	p=(PBYTE)m_pVarBuffer;
			BYTE	tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}
		}
		break;
	case LVT_VAR_ARRAY:
		{
			CLBS_Variable**		p =(CLBS_Variable**)m_pVarBuffer;
			CLBS_Variable*		tmp;
			DWORD	index	=member_count-1;
			for(DWORD i=0;i<member_count/2;i++,index--)
			{
				tmp		=p[i];
				p[i]	=p[index];
				p[index]=tmp;				
			}
		}
		break;
	default:
		break;
	}
}
VOID		CLBS_Variable::ReverseByte()
{
	if(m_dwVarSize < 2)
		return;

	PBYTE	p=(PBYTE)m_pVarBuffer;
	BYTE	tmp;
	LONG	index	=m_dwVarSize-1;
	for(DWORD i=0;i<m_dwVarSize/2;i++,index--)
	{
		tmp		=p[i];
		p[i]	=p[index];
		p[index]=tmp;				
	}
}
BOOL		CLBS_Variable::RemoveAt(LONG IN lIndex,DWORD IN dwCount)
{
	DWORD	count	=GetMemberCount();
	if(lIndex >= (LONG)count || dwCount ==0)
		return FALSE;

	DWORD	remove_count =min(dwCount,count-lIndex);

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			PLONG	pData	=(PLONG)m_pVarBuffer;
			DWORD	i=0;
			for(i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(LONG)*remove_count;
		}
		break;
	case LVT_STRING:
		{
			PBYTE	pData	=(PBYTE)m_pVarBuffer;
			DWORD i=0;
			for( i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(BYTE)*remove_count;
		}
		break;
	case LVT_FLOAT:
		{
			PDOUBLE	pData	=(PDOUBLE)m_pVarBuffer;
			DWORD i=0;
			for( i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(DOUBLE)*remove_count;
		}
		break;
	case LVT_BINARY_DATA:
		{
			PBYTE	pData	=(PBYTE)m_pVarBuffer;
			DWORD i=0;
			for( i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(BYTE)*remove_count;
		}
		break;
	case LVT_VAR_ARRAY:
		{
			CLBS_Variable**	pData	=(CLBS_Variable**)m_pVarBuffer;

			pData[lIndex]->Release();
			DWORD i=0;
			for( i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(CLBS_Variable*)*remove_count;
		}
		break;
	case LVT_POINTER:
		{
			LPVOID*	pData	=(LPVOID*)m_pVarBuffer;
			DWORD i=0;
			for( i=lIndex;i<count-remove_count;i++)
			{
				pData[i]=pData[i+remove_count];
			}
			pData[i]=0;
			m_dwVarSize -= sizeof(LPVOID)*remove_count;
		}
		break;
	default:
		{
			return FALSE;
		}
	}
	return TRUE;
}
CLBS_Variable::operator BOOL()
{
	return !IsNull();
}
CLBS_Variable::operator LONG()
{
	LONG*	p =(LONG*)m_pVarBuffer;
	if(p==NULL)
		return 0;
	return *p;
}
CLBS_Variable::operator DWORD()
{
	DWORD*	p =(DWORD*)m_pVarBuffer;
	if(p==NULL)
		return 0;
	return *p;
}
CLBS_Variable::operator DOUBLE()
{
	DOUBLE*	p =(DOUBLE*)m_pVarBuffer;
	if(p==NULL)
		return 0;
	return *p;
}

CLBS_Variable::operator LPVOID()
{
    LPVOID*	p = (LPVOID*)m_pVarBuffer;
    if (p == NULL)
        return 0;
    return *p;
}
CLBS_Variable::operator LPCSTR()
{
	return (LPCSTR)m_pVarBuffer;
}
CLBS_Variable::operator PLONG()
{
	return (PLONG)m_pVarBuffer;
}
CLBS_Variable::operator PBYTE()
{
	return (PBYTE)m_pVarBuffer;
}
CLBS_Variable::operator PCHAR()
{
	return (PCHAR)m_pVarBuffer;
}
CLBS_Variable::operator PDOUBLE()
{
	return (PDOUBLE)m_pVarBuffer;
}
CLBS_Variable::operator LPVariablePtr()
{
    return (LPVariablePtr)m_pVarBuffer;
}






inline CLBS_ObjBase*	CLBS_Variable::GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent/* =TRUE */)
{
	return CLBS_Script_FunctionMap::GetInstance().GetVariablePrivateFunction(pFuncName);
}
BOOL			CLBS_Variable::ToFloat()
{
	if(m_VarType == LVT_INTEGER)
	{
		DWORD		count	=GetMemberCount();
		m_dwVarSize			=count*sizeof(DOUBLE);
		m_dwVarBufferSize	=m_dwVarSize+1;
		m_VarType			=LVT_FLOAT;

		PDOUBLE		pBuf	=(PDOUBLE)LB_ALLOC(m_dwVarBufferSize);
		((BYTE*)pBuf)[m_dwVarSize]=0;

		PLONG		pData	=(PLONG)m_pVarBuffer;
		for(DWORD i=0;i<count;i++)
		{
			pBuf[i]		=(DOUBLE)pData[i];
		}

		LB_FREE(m_pVarBuffer);
		m_pVarBuffer	=pBuf;

		return TRUE;
	}
	if(m_VarType == LVT_FLOAT)
		return TRUE;
	return FALSE;
}
BOOL			CLBS_Variable::ToInteger()
{
	if(m_VarType == LVT_FLOAT)
	{
		DWORD		count	=GetMemberCount();
		m_dwVarSize			=count*sizeof(LONG);
		m_dwVarBufferSize	=m_dwVarSize+1;
		m_VarType			=LVT_FLOAT;

		PLONG		pBuf	=(PLONG)LB_ALLOC(m_dwVarBufferSize);
		((BYTE*)pBuf)[m_dwVarSize]=0;

		PDOUBLE		pData	=(PDOUBLE)m_pVarBuffer;
		for(DWORD i=0;i<count;i++)
		{
			pBuf[i]		=(LONG)pData[i];
		}

		LB_FREE(m_pVarBuffer);
		m_pVarBuffer	=pBuf;
		return TRUE;
	}
	if(m_VarType == LVT_INTEGER)
		return TRUE;
	return FALSE;
}
BOOL			CLBS_Variable::IsNull()
{
	if(m_pVarBuffer == NULL)
		return TRUE;
	DWORD count =GetMemberCount();
	if(count == 0)
		return TRUE;
	if(count > 1)
		return FALSE;

	switch(m_VarType)
	{
	case LVT_INTEGER:
		{
			PLONG	v =(PLONG)m_pVarBuffer;
			if(v[0] ==0)
				return TRUE;
		}
		break;
	case LVT_POINTER:
		{
			LPVOID* v=(LPVOID*)m_pVarBuffer;
			if(v[0]==NULL)
				return TRUE;
		}
		break;
	case LVT_UNKNOWN:
		{
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}





