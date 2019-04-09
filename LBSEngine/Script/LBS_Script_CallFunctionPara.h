#pragma once
#include "../Base/LBS_ObjListBase.h"
/********************************************************************
	created:	2012-5-30   9:27
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_CallFunctionPara.h
	author:		LB
	
	purpose:	调用脚本函数的参数
*********************************************************************/




class LBS_EXPORT CLBS_Script_CallFunctionPara :
	public CLBS_ObjListBase
{
public:
	CLBS_Script_CallFunctionPara(void);
	virtual ~CLBS_Script_CallFunctionPara(void);
public:
	VOID				Push(LPCSTR IN para);
	VOID				Push(LONG IN para);
	VOID				Push(DWORD IN para);
	VOID				Push(FLOAT IN para);
	VOID				Push(DOUBLE IN para);
	VOID				Push(LPVOID IN para);
	VOID				Push(CLBS_Variable* IN pVar);
	
	
	VOID				Push(VOID* IN pPara,DWORD IN dwParaBytes);
	VOID				Push(LONG* IN pPara,DWORD IN dwCount);
	VOID				Push(DOUBLE* IN pPara,DWORD IN dwCount);
	VOID				Push(LPVOID* IN pPara,DWORD IN dwCount);

	VOID				Push(VOID* IN pPara,DWORD IN dwPerBytes,DWORD IN dwCount);


};
