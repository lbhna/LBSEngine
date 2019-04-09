#include "StdAfx.h"
#include ".\lbs_scriptdriver.h"
#include "./LBS_Function.h"
#include "./LBS_Script_FunctionMap.h"
#include "../ErrorSystem.h"

CLBS_ScriptDriver*	CLBS_ScriptDriver::m_pSingleton =NULL;
CLBS_ScriptDriver::CLBS_ScriptDriver(void)
{
	m_Type					=LOT_SCRIPT_DRIVER;
	m_pScriptParser			=new CLBS_SyntaxParser(NULL,this);
	CErrorSystem::GetInstance();

	/////////////////////////////////
	OnInitialize();
}

CLBS_ScriptDriver::~CLBS_ScriptDriver(void)
{
	LBS_SafeRelease(m_pScriptParser);
	CErrorSystem::ExitInstance();
}
CLBS_ScriptDriver&	CLBS_ScriptDriver::GetInstance()
{
	if(CLBS_ScriptDriver::m_pSingleton == NULL)
	{
		CLBS_ScriptDriver::m_pSingleton =new CLBS_ScriptDriver();
	}
	return *CLBS_ScriptDriver::m_pSingleton;
}
VOID				CLBS_ScriptDriver::ExitInstance()
{
	LBS_SafeDelete(CLBS_ScriptDriver::m_pSingleton);
}
BOOL				CLBS_ScriptDriver::AddSystemFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr)
{
	return CLBS_Script_FunctionMap::GetInstance().AddApplicationApiFunction(pFuncName,pFuncAddr);
}
CLBS_ObjBase*		CLBS_ScriptDriver::GetFirstKernelFunction()
{
	return CLBS_Script_FunctionMap::GetInstance().GetFirstKernelFunction();
}
CLBS_ObjBase*		CLBS_ScriptDriver::GetNextKernelFunction()
{
	return CLBS_Script_FunctionMap::GetInstance().GetNextKernelFunction();
}
DWORD				CLBS_ScriptDriver::GetKernelFunctionCount()
{
	return CLBS_Script_FunctionMap::GetInstance().GetKernelFunctionCount();

}

BOOL				CLBS_ScriptDriver::LoadScript(LPCSTR IN pFileName)
{
	CErrorSystem::GetInstance().Clear();
	LBS_SafeRelease(m_pScriptParser);
	m_pScriptParser			=new CLBS_SyntaxParser(NULL,this);
	BOOL bRet =m_pScriptParser->LoadScript(pFileName);
	return bRet;
}
BOOL				CLBS_ScriptDriver::LoadScriptFromMemory(VOID* IN pMem,DWORD IN dwLen)
{
	CErrorSystem::GetInstance().Clear();
	LBS_SafeRelease(m_pScriptParser);
	m_pScriptParser			=new CLBS_SyntaxParser(NULL,this);
	BOOL bRet =m_pScriptParser->LoadScriptFromMemory(pMem,dwLen);
	return bRet;
}

BOOL				CLBS_ScriptDriver::Execute()
{
	return m_pScriptParser->Execute();
}

CLBS_Variable*		CLBS_ScriptDriver::CallScriptFunction(LPCSTR IN pFuncName,CLBS_Script_CallFunctionPara& IN Parameter,DWORD* OUT pErrorCode)
{
	if(m_pScriptParser == NULL)
	{
		if(pErrorCode != NULL)//未载入函数
			*pErrorCode		=LBS_AEC_SCRIPT_NOT_VALID;
		return NULL;
	}
	CLBS_Function*	pFunc	=(CLBS_Function*)m_pScriptParser->GetFunction(pFuncName);
	if(pFunc == NULL)
	{
		if(pErrorCode != NULL)//函数未定义
			*pErrorCode		=LBS_AEC_FUNCTION_NOT_DEFINATION;
		return NULL;
	}
	if(pFunc->GetParameterVariableCount() != Parameter.GetCount())
	{
		if(pErrorCode != NULL)//形参数量不正确
			*pErrorCode		=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}


	CLBS_Variable*	pVar	=NULL;
	CLBS_Variable*	pPara	=NULL;
	CLBS_Variable*	pRetVar =NULL;
	for(LONG i=0;i<pFunc->GetParameterVariableCount();i++)
	{
		pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(i);
		pPara	=(CLBS_Variable*)Parameter.GetObj(i);
		*pVar	=*pPara;
		pVar->SetRefVaraibleInfo(0,pPara);

	}
	pFunc->SetCallArea(m_pScriptParser);
	if(!pFunc->Execute())
	{
		if(pErrorCode != NULL)//函数执行失败
			*pErrorCode		=LBS_AEC_FAILURE;
		return NULL;
	}
	
	pRetVar	=(CLBS_Variable*)pFunc->GetReturnVariable();
	if(pRetVar != NULL)
		pRetVar->AddRef();

	if(pErrorCode != NULL)
		*pErrorCode		=LBS_AEC_SUCESS;

	return pRetVar;
}	

CLBS_Variable*		 CLBS_ScriptDriver::ExecuteScriptFunction(CLBS_CodeBase* IN pFunction,CLBS_Script_CallFunctionPara& IN Parameter,DWORD* OUT pErrorCode)
{
	if(m_pScriptParser == NULL || pFunction == NULL)
	{
		if(pErrorCode != NULL)//未载入函数
			*pErrorCode		=LBS_AEC_SCRIPT_NOT_VALID;
		return NULL;
	}
	if(pFunction->GetType() != LOT_CODE_FUNCTION)
	{
		if(pErrorCode != NULL)//形参数量不正确
			*pErrorCode		=LBS_AEC_PARA_TYPE_ERROR;
		return NULL;
	}

	CLBS_Function*	pFunc	=(CLBS_Function*)pFunction;
	if(pFunc->GetParameterVariableCount() != Parameter.GetCount())
	{
		if(pErrorCode != NULL)//形参数量不正确
			*pErrorCode		=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}


	CLBS_Variable*	pVar	=NULL;
	CLBS_Variable*	pPara	=NULL;
	CLBS_Variable*	pRetVar =NULL;
	for(LONG i=0;i<pFunc->GetParameterVariableCount();i++)
	{
		pVar	=(CLBS_Variable*)pFunc->GetParameterVariable(i);
		pPara	=(CLBS_Variable*)Parameter.GetObj(i);
		*pVar	=*pPara;
		pVar->SetRefVaraibleInfo(0,pPara);

	}
	pFunc->SetCallArea(m_pScriptParser);
	if(!pFunc->Execute())
	{
		if(pErrorCode != NULL)//函数执行失败
			*pErrorCode		=LBS_AEC_FAILURE;
		return NULL;
	}

	pRetVar	=(CLBS_Variable*)pFunc->GetReturnVariable();
	if(pRetVar != NULL)
		pRetVar->AddRef();

	if(pErrorCode != NULL)
		*pErrorCode		=LBS_AEC_SUCESS;

	return pRetVar;
}

CString			CLBS_ScriptDriver::GetLastError()
{
	return CErrorSystem::GetInstance().GetErrorInfo();
}

BYTE	gm_theKeys[16]=
{
		0x06,0x05,0x02,0x04,
		0x05,0x04,0x03,0x00,
		0x04,0x01,0x05,0x01,
		0x06,0x03,0x04,0x05,
};

CHAR*			CLBS_ScriptDriver::GetEncCode(LPCSTR IN pCode)
{
	CHAR*	pEncCode		=NULL;
	DWORD	realSize		=strlen(pCode);
	DWORD	compressedSize=0;
	
	DWORD	bufferSize	=realSize+256;
	BYTE*	pBuffer		=(BYTE*)LB_ALLOC(bufferSize);
	DWORD	i			=0;
	CHAR	header[16];
	CHAR*	pPos		=NULL;
	

	compressedSize =bufferSize;
	if(Z_OK != compress(pBuffer,&compressedSize,(BYTE*)pCode,realSize))
	{
		memcpy(pBuffer,pCode,realSize);
		compressedSize =realSize;
	}
	ZeroMemory(header,sizeof(header));
	sprintf(header,"%s%02d.",ENC_HEADER_FLAG,(realSize/compressedSize)+1);
	

	pEncCode	=(CHAR*)LB_ALLOC(compressedSize*2 + 16);
	strcpy(pEncCode,header);
	pPos		=pEncCode+strlen(header);


	for(i=0;i<compressedSize;i++)
	{
		pBuffer[i] ^= gm_theKeys[i%16];
		pBuffer[i]	= ~pBuffer[i];

		sprintf(pPos,"%02x",pBuffer[i]);
		pPos+=2;
	}
	*pPos=0;
    LB_FREE((LPVoid&)pBuffer);

	return pEncCode;
}
CHAR*			CLBS_ScriptDriver::GetDecCode(LPCSTR IN pEncCode)
{
	if(pEncCode == NULL)
		return NULL;
	DWORD		dataSize	=strlen(pEncCode);
	if(dataSize <= 8)
		return NULL;

	if(strncmp(pEncCode,ENC_HEADER_FLAG,strlen(ENC_HEADER_FLAG))!=0)
		return NULL;
	CHAR		tmp[4];
	DWORD		i			=0;
	DWORD		get_count	=0;
	DWORD		v			=0;
	DWORD		count		=0;
	CHAR*		pStart		=(CHAR*)pEncCode+strlen(ENC_HEADER_FLAG);
	CHAR*		pEnd 		=strchr(pStart,'.');
	if(pEnd==NULL)
		return NULL;
	if( (pEnd-pEncCode)>8)
		return NULL;
	ZeroMemory(tmp,sizeof(tmp));
	strncpy(tmp,pStart,2);
	LONG		scale	=atoll(tmp);
	if(scale<=0)scale =4;



	CHAR*		pCode		=NULL;	
	BYTE*		pData		=(BYTE*)LB_ALLOC(dataSize);
	pEnd ++;
	for(i=0;i<dataSize-8;i++)
	{
		if(pEnd[i]==' '||pEnd[i]=='\r'||pEnd[i]=='\n'||pEnd[i]=='\t')
			continue;

		tmp[get_count]=pEnd[i];
		get_count++;
		tmp[get_count]=0;
		if(get_count>=2)
		{
			get_count=0;
			sscanf(tmp,"%x",&v);
			v =~ v;
			v ^= gm_theKeys[count%16];
			pData[count]=(BYTE)v;
			count++;
		}
	}

	
	DWORD	realSize =count*scale+128;
	pCode	=(CHAR*)LB_ALLOC(realSize);
	if(Z_OK != uncompress((BYTE*)pCode,&realSize,pData,count))
	{
		memcpy(pCode,pData,count);
		realSize=count;
	}
	pCode[realSize]=0;	

    LB_FREE((LPVoid&)pData);

	return pCode;
}
VOID			CLBS_ScriptDriver::FreeEncOrDecCode(CHAR* IN pCode)
{
    LB_FREE((LPVoid&)pCode);
}
