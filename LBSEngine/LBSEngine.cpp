// LBSEngine.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "./Script/LBS_Script_FunctionMap.h"
#include "./Script/LBS_EventWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHAR			gm_pszSystemDir[1024]			="\0";

HMODULE			gm_hLibrary					=NULL;


static AFX_EXTENSION_MODULE LBSEngineDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("LBSEngine.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(LBSEngineDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(LBSEngineDLL);

		AfxSocketInit();


		ZeroMemory(gm_pszSystemDir,sizeof(gm_pszSystemDir));
		GetModuleFileName(NULL,gm_pszSystemDir,sizeof(gm_pszSystemDir));
		for(size_t i=strlen(gm_pszSystemDir)-1;i>0;i--)
		{
			if(gm_pszSystemDir[i]=='\\')
			{
				gm_pszSystemDir[i]=0;
				break;
			}
		}

		CLBS_Script_FunctionMap::GetInstance();


		
		if (!Scintilla_RegisterClasses(hInstance))
			return 0;

		//char filename[1024];
		//sprintf(filename,"%s\\SciLexer.dll",gm_pszSystemDir);
		//gm_hLibrary =LoadLibrary(filename);
		//if(gm_hLibrary==NULL)
		//{
		//	AfxMessageBox("缺少DLL [SciLexer.dll]，请重新安装!");
		//	return FALSE;
		//}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LBSEngine.DLL Terminating!\n");
		CLBS_Script_FunctionMap::ExitInstance();
		CLBS_EventWnd::ExitInstance();
	

		
		Scintilla_ReleaseResources();
	

		//FreeLibrary(gm_hLibrary);
		AfxSocketTerm();
		// Terminate the library before destructors are called
		AfxTermExtensionModule(LBSEngineDLL);
	}
	return 1;   // ok
}



LPCSTR	ArrangeFilePath(LPSTR pFilePath)
{
	size_t	len		=strlen(pFilePath);
	LPSTR	pTmp	=(LPSTR)LB_ALLOC((INT)(len+1));
	strcpy(pTmp,pFilePath);
	for(size_t i=0;i<len;i++)
	{
		if(pTmp[i] == '/')
			pTmp[i]='\\';
	}
	PathCanonicalize(pFilePath,pTmp);
    LB_FREE((LPVoid&)pTmp);
	return pFilePath;
}
LPSTR	TrimRight(LPSTR IN pSrc,CHAR IN ch=' ')
{
	LPSTR	p	=pSrc+(strlen(pSrc)-1);
	while(*p == ' '&& p >= pSrc)
	{
		*p=0;
		p--;
	}
	return pSrc;
}
LPSTR	TrimLeft(LPSTR IN pSrc,CHAR IN ch=' ')
{
	LPSTR	p	=pSrc;
	while(*p == ch && *p != 0)
		p++;
	LONG i=0;
	for( i=0;*p!=0;i++,p++)
		pSrc[i]=*p;
	pSrc[i]=0;
	return pSrc;
}
BOOL	IsDigit(CHAR IN ch)
{
	if(ch >= '0' && ch <= '9')
		return TRUE;
	return FALSE;
}
BOOL	IsHexChar(CHAR IN ch)
{
	if(ch >= '0' && ch <= '9')
		return TRUE;
	if(ch >= 'a' && ch <= 'f')
		return TRUE;
	if(ch >= 'A' && ch <='F')
		return TRUE;
	if(ch == 'x' || ch=='X')
		return TRUE;
	return FALSE;
}
INT		GetStringType(LPSTR IN pSrc)
{//返回-1 无效，0：字串，1：10进制整数  2: 浮点数 3：16进制整数
	if(pSrc == NULL)
		return -1;
	if(IsDigit(pSrc[0]))
	{
		if(pSrc[1] == 'x' || pSrc[1]=='X')
			return 3;
		if(strchr(pSrc,'.')!=NULL)
			return 2;
		return 1;
	}
	else if(pSrc[0]=='-'&&IsDigit(pSrc[1]))
	{
		if(strchr(pSrc,'.')!=NULL)
			return 2;
		return 1;
	}
	return 0;
}
VOID	CreateDirectoryTree(LPCSTR IN pPath)
{
	if(pPath==NULL)
		return;
	ArrangeFilePath((LPSTR)pPath);

	LPSTR	pPos =(LPSTR)strchr(pPath,'\\');
	while(pPos!=NULL)
	{
		*pPos = '\0';
		CreateDirectory(pPath,NULL);
		*pPos = '\\';

		pPos++;
		pPos =(LPSTR)strchr(pPos,'\\');
	}
	CreateDirectory(pPath,NULL);
}
