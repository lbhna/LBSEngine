// LBS_Debuger.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "LBS_Debuger.h"
#include "LBS_DebugerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CString			gm_strOpenFileName;
// CLBS_DebugerApp

BEGIN_MESSAGE_MAP(CLBS_DebugerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLBS_DebugerApp 构造

CLBS_DebugerApp::CLBS_DebugerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLBS_DebugerApp 对象

CLBS_DebugerApp theApp;





void	SetFileDefaultEditor2(LPCSTR IN pFileExtName,LPCSTR IN pEditorPathFileName)
{

	char	Path[1024]="\0";
	sprintf(Path,"%s\\shell\\open\\command",pFileExtName);

	HKEY	hkey;
	if(RegOpenKey(HKEY_CLASSES_ROOT,Path,&hkey)!=ERROR_SUCCESS)
		RegCreateKey(HKEY_CLASSES_ROOT,Path,&hkey);

	CString CmdLine;
	CmdLine.Format("%s %%1",pEditorPathFileName);
	RegSetValue(hkey,NULL,REG_SZ,CmdLine,CmdLine.GetLength());

	RegCloseKey(hkey);
}


void	SetFileDefaultEditor(LPCSTR IN pFileExtName,LPCSTR IN pEditorPathFileName)
{

	SetFileDefaultEditor2(pFileExtName,pEditorPathFileName);

	HKEY		hSubKey ;
	CString		CmdLine;
	LONG ret=RegCreateKey(HKEY_CLASSES_ROOT,pFileExtName,&hSubKey);
	CmdLine.Format("%s","LBScriptFile");
	ret=RegSetValue(hSubKey,NULL,REG_SZ,CmdLine,CmdLine.GetLength());



	HKEY hChild ;
	ret=RegOpenKeyEx(hSubKey,"OpenWithProgids",0,KEY_ALL_ACCESS,&hChild);
	DWORD i=0;
	CHAR Name[128];
	while(TRUE)
	{
		ZeroMemory(Name,128);
		if(RegEnumKey(hChild,i,Name,128)!=ERROR_SUCCESS)
			break;
		i++;
		ret=RegDeleteKey(hChild,Name);
	}
	ret=RegDeleteKey(hSubKey,"OpenWithProgids");

	ret=RegCreateKey(hSubKey,"OpenWithProgids\\LBScriptFile",&hSubKey);
	ret=RegCloseKey(hSubKey);


	ret=RegCreateKey(HKEY_CLASSES_ROOT,"LBScriptFile\\shell\\open\\command",&hSubKey);

	CmdLine.Format("\"%s\" \"%%1\"",pEditorPathFileName);
	ret=RegSetValue(hSubKey,NULL,REG_SZ,CmdLine,CmdLine.GetLength());
	RegCloseKey(hSubKey);


	ret=RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts",&hSubKey);
	ret=RegCreateKey(hSubKey,pFileExtName,&hSubKey);
	ret=RegCloseKey(hSubKey);
	ret=RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.lbs",&hSubKey);
	CmdLine.Format("%s","LBScriptFile");
	ret=RegSetValueEx(hSubKey,"Progid",0,REG_SZ,(BYTE*)(LPCSTR)CmdLine,CmdLine.GetLength());
	RegCloseKey(hSubKey);

}




// CLBS_DebugerApp 初始化

BOOL CLBS_DebugerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	char	FileName[1024];
	ZeroMemory(FileName,sizeof(FileName));
	GetModuleFileName(NULL,FileName,1024);
	SetFileDefaultEditor(".lbs",FileName);

	int num	=0;
	LPWSTR* ppCmdLine		=CommandLineToArgvW(GetCommandLineW(),&num);
	gm_strOpenFileName.Empty();
	if(num>1)
	{
		gm_strOpenFileName =CString(ppCmdLine[1]);
	}


	CLBS_DebugerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}
