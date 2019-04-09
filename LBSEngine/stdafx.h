// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <SDKDDKVer.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

//#include <winsock2.h>
//#pragma comment(lib," Ws2_32.lib")

#include <afxsock.h>

//#include < Mmsystem.h>
//#pragma comment(lib,"Winmm.lib")

#include "./CodeEditor/include/Scintilla.h"
#include "./CodeEditor/include/SciLexer.h"
#ifdef _DEBUG
#   pragma comment(lib,"SciLexer_d.lib")
#else
#   pragma comment(lib,"SciLexer.lib")
#endif

#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"comctl32.lib")

#include "zlib/zlib.h"
#include "./Support/LBS_ListT.h"
#include "./Support/MemoryPool.h"
#include "./Syntax/LBS_SyntaxParser.h"
#include "./ErrorSystem.h"


#define ENC_HEADER_FLAG "xLBS.\0"
extern	BYTE				gm_theKeys[16];
extern CHAR					gm_pszSystemDir[];









extern DWORD				GetKeywordsCount();
extern PKEYWORDSSTRUCT		GetKeywords(DWORD IN dwIndex);
extern PKEYWORDSSTRUCT		GetOrderKeywordsArray();
extern BOOL					IsMatchKeywords(LPSTR IN pCode,PKEYWORDSSTRUCT& OUT pKeywords,BOOL IN bAbsolute=TRUE);
extern LPCSTR				ArrangeFilePath(LPSTR pFilePath);
extern LPSTR				TrimLeft(LPSTR IN pSrc,CHAR IN ch);
extern LPSTR				TrimRight(LPSTR IN pSrc,CHAR IN ch);
extern INT					GetStringType(LPSTR IN pSrc);
extern BOOL					IsDigit(CHAR IN ch);

extern VOID					CreateDirectoryTree(LPCSTR IN pPath);


