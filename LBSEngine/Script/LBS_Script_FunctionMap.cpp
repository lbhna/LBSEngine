#include "StdAfx.h"
#include ".\lbs_script_functionmap.h"
#include "./LBS_Function.h"
#include "./LBS_ApiImplement.h"


CLBS_Script_FunctionMap* CLBS_Script_FunctionMap::m_pSingleton		=NULL;

CLBS_Script_FunctionMap::CLBS_Script_FunctionMap(void)
{
	m_lGetIndex			=-1;
	m_GetPosition		=NULL;
	m_bGetVariableFunc	=TRUE;


	//初始化变量的私有成员函数
	AddVariablePrivateFunction("length",CLBS_ApiImplement::var_length);
	AddVariablePrivateFunction("bytes",CLBS_ApiImplement::var_bytes);
	AddVariablePrivateFunction("toString",CLBS_ApiImplement::var_toString);
	AddVariablePrivateFunction("toHexString",CLBS_ApiImplement::var_toHexString);
	AddVariablePrivateFunction("toInteger",CLBS_ApiImplement::var_toInteger);
	AddVariablePrivateFunction("toFloat",CLBS_ApiImplement::var_toFloat);
	AddVariablePrivateFunction("toBinary",CLBS_ApiImplement::var_toBinary);
	AddVariablePrivateFunction("toLowerCase",CLBS_ApiImplement::var_toLowerCase);
	AddVariablePrivateFunction("toUpperCase",CLBS_ApiImplement::var_toUpperCase);
	AddVariablePrivateFunction("trimSpace",CLBS_ApiImplement::var_trimSpace);

	AddVariablePrivateFunction("subString",CLBS_ApiImplement::var_subString);
	AddVariablePrivateFunction("indexOf",CLBS_ApiImplement::var_indexOf);
	AddVariablePrivateFunction("lastIndexOf",CLBS_ApiImplement::var_lastIndexOf);
	AddVariablePrivateFunction("reverseBytes",CLBS_ApiImplement::var_reverseBytes);
	AddVariablePrivateFunction("reverse",CLBS_ApiImplement::var_reverse);
	AddVariablePrivateFunction("removeAt",CLBS_ApiImplement::var_removeAt);
	AddVariablePrivateFunction("getName",CLBS_ApiImplement::var_getName);
	AddVariablePrivateFunction("getTypeName",CLBS_ApiImplement::var_getTypeName);
	AddVariablePrivateFunction("getToken",CLBS_ApiImplement::var_getToken);
	AddVariablePrivateFunction("getByte",CLBS_ApiImplement::var_getByte);
	AddVariablePrivateFunction("reSize",CLBS_ApiImplement::var_reSize);


	//初始化系统API函数

	//数学函数------------------------------------------------
	AddSystemApiFunction("sin",CLBS_ApiImplement::math_sin);
	AddSystemApiFunction("cos",CLBS_ApiImplement::math_cos);
	AddSystemApiFunction("tan",CLBS_ApiImplement::math_tan);
	AddSystemApiFunction("asin",CLBS_ApiImplement::math_asin);
	AddSystemApiFunction("acos",CLBS_ApiImplement::math_acos);
	AddSystemApiFunction("atan",CLBS_ApiImplement::math_atan);
	AddSystemApiFunction("sqrt",CLBS_ApiImplement::math_sqrt);
	AddSystemApiFunction("exp",CLBS_ApiImplement::math_exp);
	AddSystemApiFunction("log",CLBS_ApiImplement::math_log);
	AddSystemApiFunction("log10",CLBS_ApiImplement::math_log10);
	AddSystemApiFunction("floor",CLBS_ApiImplement::math_floor);
	AddSystemApiFunction("ceil",CLBS_ApiImplement::math_ceil);
	AddSystemApiFunction("fmod",CLBS_ApiImplement::math_fmod);
	AddSystemApiFunction("pow",CLBS_ApiImplement::math_pow);
	AddSystemApiFunction("rand",CLBS_ApiImplement::math_rand);
	AddSystemApiFunction("abs",CLBS_ApiImplement::math_abs);

	//文件操作-------------------------------------------------
	AddSystemApiFunction("fopen",CLBS_ApiImplement::file_fopen);
	AddSystemApiFunction("fclose",CLBS_ApiImplement::file_fclose);
	AddSystemApiFunction("fread",CLBS_ApiImplement::file_fread);
	AddSystemApiFunction("fwrite",CLBS_ApiImplement::file_fwrite);
	AddSystemApiFunction("freadString",CLBS_ApiImplement::file_freadString);
	AddSystemApiFunction("fwriteString",CLBS_ApiImplement::file_fwriteString);
	AddSystemApiFunction("fseek",CLBS_ApiImplement::file_fseek);
	AddSystemApiFunction("fseekToBegin",CLBS_ApiImplement::file_fseekToBegin);
	AddSystemApiFunction("fseekToEnd",CLBS_ApiImplement::file_fseekToEnd);
	AddSystemApiFunction("fgetLength",CLBS_ApiImplement::file_fgetLength);
	AddSystemApiFunction("fsetLength",CLBS_ApiImplement::file_fsetLength);
	AddSystemApiFunction("fgetPosition",CLBS_ApiImplement::file_fgetPosition);


	//磁盘文件和目录操作----------------------------------------------
	AddSystemApiFunction("createDirectory",CLBS_ApiImplement::explorer_createDirectory);
	AddSystemApiFunction("setCurrentDirectory",CLBS_ApiImplement::explorer_setCurrentDirectory);
	AddSystemApiFunction("getCurrentDirectory",CLBS_ApiImplement::explorer_getCurrentDirectory);
	AddSystemApiFunction("deleteDirectory",CLBS_ApiImplement::explorer_deleteDirectory);
	AddSystemApiFunction("deleteFile",CLBS_ApiImplement::explorer_deleteFile);
	AddSystemApiFunction("listDirectory",CLBS_ApiImplement::explorer_listDirectory);
	AddSystemApiFunction("isFile",CLBS_ApiImplement::explorer_isFile);
	AddSystemApiFunction("isDirectory",CLBS_ApiImplement::explorer_isDirectory);
	AddSystemApiFunction("fileIsExist",CLBS_ApiImplement::explorer_fileIsExist);


	//脚本系统--------------------------------------------------------------
	AddSystemApiFunction("getScriptRootDirectory",CLBS_ApiImplement::script_getScriptRootDirectory);
	AddSystemApiFunction("getScriptLoaderAppDirectory",CLBS_ApiImplement::script_getScriptLoaderAppDirectory);
	
	//定时器--------------------------------------------------------------
	AddSystemApiFunction("createTimer"	,CLBS_ApiImplement::script_createTimer);
	AddSystemApiFunction("killTimer"	,CLBS_ApiImplement::script_killTimer);
	AddSystemApiFunction("execute"		,CLBS_ApiImplement::script_execute);

	//网络--------------------------------------------------------------
	AddSystemApiFunction("openUDP"		,CLBS_ApiImplement::system_openUDP);
	AddSystemApiFunction("sendToUDP"	,CLBS_ApiImplement::system_sendToUDP);
	AddSystemApiFunction("closeUDP"		,CLBS_ApiImplement::system_closeUDP);
	AddSystemApiFunction("getUDPPort"	,CLBS_ApiImplement::system_getUDPPort);


	AddSystemApiFunction("openTCP"		,CLBS_ApiImplement::system_openTCP		);
	AddSystemApiFunction("connectTCP"	,CLBS_ApiImplement::system_connectTCP	);
	AddSystemApiFunction("sendToTCP"	,CLBS_ApiImplement::system_sendToTCP	);
	AddSystemApiFunction("closeTCP"		,CLBS_ApiImplement::system_closeTCP	);
	AddSystemApiFunction("getTCPIP"		,CLBS_ApiImplement::system_getTCPIP		);
	AddSystemApiFunction("getTCPPort"	,CLBS_ApiImplement::system_getTCPPort	);



	AddSystemApiFunction("getLocalHostIpArray"		,CLBS_ApiImplement::system_getLocalHostIpArray);
	AddSystemApiFunction("getLocalHostDefaultIp"	,CLBS_ApiImplement::system_getLocalHostDefaultIp);

	AddSystemApiFunction("raisePrivileges"	,CLBS_ApiImplement::system_raisePrivileges);

	//INI配置文件读写---------------------------------------------------------
	AddSystemApiFunction("getIniString",CLBS_ApiImplement::ini_getIniString);
	AddSystemApiFunction("getIniInteger",CLBS_ApiImplement::ini_getIniInteger);
	AddSystemApiFunction("getIniFloat",CLBS_ApiImplement::ini_getIniFloat);
	AddSystemApiFunction("setIniString",CLBS_ApiImplement::ini_setIniString);
	AddSystemApiFunction("setIniInteger",CLBS_ApiImplement::ini_setIniInteger);
	AddSystemApiFunction("setIniFloat",CLBS_ApiImplement::ini_setIniFloat);

	//系统函数
	AddSystemApiFunction("getDate",CLBS_ApiImplement::system_getDate);
	AddSystemApiFunction("getTime",CLBS_ApiImplement::system_getTime);
	AddSystemApiFunction("getTickCount",CLBS_ApiImplement::system_getTickCount);
	AddSystemApiFunction("getCpuTime",CLBS_ApiImplement::system_getCpuTime);
	AddSystemApiFunction("trace",CLBS_ApiImplement::system_trace);
	AddSystemApiFunction("shellExecute",CLBS_ApiImplement::system_shellExecute);
	AddSystemApiFunction("executeDosCommand",CLBS_ApiImplement::system_executeDosCommand);
	AddSystemApiFunction("sleep",CLBS_ApiImplement::system_sleep);


	//数据结构
	AddSystemApiFunction("arrayCreate",CLBS_ApiImplement::vector_arrayCreate);
	AddSystemApiFunction("arrayAdd",CLBS_ApiImplement::vector_arrayAdd);
	AddSystemApiFunction("arrayInsert",CLBS_ApiImplement::vector_arrayInsert);
	AddSystemApiFunction("arrayGetAt",CLBS_ApiImplement::vector_arrayGetAt);
	AddSystemApiFunction("arraySetAt",CLBS_ApiImplement::vector_arraySetAt);
	AddSystemApiFunction("arrayRemoveAt",CLBS_ApiImplement::vector_arrayRemoveAt);
	AddSystemApiFunction("arrayGetLength",CLBS_ApiImplement::vector_arrayGetLength);
	AddSystemApiFunction("arrayClear",CLBS_ApiImplement::vector_arrayClear);
	AddSystemApiFunction("arrayDestroy",CLBS_ApiImplement::vector_arrayDestroy);
	AddSystemApiFunction("arrayFind",CLBS_ApiImplement::vector_arrayFind);
	
	AddSystemApiFunction("strGetToken",CLBS_ApiImplement::string_strGetToken);
	
	//内存操作
	AddSystemApiFunction("memCopy",CLBS_ApiImplement::memory_memCopy);
	AddSystemApiFunction("memCopyEx",CLBS_ApiImplement::memory_memCopyEx);

	//线程&进程
	AddSystemApiFunction("createThread",CLBS_ApiImplement::thread_createThread);
	AddSystemApiFunction("terminateThread",CLBS_ApiImplement::thread_terminateThread);
	AddSystemApiFunction("suspendThread",CLBS_ApiImplement::thread_suspendThread);
	AddSystemApiFunction("resumeThread",CLBS_ApiImplement::thread_resumeThread);
	AddSystemApiFunction("createLock",CLBS_ApiImplement::thread_createLock);
	AddSystemApiFunction("lock",CLBS_ApiImplement::thread_lock);
	AddSystemApiFunction("unlock",CLBS_ApiImplement::thread_unlock);
	AddSystemApiFunction("destroyLock",CLBS_ApiImplement::thread_destroyLock);
	AddSystemApiFunction("createEvent",CLBS_ApiImplement::thread_createEvent);
	AddSystemApiFunction("setEvent",CLBS_ApiImplement::thread_setEvent);
	AddSystemApiFunction("waitEvent",CLBS_ApiImplement::thread_waitEvent);
	AddSystemApiFunction("destroyEvent",CLBS_ApiImplement::thread_destroyEvent);

}

CLBS_Script_FunctionMap::~CLBS_Script_FunctionMap(void)
{

}
CLBS_Script_FunctionMap&	CLBS_Script_FunctionMap::GetInstance()
{
	if(CLBS_Script_FunctionMap::m_pSingleton == NULL)
	{
		CLBS_Script_FunctionMap::m_pSingleton =new CLBS_Script_FunctionMap();
	}
	return *CLBS_Script_FunctionMap::m_pSingleton;
}
VOID					CLBS_Script_FunctionMap::ExitInstance()
{
	LBS_SafeDelete(CLBS_Script_FunctionMap::m_pSingleton);
}
BOOL					CLBS_Script_FunctionMap::AddVariablePrivateFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr)
{
	if(GetVariablePrivateFunction(pFuncName) != NULL)
		return FALSE;
	
	CLBS_Function*		pFunction =new CLBS_Function(NULL,LFT_MEMBER_FUNCTION,pFuncAddr);
	pFunction->SetName(pFuncName);
	m_theVariablePrivateFunctionMap.AddObj(pFunction);
	pFunction->Release();

	return TRUE;
	
}
CLBS_ObjBase*			CLBS_Script_FunctionMap::GetVariablePrivateFunction(LPCSTR IN pFuncName)
{
	return m_theVariablePrivateFunctionMap.GetObj(pFuncName);
}

BOOL					CLBS_Script_FunctionMap::AddSystemApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr)
{
	if(GetFunction(pFuncName) != NULL)
		return FALSE;

	CLBS_Function*		pFunction =new CLBS_Function(NULL,LFT_SYS_FUNCTION,pFuncAddr);
	pFunction->SetName(pFuncName);
	m_theSystemApiFunctionMap.AddObj(pFunction);
	pFunction->Release();

	return TRUE;
	
}
BOOL					CLBS_Script_FunctionMap::AddApplicationApiFunction(LPCSTR IN pFuncName,LBS_API_FUNCTION IN pFuncAddr)
{
	if(GetFunction(pFuncName) != NULL)
		return FALSE;
	CLBS_Function*		pFunction =new CLBS_Function(NULL,LFT_APP_FUNCTION,pFuncAddr);
	pFunction->SetName(pFuncName);
	m_theSystemApiFunctionMap.AddObj(pFunction);
	pFunction->Release();
	return TRUE;
}

CLBS_ObjBase*			CLBS_Script_FunctionMap::GetFunction(LPCSTR IN pFuncName)
{
	return m_theSystemApiFunctionMap.GetObj(pFuncName);
}
DWORD					CLBS_Script_FunctionMap::GetKernelFunctionCount()
{
	return (m_theSystemApiFunctionMap.GetCount() + m_theVariablePrivateFunctionMap.GetCount());
}
CLBS_ObjBase*			CLBS_Script_FunctionMap::GetFirstKernelFunction()
{
	m_bGetVariableFunc =TRUE;
	CLBS_ObjBase*	pFunc =m_theVariablePrivateFunctionMap.GetFirstObj(m_lGetIndex,m_GetPosition);
	if(pFunc == NULL)
	{
		m_bGetVariableFunc	=FALSE;
		pFunc =m_theSystemApiFunctionMap.GetFirstObj(m_lGetIndex,m_GetPosition);
	}
	return pFunc;
}
CLBS_ObjBase*			CLBS_Script_FunctionMap::GetNextKernelFunction()
{
	CLBS_ObjBase*	pFunc =NULL;
	if(m_bGetVariableFunc)
	{
		pFunc	=m_theVariablePrivateFunctionMap.GetNextObj(m_lGetIndex,m_GetPosition);
		if(pFunc == NULL)
		{
			m_bGetVariableFunc	=FALSE;
			pFunc =m_theSystemApiFunctionMap.GetFirstObj(m_lGetIndex,m_GetPosition);
		}
	}
	else
	{
		pFunc	=m_theSystemApiFunctionMap.GetNextObj(m_lGetIndex,m_GetPosition);
	}

	return pFunc;
}
