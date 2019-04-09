#include "StdAfx.h"
#include ".\lbs_syntaxparser.h"
#include "../Script/LBS_Script_FunctionMap.h"
#include "../Script/LBS_ScriptThread.h"
#include "../Script/LBS_Script_Timer.h"
#include "../Script/LBS_Script_Lock.h"
#include "../Script/LBS_Script_UDP.h"
#include "../Script/LBS_Script_TCP.h"


CLBS_SyntaxParser::CLBS_SyntaxParser(CLBS_CodeBase* IN pParent,CLBS_ScriptDriver* IN pScriptDrvier):
CLBS_CodeBase(pParent)
{
	m_Type						=LOT_SCRIPT_PARSER;
	m_pScriptDriver				=pScriptDrvier;
	m_pScriptMainFileName		=NULL;
	m_pScriptRootDirectory		=NULL;
}

CLBS_SyntaxParser::~CLBS_SyntaxParser(void)
{
	Clear();
}
VOID		CLBS_SyntaxParser::Clear()
{
	m_theTCPList.Clear();
	m_theUDPList.Clear();
	m_theTimerList.Clear();


	ClearThread();
	m_theThreadEventList.Clear();
	m_theThreadLockList.Clear();
	ClearMacro();
	ClearClassPrototype();
	m_theScriptFileList.Clear();
    LB_FREE((LPVoid&)m_pScriptMainFileName);
    LB_FREE((LPVoid&)m_pScriptRootDirectory);
}
BOOL		CLBS_SyntaxParser::LoadScript(LPCSTR IN pFileName)
{
	Clear();
	
	DWORD	len				=(DWORD)strlen(pFileName);
	m_pScriptMainFileName	=(LPSTR)LB_ALLOC(len+1);
	m_pScriptRootDirectory	=(LPSTR)LB_ALLOC(len+1);
	ZeroMemory(m_pScriptMainFileName,len+1);
	strncpy(m_pScriptMainFileName,pFileName,len);
	ArrangeFilePath(m_pScriptMainFileName);
	
	ZeroMemory(m_pScriptRootDirectory,len+1);
	strncpy(m_pScriptRootDirectory,pFileName,len);
	for(size_t i=strlen(m_pScriptRootDirectory)-1;i>0;i--)
	{
		if(m_pScriptRootDirectory[i]=='\\'||m_pScriptRootDirectory[i]=='/')
		{
			m_pScriptRootDirectory[i]=0;
			break;
		}
	}
	if(AddScriptFile(m_pScriptMainFileName) == NULL)
		return FALSE;

	return TRUE;
}
BOOL					CLBS_SyntaxParser::LoadScriptFromMemory(VOID* IN pMem,DWORD IN dwLen)
{
	Clear();
	CHAR	pFileName[1024];
	GetModuleFileName(NULL,pFileName,sizeof(pFileName));

	DWORD	len				=(DWORD)strlen(pFileName);
	m_pScriptRootDirectory	=(LPSTR)LB_ALLOC(len+1);

	m_pScriptMainFileName	=(LPSTR)LB_ALLOC(64);
	sprintf(m_pScriptMainFileName,"mem_%x_%x_%d",GetTickCount(),(DWORD_PTR)pMem,dwLen);


	ZeroMemory(m_pScriptRootDirectory,len+1);
	strncpy(m_pScriptRootDirectory,pFileName,len);
	for(size_t i=strlen(m_pScriptRootDirectory)-1;i>0;i--)
	{
		if(m_pScriptRootDirectory[i]=='\\'||m_pScriptRootDirectory[i]=='/')
		{
			m_pScriptRootDirectory[i]=0;
			break;
		}
	}
	if(AddScriptMemoryFile(m_pScriptMainFileName,pMem,dwLen) == NULL)
		return FALSE;

	return TRUE;
}

CLBS_ScriptFile*		CLBS_SyntaxParser::AddScriptFile(LPCSTR IN pFileName)
{
	ArrangeFilePath((LPSTR)pFileName);
	CLBS_ScriptFile*	pNewFile	=GetScriptFile(pFileName);
	if(pNewFile != NULL)
		return pNewFile;
	
	//TRACE("%s\r\n",pFileName);

	pNewFile	=new CLBS_ScriptFile(this);
	if(!pNewFile->Load(pFileName))
	{
		LBS_SafeRelease(pNewFile);
		return NULL;
	}

	m_theScriptFileList.AddObj(pNewFile);
	pNewFile->Release();

	return pNewFile;
}
CLBS_ScriptFile*		CLBS_SyntaxParser::AddScriptMemoryFile(LPCSTR IN pFileName,VOID* IN pMem,DWORD IN dwLen)
{
	CLBS_ScriptFile*	pNewFile	=GetScriptFile(pFileName);
	if(pNewFile != NULL)
		return pNewFile;
	//TRACE("%s\r\n",pFileName);

	pNewFile	=new CLBS_ScriptFile(this);
	if(!pNewFile->LoadFromMemory(pFileName,pMem,dwLen))
	{
		LBS_SafeRelease(pNewFile);
		return NULL;
	}
	m_theScriptFileList.AddObj(pNewFile);
	pNewFile->Release();
	return pNewFile;
}
CLBS_ScriptFile*		CLBS_SyntaxParser::GetScriptFile(LPCSTR IN pFileName)
{
	CLBS_ScriptFile*	pFile;
	for(LONG i=0;i<m_theScriptFileList.GetCount();i++)
	{
		pFile	=(CLBS_ScriptFile*)m_theScriptFileList.GetObj(i);
		if(strcmpi(pFile->GetFileName(),pFileName) == 0)
		{
			return pFile;
		}
	}
	return NULL;
}

CLBS_CodeBase* CLBS_SyntaxParser::GetMacro(LPCSTR IN pMacroName)
{
	CLBS_CodeBase*		pMacro=NULL;
	for(LONG i=0;i<m_theMacroDefineList.GetCount();i++)
	{
		pMacro	=(CLBS_CodeBase*)m_theMacroDefineList.GetObj(i);
		if(strcmp(pMacroName,pMacro->GetName())==0)
			return pMacro;
	}
	return NULL;
}
VOID					CLBS_SyntaxParser::AddMacro(CLBS_CodeBase* IN pNewMacro)
{
	if(GetMacro(pNewMacro->GetName()) != NULL)
		return;
	m_theMacroDefineList.AddObj(pNewMacro);
}
VOID					CLBS_SyntaxParser::ClearMacro()
{
	m_theMacroDefineList.Clear();
}
CLBS_ObjBase*			CLBS_SyntaxParser::GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent)
{
	CLBS_ObjBase*	pFunc	=GetMemberFunction(pFuncName);
	if(pFunc != NULL)
		return pFunc;

	return CLBS_Script_FunctionMap::GetInstance().GetFunction(pFuncName);

}

CLBS_CodeBase*	CLBS_SyntaxParser::GetClassPrototype(LPCSTR IN pClassName)
{
	return (CLBS_CodeBase*)m_theClassDefineList.FindObj(pClassName);
}
BOOL					CLBS_SyntaxParser::AddClassPrototype(CLBS_CodeBase* IN pNewClass)
{
	
	if(GetClassPrototype(pNewClass->GetName())!=NULL)
	{
		ShowDebugTrace("#错误：类[%s]重复定义！",pNewClass->GetName());
		return FALSE;
	}
	m_theClassDefineList.AddObj(pNewClass);
	return TRUE;
}
VOID					CLBS_SyntaxParser::ClearClassPrototype()
{
	m_theClassDefineList.Clear();
}
CLBS_ObjBase*		CLBS_SyntaxParser::CreateNewThread(CLBS_ObjBase* IN pThreadFunc)
{
	CLBS_ObjBase*	pNewThread =new CLBS_ScriptThread(pThreadFunc);
	m_theChildThreadList.AddObj(pNewThread);
	

	pNewThread->Release();
	return pNewThread;
}
VOID					CLBS_SyntaxParser::DestroyThread(CLBS_ObjBase* IN pThread)
{
	if(pThread != NULL)
		((CLBS_ScriptThread*)pThread)->Terminate();
	m_theChildThreadList.DelObj(pThread);
}
VOID					CLBS_SyntaxParser::ClearThread()
{
	CLBS_ScriptThread*	pThread =NULL;
	for(LONG i=0;i<m_theChildThreadList.GetCount();i++)
	{
		pThread	=(CLBS_ScriptThread*)m_theChildThreadList.GetObj(i);
		if(pThread != NULL)
			pThread->Terminate();
	}	
	m_theChildThreadList.Clear();
}
CLBS_ObjBase*			CLBS_SyntaxParser::CreateNewLock()
{
	CLBS_ObjBase*	pLock	=new CLBS_Script_Lock();
	m_theThreadLockList.AddObj(pLock);
	pLock->Release();
	return pLock;
}
VOID					CLBS_SyntaxParser::DestroyLock(CLBS_ObjBase* IN pLock)
{
	m_theThreadLockList.DelObj(pLock);
}
CLBS_ObjBase*			CLBS_SyntaxParser::CreateNewEvent()
{
	CLBS_ObjBase*	pEvent =new CLBS_Script_Event();
	m_theThreadEventList.AddObj(pEvent);
	pEvent->Release();
	return pEvent;
}
VOID					CLBS_SyntaxParser::DestroyEvent(CLBS_ObjBase* IN pEvent)
{
	m_theThreadEventList.DelObj(pEvent);
}
CLBS_ObjBase*			CLBS_SyntaxParser::GetUDP(UINT_PTR IN id)
{
	CLBS_Script_UDP*	pUDP =NULL;
	for(LONG i=0;i<m_theUDPList.GetCount();i++)
	{
		pUDP =(CLBS_Script_UDP*)m_theUDPList.GetObj(i);
		if(pUDP->GetID()==id)
			return pUDP;
	}
	return NULL;
}
UINT_PTR				CLBS_SyntaxParser::OpenUDP(CLBS_ObjBase* IN pRecvCallbackFunc,UINT IN uPort,LPCSTR IN pIpAddr)
{
	CLBS_Script_UDP*	pNewUDP =new CLBS_Script_UDP();
	if(!pNewUDP->Create(uPort,pRecvCallbackFunc,pIpAddr))
	{
		pNewUDP->Release();
		return -1;
	}
	m_theUDPList.AddObj(pNewUDP);
	pNewUDP->Release();
	return pNewUDP->GetID();
}
VOID						CLBS_SyntaxParser::CloseUDP(UINT_PTR IN id)
{
	CLBS_Script_UDP*	pUDP =(CLBS_Script_UDP*)GetUDP(id);
	if(pUDP == NULL)
		return;
	pUDP->Close();
	m_theUDPList.DelObj(pUDP);	
}
LONG						CLBS_SyntaxParser::SendToUdp(UINT_PTR IN id,LPVOID IN pData,LONG IN lLen,LPCSTR IN pIp,UINT IN uPort)
{
	CLBS_Script_UDP*	pUDP =(CLBS_Script_UDP*)GetUDP(id);
	if(pUDP == NULL)
		return -1;
	return pUDP->SendTo(pIp,uPort,pData,lLen);
}

SOCKET						CLBS_SyntaxParser::OpenTCP(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc)
{
	CLBS_Script_TCP*	pNewTCP=new CLBS_Script_TCP(this);
	if(!pNewTCP->Create(pIp,uPort,pAcceptCallbackFunc,pRecvCallbackFunc,pCloseCallbackFunc,TRUE))
	{
		pNewTCP->Release();
		return 0;
	}
	m_theTCPList.AddObj(pNewTCP);
	pNewTCP->Release();
	return pNewTCP->GetHandle();
}
SOCKET						CLBS_SyntaxParser::ConnectTCP(LPCSTR IN pIp,UINT IN uPort,CLBS_ObjBase* IN pAcceptCallbackFunc,CLBS_ObjBase* IN pRecvCallbackFunc,CLBS_ObjBase* IN pCloseCallbackFunc)
{
	CLBS_Script_TCP*	pNewTCP=new CLBS_Script_TCP(this);
	if(!pNewTCP->Create(NULL,0,pAcceptCallbackFunc,pRecvCallbackFunc,pCloseCallbackFunc,FALSE))
	{
		pNewTCP->Release();
		return 0;
	}
	if(!pNewTCP->Connect(pIp,uPort))
	{
		pNewTCP->Release();
		return 0;
	}
	m_theTCPList.AddObj(pNewTCP);
	pNewTCP->Release();
	return pNewTCP->GetHandle();
}
VOID						CLBS_SyntaxParser::SendToTCP(SOCKET IN hSocket,LPVOID IN pData,LONG IN lLen)
{
	CLBS_Script_TCP*	pTcp	=(CLBS_Script_TCP*)GetTCP(hSocket);
	if(pTcp != NULL)
		pTcp->Send(pData,lLen);
}
VOID						CLBS_SyntaxParser::CloseTCP(SOCKET IN hSocket)
{
	CLBS_Script_TCP*	pTcp	=NULL;
	CLBS_Script_TCP*	pClient	=NULL;
	for(long i=0;i<m_theTCPList.GetCount();i++)
	{
		pTcp	=(CLBS_Script_TCP*)m_theTCPList.GetObj(i);
		if(pTcp->GetHandle() == hSocket)
		{
			m_theTCPList.DelObj(pTcp);
			return;
		}
		if(pTcp->IsServer())
		{
			pClient =(CLBS_Script_TCP*)pTcp->FindClient(hSocket);
			if(pClient != NULL)
			{
				pTcp->CloseClient(hSocket);
				return;
			}
		}		
	}
}
CLBS_ObjBase*				CLBS_SyntaxParser::GetTCP(SOCKET IN hSocket)
{
	CLBS_Script_TCP*	pTcp	=NULL;
	CLBS_Script_TCP*	pClient	=NULL;
	for(long i=0;i<m_theTCPList.GetCount();i++)
	{
		pTcp	=(CLBS_Script_TCP*)m_theTCPList.GetObj(i);
		if(pTcp->GetHandle() == hSocket)
			return pTcp;
		if(pTcp->IsServer())
		{
			pClient =(CLBS_Script_TCP*)pTcp->FindClient(hSocket);
			if(pClient != NULL)
				return pClient;
		}		
	}
	return NULL;
}







UINT_PTR				CLBS_SyntaxParser::SetTimer(CLBS_ObjBase* IN pTimerFunc,DWORD IN dwInterval)
{
	CLBS_Script_Timer*	pTimer	=new CLBS_Script_Timer(pTimerFunc,dwInterval);
	m_theTimerList.AddObj(pTimer);
	pTimer->Release();
	return pTimer->GetID();
}
VOID						CLBS_SyntaxParser::KillTimer(UINT_PTR IN idEvent)
{
	m_theTimerList.DelObj(GetTimer(idEvent));
}
CLBS_ObjBase*				CLBS_SyntaxParser::GetTimer(UINT_PTR IN idEvent)
{
	CLBS_Script_Timer*	pTimer =NULL;
	for(LONG i=0;i<m_theTimerList.GetCount();i++)
	{
		pTimer =(CLBS_Script_Timer*)m_theTimerList.GetObj(i);
		if(pTimer->GetID()==idEvent)
			return pTimer;
	}
	return NULL;
}





