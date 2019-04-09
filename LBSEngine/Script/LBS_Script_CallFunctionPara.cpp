#include "StdAfx.h"
#include ".\lbs_script_callfunctionpara.h"
#include "./LBS_Variable.h"

CLBS_Script_CallFunctionPara::CLBS_Script_CallFunctionPara(void)
{
	m_Type			=LOT_FUNCTION_PARAMETER;
}

CLBS_Script_CallFunctionPara::~CLBS_Script_CallFunctionPara(void)
{
}
VOID				CLBS_Script_CallFunctionPara::Push(LPCSTR IN para)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	*pVar =para;
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(LONG IN para)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	*pVar =para;
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(LPVOID IN para)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	*pVar =para;
	AddObj(pVar);
	pVar->Release();
}


VOID				CLBS_Script_CallFunctionPara::Push(DWORD IN para)
{
	Push((LONG)para);
}
VOID				CLBS_Script_CallFunctionPara::Push(FLOAT IN para)
{
	Push((DOUBLE)para);
}
VOID				CLBS_Script_CallFunctionPara::Push(DOUBLE IN para)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	*pVar =para;
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(CLBS_Variable* IN pVar)
{
	AddObj(pVar);
}




VOID				CLBS_Script_CallFunctionPara::Push(VOID* IN pPara,DWORD IN dwParaBytes)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	pVar->Set(pPara,dwParaBytes,LVT_BINARY_DATA);
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(LONG* IN pPara,DWORD IN dwCount)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	pVar->Set(pPara,dwCount*sizeof(LONG),LVT_INTEGER);
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(DOUBLE* IN pPara,DWORD IN dwCount)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	pVar->Set(pPara,dwCount*sizeof(DOUBLE),LVT_FLOAT);
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(LPVOID* IN pPara,DWORD IN dwCount)
{
	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	pVar->Set(pPara,dwCount*sizeof(LPVOID),LVT_POINTER);
	AddObj(pVar);
	pVar->Release();
}
VOID				CLBS_Script_CallFunctionPara::Push(VOID* IN pPara,DWORD IN dwPerBytes,DWORD IN dwCount)
{
	if(pPara == NULL || dwCount <=0 || dwPerBytes <= 0)
		return;
	CLBS_Variable**	pVarArray =new CLBS_Variable*[dwCount];
	for(DWORD i=0;i<dwCount;i++)
	{
		pVarArray[i] =new CLBS_Variable(NULL);
		pVarArray[i]->Set( (BYTE*)pPara + ( dwPerBytes* i) , dwPerBytes,LVT_BINARY_DATA);
	}

	CLBS_Variable*	pVar =new CLBS_Variable(NULL);
	pVar->Set(pVarArray , dwCount*sizeof(CLBS_Variable*) , LVT_VAR_ARRAY);
	AddObj(pVar);
	pVar->Release();

	for(DWORD i=0;i<dwCount;i++)
	{
		pVarArray[i]->Release();
	}
	LBS_SafeDeleteArray(pVarArray);
}
