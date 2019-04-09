#include "StdAfx.h"
#include "./LBS_ApiImplement.h"
#include "./LBS_Variable.h"
#include "./LBS_Script_Timer.h"
#include "./LBS_Function.h"
#include "./LBS_ScriptThread.h"
#include "./LBS_Script_Lock.h"
#include "./LBS_Script_UDP.h"
#include "./LBS_Script_TCP.h"

CLBS_ApiImplement::CLBS_ApiImplement(void)
{
}

CLBS_ApiImplement::~CLBS_ApiImplement(void)
{
}


CLBS_ObjBase*	CLBS_ApiImplement::var_length(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet =(LONG)((CLBS_Variable*)pThis)->GetMemberCount();
	return pRet;
}
//得到变量字节数量
CLBS_ObjBase*	CLBS_ApiImplement::var_bytes(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet =(LONG)((CLBS_Variable*)pThis)->GetDataSize();
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::var_getByte(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pSelf	=(CLBS_Variable*)pThis;

	switch(paraList.GetCount())
	{
	case 0:
		{
			pRet =new CLBS_Variable(NULL);
			pRet->Set(pSelf->GetDataBuffer(),pSelf->GetDataSize(),LVT_BINARY_DATA);
		}
		break;
	case 1:
		{
			CLBS_Variable*	pPara1 =(CLBS_Variable*)paraList.GetObj(0);
			if(pPara1->GetVarType() != LVT_INTEGER)
			{
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}

			LONG	index =(LONG)*pPara1;
			if(index >= pSelf->GetDataSize() || index < 0)
			{
				errorCode =LBS_AEC_PARA_INVALID;
				return NULL;
			}

			pRet =new CLBS_Variable(NULL);
			pRet->Set((BYTE*)pSelf->GetDataBuffer()+index,pSelf->GetDataSize()-index,LVT_BINARY_DATA);

		}
		break;
	case 2:
		{
			CLBS_Variable*	pPara1 =(CLBS_Variable*)paraList.GetObj(0);
			CLBS_Variable*	pPara2 =(CLBS_Variable*)paraList.GetObj(1);
			if(pPara1->GetVarType() != LVT_INTEGER ||
				pPara2->GetVarType() != LVT_INTEGER)
			{
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}

			LONG	index  =(LONG)*pPara1;
			LONG	size   =(LONG)*pPara2;

			if(index >= pSelf->GetDataSize() || index < 0)
			{
				errorCode =LBS_AEC_PARA_INVALID;
				return NULL;
			}
			
			LONG	leftSize =pSelf->GetDataSize()-index;
			if(size > leftSize)
			{
				errorCode =LBS_AEC_PARA_INVALID;
				return NULL;
			}
			if(size < 0)
				size = leftSize;

			pRet =new CLBS_Variable(NULL);
			pRet->Set((BYTE*)pSelf->GetDataBuffer()+index,size,LVT_BINARY_DATA);

		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
		break;
	}
	
	return pRet;
}


//转换为字符串
CLBS_ObjBase*	CLBS_ApiImplement::var_toString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	CHAR	format_str[128];
	if(paraList.GetCount()==0 )
	{
		if(pVar->GetVarType() == LVT_FLOAT)
			sprintf(format_str,"%%f");
		else
			sprintf(format_str,"%%d");
	}
	else if(paraList.GetCount()==1)
	{
		CLBS_Variable*	pPara1 =(CLBS_Variable*)paraList.GetObj(0);
		if(pPara1->GetVarType() != LVT_INTEGER)
		{
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		if(pVar->GetVarType() == LVT_FLOAT)
			sprintf(format_str,"%%.0%df",(LONG)*pPara1);
		else
			sprintf(format_str,"%%0%dd",(LONG)*pPara1);
	}
	else
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	CHAR			tmp[256];
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			*pRet	=*pVar;
		}
		break;
	case LVT_INTEGER:
		{
			sprintf(tmp,format_str,(LONG)*pVar);
			pRet->Set(tmp,(DWORD)strlen(tmp),LVT_STRING);
		}
		break;
	case LVT_POINTER:
		{
			sprintf(tmp,format_str,(LONG)(LPVOID)*pVar);
			pRet->Set(tmp,(DWORD)strlen(tmp),LVT_STRING);
		}
		break;
	case LVT_FLOAT:
		{
			sprintf(tmp,format_str,(DOUBLE)*pVar);
			pRet->Set(tmp,(DWORD)strlen(tmp),LVT_STRING);
		}
		break;
	case LVT_BINARY_DATA:
		{
			*pRet	=(LPCSTR)*pVar;
		}
		break;
	default:
		{
			*pRet	=(LPCSTR)"null";
		}
		break;
	}
	return pRet;
}
//转换为16进制字串
CLBS_ObjBase*	CLBS_ApiImplement::var_toHexString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;

	PBYTE	pData	=(PBYTE)pVar->GetDataBuffer();
	LONG	size	=(LONG)pVar->GetDataSize();

	CHAR*	pStr	=(CHAR*)LB_ALLOC( (size+2)*2);
	CHAR*	pPos	=pStr;

	strcpy(pPos,"0x");pPos+=2;

	for(LONG i=size-1;i>=0;i--)
	{
		sprintf(pPos,"%02x",pData[i]);
		pPos+=2;
	}
	*pPos =0;

	pRet =new CLBS_Variable(pVar->GetParent());

	*pRet =(LPCSTR)pStr;

    LB_FREE((LPVoid&)pStr);

	return pRet;
}

//转换为整型
CLBS_ObjBase*	CLBS_ApiImplement::var_toInteger(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	BOOL b32Integer	=TRUE;

	switch(paraList.GetCount())
	{
	case 0:
		{
		}
		break;
	case 1:
		{
			CLBS_Variable*	pPara1	=(CLBS_Variable*)paraList.GetObj(0);
			b32Integer	=(BOOL)*pPara1;
		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
		break;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			*pRet	=(LONG)atoll((LPCSTR)*pVar);
		}
		break;
	case LVT_INTEGER:
		{
			*pRet	=*pVar;
		}
		break;
	case LVT_POINTER:
		{
			*pRet	=(LONG)(LPVOID)*pVar;
		}
		break;
	case LVT_FLOAT:
		{
			*pRet	=(LONG)(DOUBLE)*pVar;
		}
		break;
	case LVT_BINARY_DATA:
		{
			*pRet =(LONG)0;
			DWORD	size =pVar->GetDataSize();
			if(size < 5)
			{
				LONG value =0;
				memcpy(&value,pVar->GetDataBuffer(),min(sizeof(LONG),size));
				*pRet	=value;
			}
			else
			{
				if(b32Integer)
				{
					DWORD count		=size/sizeof(LONG);
					if(count > 0)
					{
						pRet->Expand(count);
						LONG*		pData	=(LONG*)pRet->GetDataBuffer();
						LONG*		pOrg	=(LONG*)pVar->GetDataBuffer();
						for(DWORD i=0;i<count;i++)
							pData[i]=(LONG)pOrg[i];
					}
				}
				else
				{
					DWORD count		=size/sizeof(__int64);
					if(count > 0)
					{
						pRet->Expand(count);
						LONG*		pData	=(LONG*)pRet->GetDataBuffer();
						__int64*	pOrg	=(__int64*)pVar->GetDataBuffer();
						for(DWORD i=0;i<count;i++)
							pData[i]=(LONG)pOrg[i];
					}
				}
			}
		}
		break;
	default:
		{
			*pRet	=(LONG)0;
		}
		break;
	}
	return pRet;
}
//转换为浮点数
CLBS_ObjBase*	CLBS_ApiImplement::var_toFloat(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	

	BOOL b32Float	=FALSE;

	switch(paraList.GetCount())
	{
	case 0:
		{
		}
		break;
	case 1:
		{
			CLBS_Variable*	pPara1	=(CLBS_Variable*)paraList.GetObj(0);
			b32Float	=(BOOL)*pPara1;
		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
		break;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			*pRet	=(DOUBLE)atof((LPCSTR)*pVar);
		}
		break;
	case LVT_INTEGER:
		{
			*pRet	=(DOUBLE)(LONG)*pVar;
		}
		break;
	case LVT_FLOAT:
		{
			*pRet	=*pVar;
		}
		break;
	case LVT_BINARY_DATA:
		{
			*pRet =0.0f;
			DWORD	size =pVar->GetDataSize();
			if(size < 5)
			{
				FLOAT value =0.0f;
				memcpy(&value,pVar->GetDataBuffer(),min(sizeof(FLOAT),size));
				*pRet	=value;
			}
			else
			{
				if(b32Float)
				{
					DWORD count		=size/sizeof(FLOAT);
					if(count > 0)
					{
						pRet->Expand(count);
						DOUBLE*		pData	=(DOUBLE*)pRet->GetDataBuffer();
						FLOAT*		pOrg	=(FLOAT*)pVar->GetDataBuffer();
						for(DWORD i=0;i<count;i++)
							pData[i]=pOrg[i];
					}
				}
				else
				{
					DWORD count		=size/sizeof(DOUBLE);
					if(count > 0)
					{
						pRet->Expand(count);
						DOUBLE*		pData	=(DOUBLE*)pRet->GetDataBuffer();
						DOUBLE*		pOrg	=(DOUBLE*)pVar->GetDataBuffer();
						for(DWORD i=0;i<count;i++)
							pData[i]=pOrg[i];
					}
				}
			}
			
		}
		break;
	default:
		{
			*pRet	=(DOUBLE)0.0;
		}
		break;
	}
	return pRet;
}
//转换为二进制内存

CLBS_ObjBase*	CLBS_ApiImplement::var_toBinary(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	
	pRet->Set(pVar->GetDataBuffer(),pVar->GetDataSize(),LVT_BINARY_DATA);
	return pRet;

}

//转换为小写字串
CLBS_ObjBase*	CLBS_ApiImplement::var_toLowerCase(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			errorCode	=0;
			_strlwr((CHAR*)(LPCSTR)*pVar);
			pRet =new CLBS_Variable(NULL);
			*pRet	=*pVar;
		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		break;
	}
	return pRet;
}
//转换为大字字串
CLBS_ObjBase*	CLBS_ApiImplement::var_toUpperCase(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			_strupr((CHAR*)(LPCSTR)*pVar);
			pRet =new CLBS_Variable(NULL);
			*pRet	=*pVar;
		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		break;
	}
	return pRet;
}



CLBS_ObjBase*	CLBS_ApiImplement::var_trimSpace(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			CHAR*	pBuf =(CHAR*)LB_ALLOC(pVar->GetDataSize()+1);
			strcpy(pBuf,(LPCSTR)*pVar);
			TrimLeft(TrimRight(pBuf,' '),' ');
			*pVar =(LPCSTR)pBuf;
            LB_FREE((LPVoid&)pBuf);
			pRet =new CLBS_Variable(NULL);
			*pRet	=*pVar;
		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		break;
	}
	return pRet;
}
//得到子串char* subString(int iStart,int iLen);
CLBS_ObjBase*	CLBS_ApiImplement::var_subString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=2 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)paraList.GetObj(0);
	CLBS_Variable*	pPara2	=(CLBS_Variable*)paraList.GetObj(1);
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;

	if(pPara1->GetVarType() != LVT_INTEGER || pPara2->GetVarType() != LVT_INTEGER )
	{	
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	LONG	lStart	=(LONG)*pPara1;
	LONG	lLen	=(LONG)*pPara2;


	CLBS_Variable*	pRet	=NULL;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			pRet =new CLBS_Variable(NULL);
			if( (lStart) >= (LONG)pVar->GetMemberCount() || lStart<0 || lLen<=0)
			{
				pRet->Set(pVar->GetDataBuffer(),0,LVT_STRING);
			}
			else
			{
				if(lStart+lLen > (LONG)pVar->GetMemberCount())
					lLen	=(LONG)pVar->GetMemberCount() - lStart;

				pRet->Set((CHAR*)pVar->GetDataBuffer() + lStart,lLen,LVT_STRING);
			}

		}
		break;
	default:
		{
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		break;
	}
	return pRet;
}
//得到子串第一次出现的位置int indexOf(char* pStr);
CLBS_ObjBase*	CLBS_ApiImplement::var_indexOf(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}	
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;

	LONG			iStart	=0;
	CLBS_Variable*	pPara1;
	CLBS_Variable*	pPara2;
	switch(paraList.GetCount())
	{
	case 1:
		{
			pPara1	=(CLBS_Variable*)paraList.GetObj(0);
			if(pPara1->GetVarType() != LVT_STRING)
			{	
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
		}
		break;
	case 2:
		{
			pPara1	=(CLBS_Variable*)paraList.GetObj(0);
			if(pPara1->GetVarType() != LVT_STRING)
			{	
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			pPara2	=(CLBS_Variable*)paraList.GetObj(1);
			if(pPara2->GetVarType() != LVT_INTEGER)
			{	
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			iStart =(LONG)*pPara2;
		}
		break;
	default:
		{
			errorCode=LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
	}


	CLBS_Variable*	pRet	=NULL;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			LPCSTR		pString		=(LPCSTR)*pVar;
			LPCSTR		pPos		=strstr(pString+iStart,(LPCSTR)*pPara1);
			LONG		index		=-1;
			if(pPos != NULL)
			{
				index	=(LONG)(pPos - pString);
			}
			pRet	=new CLBS_Variable(NULL);
			*pRet	=index;
		}
		break;
	default:
		{
			errorCode	= LBS_AEC_PARA_TYPE_ERROR;
		}
		break;
	}
	return pRet;
}
//得到子串最后一次出现的位置int lastIndexOf(char* pStr);
CLBS_ObjBase*	CLBS_ApiImplement::var_lastIndexOf(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=1 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)paraList.GetObj(0);
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;

	if(pPara1->GetVarType() != LVT_STRING)
	{	
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	switch(pVar->GetVarType())
	{
	case LVT_STRING:
		{
			LPCSTR		pString		=(LPCSTR)*pVar;
			LONG		index		=-1;
			LPCSTR		pstr		=pString;
			LPCSTR		pLastPos	=NULL;
			LPCSTR		pPos		=NULL;
			while(TRUE)
			{
				pPos		=strstr(pstr,(LPCSTR)*pPara1);
				if(pPos == NULL)
					break;
				pstr		=pPos+1;
				pLastPos	=pPos;
			}

			if(pLastPos != NULL)
			{
				index	=(LONG)(pLastPos - pString);
			}
			pRet	=new CLBS_Variable(NULL);
			*pRet	=index;
		}
		break;
	default:
		{
			errorCode	=LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		break;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::var_reverseBytes(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;

	pVar->ReverseByte();

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	*pRet =*pVar;
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::var_reverse(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=0 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	pVar->ReverseOrder();
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	*pRet =*pVar;

	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::var_removeAt(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	DWORD			remove_count	=1;
	CLBS_Variable*	pPara1;
	CLBS_Variable*	pPara2;
	CLBS_Variable*	pVar			=(CLBS_Variable*)pThis;
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	if(paraList.GetCount()==1 )
	{
		pPara1	=(CLBS_Variable*)paraList.GetObj(0);
		if(pPara1->GetVarType() != LVT_INTEGER)
		{	
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
	}
	else if(paraList.GetCount()==2)
	{
		pPara1	=(CLBS_Variable*)paraList.GetObj(0);
		pPara2	=(CLBS_Variable*)paraList.GetObj(1);
		if(pPara1->GetVarType() != LVT_INTEGER||
			pPara2->GetVarType() != LVT_INTEGER)
		{	
			errorCode =LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		remove_count =(LONG)*pPara2;
	}
	else
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	if(!pVar->RemoveAt((LONG)*pPara1,remove_count))
		*pRet =(LONG)0;
	else
		*pRet =(LONG)1;
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::var_getName(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	DWORD			remove_count	=1;
	CLBS_Variable*	pVar			=(CLBS_Variable*)pThis;
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet	=(LPCSTR)pVar->GetName();
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::var_getTypeName(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	DWORD			remove_count	=1;
	CLBS_Variable*	pVar			=(CLBS_Variable*)pThis;
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet	=(LPCSTR)pVar->GetTypeName();
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::var_getToken(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(paraList.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pString		=(CLBS_Variable*)pThis;
	CLBS_Variable*	pToken		=(CLBS_Variable*)paraList.GetObj(0);
	if(pString->GetVarType() != LVT_STRING ||
		pToken->GetVarType() != LVT_STRING)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CHAR*			pBuffer =(CHAR*)LB_ALLOC(pString->GetDataSize()+1);
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	pRet->Set(" ",0,LVT_VAR_ARRAY);

	CLBS_Variable*	pSubString;
	LPSTR			p		=(LPSTR)pString->GetDataBuffer();
	BYTE			token	=*(LPSTR)pToken->GetDataBuffer();
	DWORD			index	=0;
	DWORD			dwCount	=0;

	while(*p != 0)
	{
		index	=0;
		while(*p != 0 && *p != token)
		{
			pBuffer[index]=*p;
			index++;
			p++;
		}
		pBuffer[index]=0;
		//if(index > 0)
		{
			pSubString		=new CLBS_Variable(NULL);
			*pSubString		=(LPCSTR)pBuffer;
			pRet->SetSuffixValue(dwCount,pSubString);
			pSubString->Release();
			dwCount++;
		}
		while(*p != 0 && *p == token)
			p++;
	}
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::var_reSize(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(pThis == NULL)
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}	
	if(paraList.GetCount()!=1 )
	{
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis->GetType() != LOT_CODE_VARIABLE)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Variable*	pVar	=(CLBS_Variable*)pThis;
	if(pVar->GetVarType() != LVT_STRING && pVar->GetVarType() != LVT_BINARY_DATA&& pVar->GetVarType() != LVT_UNKNOWN)
	{
		errorCode	=LBS_AEC_NOT_SUPPORT_METHOD;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)paraList.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	if(!pVar->ReSize((LONG)*pPara))
	{
		errorCode =LBS_AEC_SYSTEM_CALL_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	*pRet =*pVar;
	return pRet;

}




CLBS_ObjBase*	CLBS_ApiImplement::math_sin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=sin(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=sin(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_cos(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=cos(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=cos(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_tan(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=tan(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=tan(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::math_asin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=asin(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=asin(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_acos(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=acos(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=acos(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_atan(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=atan(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=atan(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::math_sqrt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=sqrt(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=sqrt(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	errorCode =0;
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_exp(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=exp(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=exp(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_log(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=log(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=log(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_log10(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=log10(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=log10(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::math_floor(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=floor(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=floor(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_ceil(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_FLOAT)
	{
		DOUBLE	value	=(DOUBLE)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=ceil(value);
	}
	else if(pVar->GetVarType() == LVT_INTEGER)
	{
		DOUBLE	value	=(DOUBLE)(LONG)*pVar;
		pRet			=new CLBS_Variable(pVar->GetParent());
		*pRet			=ceil(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_fmod(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pVar1	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pVar2	=(CLBS_Variable*)para.GetObj(1);
	DOUBLE			v1,v2;

	if(pVar1->GetVarType() == LVT_FLOAT)
	{
		v1	=(DOUBLE)*pVar1;
	}
	else if(pVar1->GetVarType() == LVT_INTEGER)
	{
		v1	=(DOUBLE)(LONG)*pVar1;
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	if(pVar2->GetVarType() == LVT_FLOAT)
	{
		v2	=(DOUBLE)*pVar2;
	}
	else if(pVar2->GetVarType() == LVT_INTEGER)
	{
		v2	=(DOUBLE)(LONG)*pVar2;
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}


	CLBS_Variable*	pRet =new CLBS_Variable(pVar1->GetParent());
	*pRet =fmod(v1,v2);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_pow(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{//参数数量不正确
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pVar1	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pVar2	=(CLBS_Variable*)para.GetObj(1);
	DOUBLE			v1,v2;

	if(pVar1->GetVarType() == LVT_FLOAT)
	{
		v1	=(DOUBLE)*pVar1;
	}
	else if(pVar1->GetVarType() == LVT_INTEGER)
	{
		v1	=(DOUBLE)(LONG)*pVar1;
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	if(pVar2->GetVarType() == LVT_FLOAT)
	{
		v2	=(DOUBLE)*pVar2;
	}
	else if(pVar2->GetVarType() == LVT_INTEGER)
	{
		v2	=(DOUBLE)(LONG)*pVar2;
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}


	CLBS_Variable*	pRet =new CLBS_Variable(pVar1->GetParent());
	*pRet =pow(v1,v2);
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::math_rand(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	LONG			max_value=0;
	if(pVar->GetVarType() == LVT_INTEGER)
	{
		max_value		=(LONG)*pVar;
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}


	pRet =new CLBS_Variable(pVar->GetParent());

	static BOOL gm_rand_seed_set=FALSE;
	if(!gm_rand_seed_set)
	{
		srand(time(NULL));
		gm_rand_seed_set=TRUE;
	}

	*pRet =(LONG)(rand()%max_value);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::math_abs(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode =LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{//参数数量不正确
		errorCode =LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=NULL;
	CLBS_Variable*	pVar	=(CLBS_Variable*)para.GetObj(0);
	if(pVar->GetVarType() == LVT_INTEGER)
	{
		pRet			=new CLBS_Variable(pVar->GetParent());
		LONG	value	=(LONG)*pVar;
		*pRet			=abs(value);
	}
	else if(pVar->GetVarType() == LVT_FLOAT)
	{
		pRet			=new CLBS_Variable(pVar->GetParent());
		DOUBLE	value	=(DOUBLE)*pVar;
		*pRet			=fabs(value);
	}
	else
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::file_fopen(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*		pFile	= new CFile();
	if(!pFile->Open((LPCSTR)*pPara1,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone|CFile::modeNoTruncate|CFile::typeBinary))
	{
		errorCode	=LBS_AEC_SYSTEM_CALL_ERROR;
		delete pFile;
		return NULL;
	}
    pFile->SeekToBegin();
	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(LPVOID)pFile;
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fclose(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_POINTER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pPara1;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->Close();
	delete pFile;
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(LONG)0;

	if(para.GetCount() != 3)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pResult;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSizePara	=(CLBS_Variable*)para.GetObj(2);
	if(pFilePara->GetVarType() != LVT_POINTER ||
		pSizePara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pResult;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return pResult;
	}
	LONG	size		=(LONG)*pSizePara;
	CHAR*	pBuffer		=(CHAR*)LB_ALLOC(size+1);
	size	=pFile->Read(pBuffer,size);
	pBuffer[size]=0;
	pBufferPara->Set(pBuffer,size,LVT_BINARY_DATA);
	LB_FREE((LPVoid&)pBuffer);

	*pResult	=size;
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fwrite(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->Write(pBufferPara->GetDataBuffer(),pBufferPara->GetDataSize());
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_freadString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet	=(LONG)0;

	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}

	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pRet;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return pRet;
	}
    ULONGLONG   pos     = pFile->GetPosition();


    CHAR        tmp[128];
    BOOL        bEndLine       = FALSE;
    ULONGLONG   size = 0;
    ULONGLONG   cutSize = 0;
    ULONGLONG   i = 0;

    while (!bEndLine)
    {
        ULONGLONG readSize =pFile->Read(tmp, 128);
        if (readSize == 0)
            break;

        for (i = 0; i < readSize; i++)
        {
            if (tmp[i] == 0x0d)
            {
                cutSize = 1;

                CHAR ch = 0;
                if ((i + 1) < readSize)
                {
                    ch = tmp[i + 1];
                }
                else
                {
                    if (pFile->Read(&ch, 1) == 0)
                        ch = 0;
                }

                if (ch == 0x0a)
                {
                    cutSize +=1;
                    i       +=1;
                }                
                bEndLine = TRUE;
                break;
            }
            else if (tmp[i] == 0x0a)
            {
                bEndLine = TRUE;
                cutSize = 1;
                break;
            }
        }
        size += (i+1);
    }
    pFile->Seek(pos, CFile::begin);

    CHAR* pBuffer = (CHAR*)LB_ALLOC(size+1);
    size=pFile->Read(pBuffer, size);
    pBuffer[size-cutSize] = 0;


    (*pBufferPara)    = (LPCSTR)pBuffer;
    (*pRet)           = (LONG)size;
	
    LB_FREE((LPVoid&)pBuffer);

	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fwriteString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	if(pFilePara->GetVarType() != LVT_POINTER ||
		pBufferPara->GetVarType() != LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
    LPCSTR   pString = (LPCSTR)*pBufferPara;
    UINT    size    = strlen(pString);

    pFile->Write(pString, size);

	return NULL;

}
CLBS_ObjBase*	CLBS_ApiImplement::file_fseek(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	if(pFilePara->GetVarType() != LVT_POINTER ||
		pBufferPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->Seek((LONG)*pBufferPara,CFile::begin);

	return NULL;

}
CLBS_ObjBase*	CLBS_ApiImplement::file_fseekToBegin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->SeekToBegin();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fseekToEnd(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->SeekToEnd();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fgetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(LONG)0;

	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pResult;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pResult;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return pResult;
	}
    pFile->SeekToBegin();
	*pResult	=(LONG)pFile->GetLength();
    pFile->SeekToBegin();
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fsetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pBufferPara	=(CLBS_Variable*)para.GetObj(1);
	if(pFilePara->GetVarType() != LVT_POINTER ||
		pBufferPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pFile->SetLength((LONG)*pBufferPara);
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::file_fgetPosition(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	= (LONG)0;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pResult;
	}
	CLBS_Variable*	pFilePara	=(CLBS_Variable*)para.GetObj(0);
	if(pFilePara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pResult;
	}
	CFile*	pFile	=(CFile*)(LPVOID)*pFilePara;
	if(pFile == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return pResult;
	}
	*pResult	=(LONG)pFile->GetPosition();
	
	return pResult;
}

CLBS_ObjBase*	CLBS_ApiImplement::explorer_createDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CreateDirectoryTree((LPCSTR)*pPara1);
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_setCurrentDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LONG)SetCurrentDirectory((LPCSTR)*pPara1);
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_getCurrentDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CHAR	dir[1024];
	ZeroMemory(dir,sizeof(dir));
	GetCurrentDirectory(sizeof(dir),dir);
	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LPCSTR)dir;
	return pResult;	
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_deleteDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LONG)RemoveDirectory((LPCSTR)*pPara1);
	return pResult;
}

CLBS_ObjBase*	CLBS_ApiImplement::explorer_deleteFile(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LONG)DeleteFile((LPCSTR)*pPara1);
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_listDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	pResult->Set(" ",0,LVT_VAR_ARRAY);

	CLBS_Variable*	pFileName;
	DWORD			dwCount	=0;
	if(SetCurrentDirectory((LPCSTR)*pPara1))
	{
		WIN32_FIND_DATA		fd;
		HANDLE	hFind	=FindFirstFile("*.*",&fd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			if(strcmp(fd.cFileName,".")!=0 && strcmp(fd.cFileName,"..")!=0)
			{
				pFileName=new CLBS_Variable(NULL);
				*pFileName =(LPCSTR)fd.cFileName;
				pResult->SetSuffixValue(dwCount,pFileName);
				dwCount++;
				pFileName->Release();
			}
			while(FindNextFile(hFind,&fd))
			{
				if(strcmp(fd.cFileName,".")!=0 && strcmp(fd.cFileName,"..")!=0)
				{
					pFileName=new CLBS_Variable(NULL);
					*pFileName =(LPCSTR)fd.cFileName;
					pResult->SetSuffixValue(dwCount,pFileName);
					dwCount++;
					pFileName->Release();
				}
			}
			FindClose(hFind);
		}
	}
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_isFile(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LONG)FALSE;

	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pResult;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pResult;
	}

	DWORD dwAttribute =GetFileAttributes((LPCSTR)*pPara1);
	if( INVALID_FILE_ATTRIBUTES == dwAttribute)
	{
		*pResult	=(LONG)0;
	}
	else
	{
		if( (dwAttribute&FILE_ATTRIBUTE_DIRECTORY) != 0)
			*pResult	=(LONG)0;
		else
			*pResult	=(LONG)1;
	}
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_isDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LONG)0;

	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pResult;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pResult;
	}
	DWORD dwAttribute =GetFileAttributes((LPCSTR)*pPara1);
	if( INVALID_FILE_ATTRIBUTES == dwAttribute)
	{
		*pResult	=(LONG)0;
	}
	else
	{
		if( (dwAttribute&FILE_ATTRIBUTE_DIRECTORY) != 0)
			*pResult	=(LONG)1;
		else
			*pResult	=(LONG)0;
	}
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::explorer_fileIsExist(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	DWORD dwAttribute =GetFileAttributes((LPCSTR)*pPara1);
	if( INVALID_FILE_ATTRIBUTES == dwAttribute)
	{
		*pResult	=(LONG)0;
	}
	else
	{
		*pResult	=(LONG)1;
	}
	return pResult;
}


CLBS_ObjBase*	CLBS_ApiImplement::script_getScriptRootDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();

	LPCSTR			pRootDir	=(LPCSTR)pParser->GetScriptRootDirectory();

	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=pRootDir;
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::script_getScriptLoaderAppDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pResult =new CLBS_Variable(NULL);
	*pResult	=(LPCSTR)gm_pszSystemDir;
	return pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::script_createTimer(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_CodeBase*	pArea		=(CLBS_CodeBase*)pThis;


	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pPara2	=(CLBS_Variable*)para.GetObj(1);
	if(pPara1->GetVarType() != LVT_STRING ||
		pPara2->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_Function*	pFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*pPara1);
	if(pFunc == NULL)
	{
		errorCode	=LBS_AEC_PARA_NOT_FOUND;
		return NULL;
	}


	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	UINT_PTR ID =pParser->SetTimer(pFunc,(LONG)*pPara2);
	if(ID == 0)
	{
		errorCode	=LBS_AEC_SYSTEM_CALL_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	*pRet	=(DWORD)ID;
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::script_killTimer(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara1	=(CLBS_Variable*)para.GetObj(0);
	if(pPara1->GetVarType() != LVT_INTEGER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	pParser->KillTimer((DWORD)*pPara1);

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::script_execute(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_CodeBase*	pArea		=(CLBS_CodeBase*)pThis;
	if(pArea == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_Variable*	pCode	=(CLBS_Variable*)para.GetObj(0);
	if(pCode->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(pCode->GetParent());
	*pRet =(LONG)0;

	CLBS_CodeBase*	pOwner =new CLBS_CodeBase(pArea);
	pOwner->SetScriptFile(pCode->GetScriptFile());
	if(pOwner->GetScriptFile()->ParseScript(pOwner,(LPCSTR)*pCode))
	{
		if(pOwner->Execute())
		{
			*pRet =(LONG)1;
		}
	}
	pOwner->Release();

	return pRet;
}


//Ini 配置文件读写操作
//char* getIniString(char* iniFileName,char* mainKeyName,char* subKeyName,char* defaultString);
CLBS_ObjBase*	CLBS_ApiImplement::ini_getIniString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pDefault	=(CLBS_Variable*)para.GetObj(3);
	if(pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType() != LVT_STRING ||
		pSubKey->GetVarType()  != LVT_STRING ||
		pDefault->GetVarType() != LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CHAR		resultString[2048];
	DWORD		len					=GetPrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,(LPCSTR)*pDefault,resultString,sizeof(resultString),(LPCSTR)*pFileName);
	resultString[len]=0;

	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(LPCSTR)resultString;

	return  pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::ini_getIniInteger(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pDefault	=(CLBS_Variable*)para.GetObj(3);
	if(pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType() != LVT_STRING ||
		pSubKey->GetVarType()  != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	if(pDefault->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CHAR		defaultString[128];
	CHAR		resultString[128];
	sprintf(defaultString,"%d",(LONG)*pDefault);
	DWORD		len					=GetPrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,defaultString,resultString,sizeof(resultString),(LPCSTR)*pFileName);
	resultString[len]=0;

	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(LONG)atoll(resultString);

	return  pResult;
}
CLBS_ObjBase*	CLBS_ApiImplement::ini_getIniFloat(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pDefault	=(CLBS_Variable*)para.GetObj(3);
	if(pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType() != LVT_STRING ||
		pSubKey->GetVarType()  != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CHAR		defaultString[128];
	CHAR		resultString[128];
	if(pDefault->GetVarType() == LVT_FLOAT)
	{
		sprintf(defaultString,"%f",(DOUBLE)*pDefault);
	}
	else if(pDefault->GetVarType() == LVT_INTEGER)
	{
		sprintf(defaultString,"%d",(LONG)*pDefault);
	}
	else
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	DWORD		len					=GetPrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,defaultString,resultString,sizeof(resultString),(LPCSTR)*pFileName);
	resultString[len]=0;
	CLBS_Variable*	pResult		=new CLBS_Variable(NULL);
	*pResult	=(DOUBLE)atof(resultString);
	return  pResult;
}

CLBS_ObjBase*	CLBS_ApiImplement::ini_setIniString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pValue		=(CLBS_Variable*)para.GetObj(3);
	if(	pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType()	!= LVT_STRING ||
		pSubKey->GetVarType()	!= LVT_STRING ||
		pValue->GetVarType()	!= LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	WritePrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,(LPCSTR)*pValue,(LPCSTR)*pFileName);
	return  NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::ini_setIniInteger(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pValue		=(CLBS_Variable*)para.GetObj(3);
	if(	pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType()	!= LVT_STRING ||
		pSubKey->GetVarType()	!= LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CHAR		buf[128];
	if(pValue->GetVarType() == LVT_INTEGER)
	{
		sprintf(buf,"%d",(LONG)*pValue);
	}
	else if(pValue->GetVarType() == LVT_FLOAT)
	{
		sprintf(buf,"%.0f",(DOUBLE)*pValue);
	}
	else
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}


	WritePrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,(LPCSTR)buf,(LPCSTR)*pFileName);
	return  NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::ini_setIniFloat(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pFileName	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pMainKey	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSubKey		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pValue		=(CLBS_Variable*)para.GetObj(3);
	if(	pFileName->GetVarType() != LVT_STRING ||
		pMainKey->GetVarType()	!= LVT_STRING ||
		pSubKey->GetVarType()	!= LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CHAR		buf[128];
	if(pValue->GetVarType() == LVT_INTEGER)
	{
		sprintf(buf,"%f",(DOUBLE)(LONG)*pValue);
	}
	else if(pValue->GetVarType() == LVT_FLOAT)
	{
		sprintf(buf,"%f",(DOUBLE)*pValue);
	}
	else
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	WritePrivateProfileString((LPCSTR)*pMainKey,(LPCSTR)*pSubKey,(LPCSTR)buf,(LPCSTR)*pFileName);
	return  NULL;
}

CLBS_ObjBase*	CLBS_ApiImplement::system_getDate(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 3)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*			pYear	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*			pMonth	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*			pDay	=(CLBS_Variable*)para.GetObj(2);

	CTime	curTime		=CTime::GetCurrentTime();

	*pYear	=(LONG)curTime.GetYear();
	*pMonth	=(LONG)curTime.GetMonth();
	*pDay	=(LONG)curTime.GetDay();

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getTime(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 3)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*			pHour	=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*			pMinute	=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*			pSecond	=(CLBS_Variable*)para.GetObj(2);

	CTime	curTime		=CTime::GetCurrentTime();

	*pHour		=(LONG)curTime.GetHour();
	*pMinute	=(LONG)curTime.GetMinute();
	*pSecond	=(LONG)curTime.GetSecond();

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getTickCount(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet	=(LONG)GetTickCount();
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getCpuTime(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);

	LARGE_INTEGER	litmp; 
	LONGLONG		QPart,dfFreq;
	DOUBLE			dfTim; 
	QueryPerformanceFrequency(&litmp);
	dfFreq = (DOUBLE)litmp.QuadPart;// 获得时钟频率

	QueryPerformanceCounter(&litmp);
	QPart = litmp.QuadPart;
	
	dfTim = ((DOUBLE)QPart / (DOUBLE)dfFreq)*1000.0;// 毫秒

	*pRet =dfTim;

	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::system_trace(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pStr	=(CLBS_Variable*)para.GetObj(0);
	if(pStr->GetVarType() != LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	DWORD		size =pStr->GetDataSize();
	CHAR*		pBuf =(CHAR*)LB_ALLOC(size+32);
	_snprintf(pBuf,size+31,"[LBS]: %s",(LPCSTR)*pStr);
	OutputDebugString(pBuf);
    LB_FREE((LPVoid&)pBuf);


	return NULL;

}
CLBS_ObjBase*	CLBS_ApiImplement::system_shellExecute(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;


	CLBS_Variable	*p1,*p2,*p3,*p4;

	LPCSTR			pOper		=NULL;
	LPCSTR			pFileName	=NULL;
	LPCSTR			pCmdLine	=NULL;
	LPCSTR			pDirectory	=NULL;
	switch(para.GetCount())
	{
	case 2:
		{
			p1=(CLBS_Variable*)para.GetObj(0);
			p2=(CLBS_Variable*)para.GetObj(1);
			if(p1->GetVarType() != LVT_STRING||
				p2->GetVarType() != LVT_STRING)
			{
				errorCode	=LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			pOper		=(LPCSTR)*p1;
			pFileName	=(LPCSTR)*p2;
		}
		break;
	case 3:
		{
			p1=(CLBS_Variable*)para.GetObj(0);
			p2=(CLBS_Variable*)para.GetObj(1);
			p3=(CLBS_Variable*)para.GetObj(2);
			if(p1->GetVarType() != LVT_STRING||
				p2->GetVarType() != LVT_STRING||
				p3->GetVarType() != LVT_STRING)
			{
				errorCode	=LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			pOper		=(LPCSTR)*p1;
			pFileName	=(LPCSTR)*p2;
			pCmdLine	=(LPCSTR)*p3;
		}
		break;
	case 4:
		{
			p1=(CLBS_Variable*)para.GetObj(0);
			p2=(CLBS_Variable*)para.GetObj(1);
			p3=(CLBS_Variable*)para.GetObj(2);
			p4=(CLBS_Variable*)para.GetObj(3);
			if(p1->GetVarType() != LVT_STRING||
				p2->GetVarType() != LVT_STRING||
				p3->GetVarType() != LVT_STRING||
				p3->GetVarType() != LVT_STRING)
			{
				errorCode	=LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			pOper		=(LPCSTR)*p1;
			pFileName	=(LPCSTR)*p2;
			pCmdLine	=(LPCSTR)*p3;
			pDirectory	=(LPCSTR)*p4;
		}
		break;
	default:
		{
			errorCode	=LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
		break;
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	if(ShellExecute(NULL,pOper,pFileName,pCmdLine,pDirectory,SW_SHOW) > (HINSTANCE)32)
	{
		*pRet =(LONG)1;
	}
	else
	{
		*pRet =(LONG)0;
	}

	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::system_executeDosCommand(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pStr	=(CLBS_Variable*)para.GetObj(0);
	if(pStr->GetVarType() != LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	STARTUPINFO				si;
	PROCESS_INFORMATION		pi;
	si.cb			=sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.dwFlags		=STARTF_USESHOWWINDOW;
	si.wShowWindow	=SW_HIDE;

	DWORD			size	=pStr->GetDataSize();
	CHAR*			pCmdLine=(CHAR*)LB_ALLOC(size+16);
	ZeroMemory(pCmdLine,size+16);
	_snprintf(pCmdLine,size+15,"cmd /c %s",(LPCSTR)*pStr);
	CLBS_Variable*	pRet	=new CLBS_Variable(pStr->GetParent());
	*pRet =(LONG)CreateProcess(NULL,(LPSTR)pCmdLine,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
    LB_FREE((LPVoid&)pCmdLine);

	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_sleep(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	Sleep((LONG)*pPara);
	return NULL;

}
CLBS_ObjBase*	CLBS_ApiImplement::system_openUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet =new CLBS_Variable();
	*pRet =(DWORD)0;
	
	CLBS_Variable*	ppCallback	=NULL;
	CLBS_Variable*	ppPort		=NULL;
	CLBS_Variable*	ppIp		=NULL;
	CLBS_CodeBase*	pArea		=(CLBS_CodeBase*)pThis;
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	switch(para.GetCount())
	{
	case 2:
		{
			ppCallback	=(CLBS_Variable*)para.GetObj(0);
			ppPort		=(CLBS_Variable*)para.GetObj(1);
			if(ppPort->GetVarType() != LVT_INTEGER ||
				ppCallback->GetVarType() != LVT_STRING)
			{
				errorCode	=LBS_AEC_PARA_TYPE_ERROR;
				return pRet;
			}
			CLBS_Function*	pFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppCallback);
			if(pFunc == NULL)
			{
				errorCode	=LBS_AEC_PARA_NOT_FOUND;
				return NULL;
			}
			*pRet =(DWORD)pParser->OpenUDP(pFunc,(DWORD)*ppPort,NULL);
		}
		break;
	case 3:
		{

			ppCallback	=(CLBS_Variable*)para.GetObj(0);
			ppPort		=(CLBS_Variable*)para.GetObj(1);
			ppIp		=(CLBS_Variable*)para.GetObj(2);
			if(ppPort->GetVarType() != LVT_INTEGER ||
				ppCallback->GetVarType() != LVT_STRING  ||
				ppIp->GetVarType() != LVT_STRING )
			{
				errorCode	=LBS_AEC_PARA_TYPE_ERROR;
				return pRet;
			}
			CLBS_Function*	pFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppCallback);
			if(pFunc == NULL)
			{
				errorCode	=LBS_AEC_PARA_NOT_FOUND;
				return NULL;
			}
			*pRet =(DWORD)pParser->OpenUDP(pFunc,(DWORD)*ppPort,(LPCSTR)*ppIp);
		}
		break;
	default:
		{
			errorCode	=LBS_AEC_PARA_COUNT_ERROR;
			return pRet;
		}
		break;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_sendToUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet =new CLBS_Variable();
	*pRet =(DWORD)0;

	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}
	CLBS_Variable*	ppID		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	ppIP		=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	ppPort		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	ppData		=(CLBS_Variable*)para.GetObj(3);
	if(ppID->GetVarType() != LVT_INTEGER ||
		ppIP->GetVarType() != LVT_STRING ||
		ppPort->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pRet;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();

	*pRet	 =(LONG)pParser->SendToUdp((DWORD)*ppID,ppData->GetDataBuffer(),ppData->GetDataSize(),(LPCSTR)*ppIP,(DWORD)*ppPort);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_closeUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	pParser->CloseUDP((DWORD)*pPara);
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getUDPPort(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Script_UDP*	pUDP =(CLBS_Script_UDP*)pParser->GetUDP((DWORD)*pPara);
	CLBS_Variable*		pRet =new CLBS_Variable();
	*pRet =(DWORD)0;
	if(pUDP != NULL)
		*pRet =(DWORD)pUDP->GetPort();
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::system_openTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet =new CLBS_Variable();
	*pRet =(DWORD)0;

	if(para.GetCount() != 5)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}
	CLBS_Variable*	ppIP			=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	ppPort			=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	ppAcceptFunc	=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	ppRecvFunc		=(CLBS_Variable*)para.GetObj(3);
	CLBS_Variable*	ppCloseFunc	=(CLBS_Variable*)para.GetObj(4);
	if(ppIP->GetVarType()			!= LVT_STRING ||
		ppPort->GetVarType()		!= LVT_INTEGER||
		ppAcceptFunc->GetVarType() != LVT_STRING ||
		ppRecvFunc->GetVarType()	!= LVT_STRING ||
		ppCloseFunc->GetVarType()	!= LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pRet;
	}
	CLBS_CodeBase*		pArea		=(CLBS_CodeBase*)pThis;
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Function*	pAcceptFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppAcceptFunc);
	CLBS_Function*	pRecvFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppRecvFunc);
	CLBS_Function*	pCloseFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppCloseFunc);
	if(pAcceptFunc == NULL || pRecvFunc==NULL || pCloseFunc== NULL)
	{
		errorCode	=LBS_AEC_PARA_NOT_FOUND;
		return NULL;
	}
	*pRet =(DWORD)pParser->OpenTCP((LPCSTR)*ppIP,(DWORD)*ppPort,pAcceptFunc,pRecvFunc,pCloseFunc);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_connectTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet =new CLBS_Variable();
	*pRet =(DWORD)0;

	if(para.GetCount() != 5)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}
	CLBS_Variable*	ppIP			=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	ppPort			=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	ppAcceptFunc	=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	ppRecvFunc		=(CLBS_Variable*)para.GetObj(3);
	CLBS_Variable*	ppCloseFunc	=(CLBS_Variable*)para.GetObj(4);
	if(ppIP->GetVarType()			!= LVT_STRING ||
		ppPort->GetVarType()		!= LVT_INTEGER||
		ppAcceptFunc->GetVarType() != LVT_STRING ||
		ppRecvFunc->GetVarType()	!= LVT_STRING ||
		ppCloseFunc->GetVarType()	!= LVT_STRING)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return pRet;
	}
	CLBS_CodeBase*		pArea		=(CLBS_CodeBase*)pThis;
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Function*	pAcceptFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppAcceptFunc);
	CLBS_Function*	pRecvFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppRecvFunc);
	CLBS_Function*	pCloseFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*ppCloseFunc);
	if(pAcceptFunc == NULL || pRecvFunc==NULL || pCloseFunc== NULL)
	{
		errorCode	=LBS_AEC_PARA_NOT_FOUND;
		return NULL;
	}

	*pRet =(DWORD)pParser->ConnectTCP((LPCSTR)*ppIP,(DWORD)*ppPort,pAcceptFunc,pRecvFunc,pCloseFunc);
	return pRet;

}
CLBS_ObjBase*	CLBS_ApiImplement::system_sendToTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;

	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	ppID		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	ppData		=(CLBS_Variable*)para.GetObj(1);
	if(ppID->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	pParser->SendToTCP((DWORD)*ppID,ppData->GetDataBuffer(),ppData->GetDataSize());
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_closeTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();
	
	pParser->CloseTCP((DWORD)*pPara);

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getTCPIP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Script_TCP*	pTcp =(CLBS_Script_TCP*)pParser->GetTCP((DWORD)*pPara);
	CLBS_Variable*		pRet =new CLBS_Variable();
	*pRet ="";
	if(pTcp != NULL)
		*pRet =pTcp->GetIpaddress();
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getTCPPort(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*			pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_INTEGER)
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_SyntaxParser* pParser=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Script_TCP*	pTcp =(CLBS_Script_TCP*)pParser->GetTCP((DWORD)*pPara);
	CLBS_Variable*		pRet =new CLBS_Variable();
	*pRet =(DWORD)0;
	if(pTcp != NULL)
		*pRet =(DWORD)pTcp->GetPort();
	return pRet;
}











CLBS_ObjBase*	CLBS_ApiImplement::system_getLocalHostDefaultIp(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if( para.GetCount() != 0 )
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	char		szHostname[255];
	HOSTENT*	pHostEnt	= NULL;
	if (gethostname(szHostname, sizeof(szHostname)))
		return NULL;
	pHostEnt = gethostbyname(szHostname);
	if (pHostEnt == NULL)
		return NULL;
	if (pHostEnt->h_length != 4)
		return NULL;

	CString ip;
	if( pHostEnt->h_addr_list[0] )
	{
		in_addr address;
		CopyMemory(&address.S_un.S_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
		ip.Format("%d.%d.%d.%d",  
			address.S_un.S_un_b.s_b1,
			address.S_un.S_un_b.s_b2,
			address.S_un.S_un_b.s_b3,
			address.S_un.S_un_b.s_b4);

		CLBS_Variable*	pRet =new CLBS_Variable();
		*pRet =ip;
		return pRet;
	}
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_getLocalHostIpArray(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	pRet->Set(" ",0,LVT_VAR_ARRAY);
	CLBS_Variable*	pSubString;


	if( para.GetCount() != 0 )
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}
	char		szHostname[255];
	HOSTENT*	pHostEnt	= NULL;
	if (gethostname(szHostname, sizeof(szHostname)))
		return pRet;
	pHostEnt = gethostbyname(szHostname);
	if (pHostEnt == NULL)
		return pRet;
	if (pHostEnt->h_length != 4)
		return pRet;

	CString ip;
	int iIndex = 0;
	while( pHostEnt->h_addr_list[iIndex] )
	{
		in_addr address;
		CopyMemory(&address.S_un.S_addr, pHostEnt->h_addr_list[iIndex], pHostEnt->h_length);
		ip.Format("%d.%d.%d.%d",  
			address.S_un.S_un_b.s_b1,
			address.S_un.S_un_b.s_b2,
			address.S_un.S_un_b.s_b3,
			address.S_un.S_un_b.s_b4);
		
		pSubString			=new CLBS_Variable(NULL);
		*pSubString		=(LPCSTR)ip;
		pRet->SetSuffixValue(iIndex,pSubString);
		pSubString->Release();

		iIndex++;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::system_raisePrivileges(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	*pRet =(DWORD)FALSE;
	if( para.GetCount() != 0 )
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return pRet;
	}

	HANDLE	hToken	=NULL;
	TOKEN_PRIVILEGES		tkp;
	tkp.PrivilegeCount		=1;
	tkp.Privileges[0].Attributes	=SE_PRIVILEGE_ENABLED;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken))
	{
		if(LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid))
		{
			*pRet =(DWORD)AdjustTokenPrivileges(hToken,FALSE,&tkp,0X10,(PTOKEN_PRIVILEGES)NULL,0);
		}
	}
	if(hToken != NULL)
		CloseHandle(hToken);
	return pRet;
}


//数据结构
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayCreate(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	
	*pRet =(LPVOID)new CLBS_ObjListBase();
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayAdd(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 3)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray =(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pPara2 =(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pName =(CLBS_Variable*)para.GetObj(2);
	if(pArray->GetVarType() != LVT_POINTER||
		pName->GetVarType() != LVT_STRING)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}



	CLBS_Variable*		pElement	=new CLBS_Variable(NULL);
	*pElement	=*pPara2;
	pElement->SetName((LPCSTR)*pName);
	pList->AddObj(pElement);
	pElement->Release();

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayInsert(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pIndex		=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pPara		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pName		=(CLBS_Variable*)para.GetObj(3);
	if(pArray->GetVarType() != LVT_POINTER ||
		pIndex->GetVarType() != LVT_INTEGER||
		pName->GetVarType() != LVT_STRING)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}

	CLBS_Variable*		pElement	=new CLBS_Variable(NULL);
	*pElement	=*pPara;
	pElement->SetName((LPCSTR)*pName);
	BOOL	bRet =pList->InsertObj((LONG)*pIndex,pElement);
	pElement->Release();

	CLBS_Variable*		pRet =new CLBS_Variable(NULL);
	*pRet =(LONG)bRet;
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayGetAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pIndex		=(CLBS_Variable*)para.GetObj(1);
	if(pArray->GetVarType() != LVT_POINTER ||
		pIndex->GetVarType() != LVT_INTEGER)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_ObjBase*		pRet		=pList->GetObj((LONG)*pIndex);

	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arraySetAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 4)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pIndex		=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pPara		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pName		=(CLBS_Variable*)para.GetObj(3);
	if(pArray->GetVarType() != LVT_POINTER ||
		pIndex->GetVarType() != LVT_INTEGER||
		pName->GetVarType() != LVT_STRING)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}

	CLBS_Variable*		pElement	=new CLBS_Variable(NULL);
	*pElement	=*pPara;
	pElement->SetName((LPCSTR)*pName);
	BOOL	bRet =pList->SetAt((LONG)*pIndex,pElement);
	pElement->Release();

	CLBS_Variable*		pRet =new CLBS_Variable(NULL);
	*pRet =(LONG)bRet;
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayRemoveAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pIndex		=(CLBS_Variable*)para.GetObj(1);
	if(pArray->GetVarType() != LVT_POINTER ||
		pIndex->GetVarType() != LVT_INTEGER)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_Variable*		pRet		=new CLBS_Variable(NULL);
	
	*pRet =(LONG)pList->DelObj((LONG)*pIndex);

	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayGetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	if(pArray->GetVarType() != LVT_POINTER )
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_Variable*		pRet		=new CLBS_Variable(NULL);

	*pRet =(LONG)pList->GetCount();

	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayClear(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	if(pArray->GetVarType() != LVT_POINTER )
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pList->Clear();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayDestroy(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	CLBS_Variable*	pArray		=(CLBS_Variable*)para.GetObj(0);
	if(pArray->GetVarType() != LVT_POINTER )
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}
	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	LBS_SafeRelease(pList);
	return NULL;
}

CLBS_ObjBase*	CLBS_ApiImplement::vector_arrayFind(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	CLBS_Variable*	pArray		=NULL;
	CLBS_Variable*	pName		=NULL;
	CLBS_Variable*	pStart		=NULL;
	LONG			lStart		=0;
	switch(para.GetCount())
	{
	case 2:
		{
			pArray		=(CLBS_Variable*)para.GetObj(0);
			pName		=(CLBS_Variable*)para.GetObj(1);
			if(pArray->GetVarType() != LVT_POINTER ||
				pName->GetVarType() != LVT_STRING)
			{
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
		}
		break;
	case 3:
		{
			pArray		=(CLBS_Variable*)para.GetObj(0);
			pName		=(CLBS_Variable*)para.GetObj(1);
			pStart		=(CLBS_Variable*)para.GetObj(2);
			if(pArray->GetVarType() != LVT_POINTER ||
				pName->GetVarType() != LVT_STRING ||
				pStart->GetVarType() != LVT_INTEGER)
			{
				errorCode =LBS_AEC_PARA_TYPE_ERROR;
				return NULL;
			}
			lStart		=(LONG)*pStart;
		}
		break;
	default:
		{
			errorCode	=LBS_AEC_PARA_COUNT_ERROR;
			return NULL;
		}
		break;
	}


	CLBS_ObjListBase*	pList		=(CLBS_ObjListBase*)(LPVOID)*pArray;
	if(pList == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}

	LONG	index	=-1;
	for(LONG i=lStart;i<pList->GetCount();i++)
	{
		if(strcmp(pList->GetObj(i)->GetName(),(LPCSTR)*pName) == 0)
		{
			index =i;
			break;
		}
	}

	CLBS_Variable*	pRet =new CLBS_Variable(NULL);
	*pRet =(LONG)index;
	return pRet;
}

CLBS_ObjBase*	CLBS_ApiImplement::string_strGetToken(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 2)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pString		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pToken		=(CLBS_Variable*)para.GetObj(1);
	if(pString->GetVarType() != LVT_STRING ||
		pToken->GetVarType() != LVT_STRING)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CHAR*			pBuffer =(CHAR*)LB_ALLOC(pString->GetDataSize()+1);
	CLBS_Variable*	pRet	=new CLBS_Variable(NULL);
	pRet->Set(" ",0,LVT_VAR_ARRAY);

	CLBS_Variable*	pSubString;
	LPSTR			p		=(LPSTR)pString->GetDataBuffer();
	BYTE			token	=*(LPSTR)pToken->GetDataBuffer();
	DWORD			index	=0;
	DWORD			dwCount	=0;
	while(*p != 0)
	{
		index	=0;
		while(*p != 0 && *p != token)
		{
			pBuffer[index]=*p;
			index++;
			p++;
		}
		pBuffer[index]=0;
		{
			pSubString		=new CLBS_Variable(NULL);
			*pSubString		=(LPCSTR)pBuffer;
			pRet->SetSuffixValue(dwCount,pSubString);
			pSubString->Release();
			dwCount++;
		}
		while(*p != 0 && *p == token)
			p++;
	}
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::memory_memCopy(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 3)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pDst		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pSrc		=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pCount		=(CLBS_Variable*)para.GetObj(2);

	if(	(pDst->GetVarType()!=LVT_INTEGER &&pDst->GetVarType()!=LVT_FLOAT &&pDst->GetVarType()!=LVT_STRING &&pDst->GetVarType()!=LVT_BINARY_DATA)||
		(pSrc->GetVarType()!=LVT_INTEGER &&pSrc->GetVarType()!=LVT_FLOAT &&pSrc->GetVarType()!=LVT_STRING &&pSrc->GetVarType()!=LVT_BINARY_DATA)||
		pCount->GetVarType()!=LVT_INTEGER)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	LONG	size	=(LONG)*pCount;
	if(size > pDst->GetDataSize() || size > pDst->GetDataSize() )
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}
	
	memcpy(pDst->GetDataBuffer(),pSrc->GetDataBuffer(),size);

	return NULL;
}

CLBS_ObjBase*	CLBS_ApiImplement::memory_memCopyEx(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 5)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pDst		=(CLBS_Variable*)para.GetObj(0);
	CLBS_Variable*	pDstPos		=(CLBS_Variable*)para.GetObj(1);
	CLBS_Variable*	pSrc		=(CLBS_Variable*)para.GetObj(2);
	CLBS_Variable*	pSrcPos		=(CLBS_Variable*)para.GetObj(3);
	CLBS_Variable*	pCount		=(CLBS_Variable*)para.GetObj(4);

	if(	(pDst->GetVarType()!=LVT_INTEGER &&pDst->GetVarType()!=LVT_FLOAT &&pDst->GetVarType()!=LVT_STRING &&pDst->GetVarType()!=LVT_BINARY_DATA)||
		(pSrc->GetVarType()!=LVT_INTEGER &&pSrc->GetVarType()!=LVT_FLOAT &&pSrc->GetVarType()!=LVT_STRING &&pSrc->GetVarType()!=LVT_BINARY_DATA)||
		pDstPos->GetVarType()!=LVT_INTEGER||
		pSrcPos->GetVarType()!=LVT_INTEGER||
		pCount->GetVarType()!=LVT_INTEGER
		)
	{
		errorCode =LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	LONG	index1	=(LONG)*pDstPos;
	LONG	index2	=(LONG)*pSrcPos;
	LONG	size	=(LONG)*pCount;
	if( (size+index1) > pDst->GetDataSize() || (size+index2) > pSrc->GetDataSize() )
	{
		errorCode =LBS_AEC_PARA_INVALID;
		return NULL;
	}

	memcpy((BYTE*)pDst->GetDataBuffer() + index1 , (BYTE*)pSrc->GetDataBuffer()+index2 , size);

	return NULL;
}

CLBS_ObjBase*	CLBS_ApiImplement::thread_createThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}

	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_CodeBase*		pArea		=(CLBS_CodeBase*)pThis;
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();



	CLBS_Variable*		pFuncName	=(CLBS_Variable*)para.GetObj(0);
	if(pFuncName->GetVarType() != LVT_STRING )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Function*	pFunc	=(CLBS_Function*)pArea->GetFunction((LPCSTR)*pFuncName);
	if(pFunc == NULL)
	{
		errorCode	=LBS_AEC_PARA_NOT_FOUND;
		return NULL;
	}

	CLBS_ScriptThread*	pThread		=(CLBS_ScriptThread*)pParser->CreateNewThread(pFunc);
	CLBS_Variable*		pRet		=new CLBS_Variable(NULL);
	pRet->Set(&pThread,sizeof(CLBS_ScriptThread*),LVT_POINTER);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_terminateThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();


	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ScriptThread*	pThread =(CLBS_ScriptThread*)(LPVOID)*pPara;
	if(pThread == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pParser->DestroyThread(pThread);

	pPara->Reset();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_suspendThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ScriptThread*	pThread =(CLBS_ScriptThread*)(LPVOID)*pPara;
	if(pThread == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pThread->Suspend();

	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_resumeThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_ScriptThread*	pThread =(CLBS_ScriptThread*)(LPVOID)*pPara;
	if(pThread == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pThread->Resume();

	return NULL;
}

CLBS_ObjBase*	CLBS_ApiImplement::thread_createLock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();

	CLBS_Script_Lock*	pLock	=(CLBS_Script_Lock*)pParser->CreateNewLock();
	CLBS_Variable*		pRet	=new CLBS_Variable(NULL);
	pRet->Set(&pLock,sizeof(CLBS_Script_Lock*),LVT_POINTER);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_lock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Lock*	pLock=(CLBS_Script_Lock*)(LPVOID)*pPara;
	if(pLock == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pLock->Lock();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_unlock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Lock*	pLock=(CLBS_Script_Lock*)(LPVOID)*pPara;
	if(pLock == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pLock->Unlock();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_destroyLock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();


	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Lock*	pLock=(CLBS_Script_Lock*)(LPVOID)*pPara;
	if(pLock == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pParser->DestroyLock(pLock);	

	pPara->Reset();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_createEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 0)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_CodeBase*		pArea		=(CLBS_CodeBase*)pThis;
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();


	CLBS_Script_Event*	pEvent	=(CLBS_Script_Event*)pParser->CreateNewEvent();
	CLBS_Variable*		pRet	=new CLBS_Variable(NULL);
	pRet->Set(&pEvent,sizeof(CLBS_Script_Event*),LVT_POINTER);
	return pRet;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_setEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Event*	pEvent=(CLBS_Script_Event*)(LPVOID)*pPara;
	if(pEvent == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pEvent->setEvent();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_waitEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Event*	pEvent=(CLBS_Script_Event*)(LPVOID)*pPara;
	if(pEvent == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pEvent->waitEvent();
	return NULL;
}
CLBS_ObjBase*	CLBS_ApiImplement::thread_destroyEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(para.GetCount() != 1)
	{
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	if(pThis == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	CLBS_SyntaxParser* pParser		=((CLBS_CodeBase*)pThis)->GetParser();



	CLBS_Variable*	pPara	=(CLBS_Variable*)para.GetObj(0);
	if(pPara->GetVarType() != LVT_POINTER )
	{
		errorCode	=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Script_Event*	pEvent=(CLBS_Script_Event*)(LPVOID)*pPara;
	if(pEvent == NULL)
	{
		errorCode	=LBS_AEC_PARA_INVALID;
		return NULL;
	}
	pParser->DestroyEvent(pEvent);

	pPara->Reset();
	return NULL;
}

