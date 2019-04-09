#pragma once
#include "..\Base\LBS_ObjListBase.h"
#include "..\Base\LBS_CodeBase.h"
#include "..\Script\LBS_ScriptDriver.h"
#include "..\Script\LBS_ScriptFile.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Syntax\LBS_SyntaxParser.h
	created:	2012/05/04
	author:		LB
	purpose:	语法解释器
\*********************************************************************/

class LBS_EXPORT CLBS_ScriptFile;
class LBS_EXPORT CLBS_ScriptDriver;
class LBS_EXPORT CLBS_SyntaxParser :
	public CLBS_CodeBase
{
protected:
	CLBS_ScriptDriver*				m_pScriptDriver;		//脚本驱动
	CLBS_ObjListBase				m_theScriptFileList;	//脚本文件列表
	LPSTR							m_pScriptMainFileName;	//脚本入口文件名
	LPSTR							m_pScriptRootDirectory;	//脚本根目录

protected:
	CLBS_ObjListBase				m_theMacroDefineList;	//宏定义列表
	CLBS_ObjListBase				m_theClassDefineList;	//类定义列表
	CLBS_ObjListBase				m_theChildThreadList;	//子线程列表
	
	CLBS_ObjListBase				m_theThreadLockList;	//同步锁对象列表
	CLBS_ObjListBase				m_theThreadEventList;	//同步事件对象列表

	CLBS_ObjListBase				m_theTimerList;			//定时器对象列表
	CLBS_ObjListBase				m_theUDPList;			//UDP对象列表
	CLBS_ObjListBase				m_theTCPList;			//TCP对象列表


public:
	CLBS_SyntaxParser(CLBS_CodeBase* IN pParent,CLBS_ScriptDriver* IN pScriptDrvier);
	virtual ~CLBS_SyntaxParser(void);

public:
	BOOL						LoadScript(LPCSTR IN pFileName);
	BOOL						LoadScriptFromMemory(VOID* IN pMem,DWORD IN dwLen);
	CLBS_ScriptFile*			AddScriptFile(LPCSTR IN pFileName);
	CLBS_ScriptFile*			AddScriptMemoryFile(LPCSTR IN pFileName,VOID* IN pMem,DWORD IN dwLen);
	CLBS_ScriptDriver*			GetScriptDriver(){return m_pScriptDriver;}


	CLBS_ScriptFile*			GetScriptFile(LPCSTR IN pFileName);
	LPSTR						GetScriptMainFileName(){return m_pScriptMainFileName;}
	LPSTR						GetScriptRootDirectory(){return m_pScriptRootDirectory;}
	VOID						Clear();
public:
	CLBS_CodeBase*				GetMacro(LPCSTR IN pMacroName);
	VOID						AddMacro(CLBS_CodeBase* IN pNewMacro);
	VOID						ClearMacro();

public:
	CLBS_CodeBase*				GetClassPrototype(LPCSTR IN pClassName);
	BOOL						AddClassPrototype(CLBS_CodeBase* IN pNewClass);
	VOID						ClearClassPrototype();
public:
	CLBS_ObjBase*				CreateNewThread(CLBS_ObjBase* IN pThreadFunc);
	VOID						DestroyThread(CLBS_ObjBase* IN pThread);
	VOID						ClearThread();
	CLBS_ObjBase*				CreateNewLock();
	VOID						DestroyLock(CLBS_ObjBase* IN pLock);
	CLBS_ObjBase*				CreateNewEvent();
	VOID						DestroyEvent(CLBS_ObjBase* IN pEvent);
public:
	
	UINT_PTR					OpenUDP(CLBS_ObjBase* IN pRecvCallbackFunc,UINT IN uPort,LPCSTR IN pIpAddr);
	VOID						CloseUDP(UINT_PTR IN id);
	LONG						SendToUdp(UINT_PTR IN id,LPVOID IN pData,LONG IN lLen,LPCSTR IN pIp,UINT IN uPort);
	VOID						CloseUDP();
	CLBS_ObjBase*				GetUDP(UINT_PTR IN id);
public:
	SOCKET						OpenTCP(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc);
	SOCKET						ConnectTCP(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc);
	VOID						SendToTCP(SOCKET IN hSocket,LPVOID IN pData,LONG IN lLen);
	VOID						CloseTCP(SOCKET IN hSocket);
	CLBS_ObjBase*				GetTCP(SOCKET IN hSocket);


public:
	UINT_PTR					SetTimer(CLBS_ObjBase* IN pTimerFunc,DWORD IN dwInterval);
	VOID						KillTimer(UINT_PTR IN idEvent);
	CLBS_ObjBase*				GetTimer(UINT_PTR IN idEvent);



public:
	virtual CLBS_ObjBase*		GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent=TRUE);




};
