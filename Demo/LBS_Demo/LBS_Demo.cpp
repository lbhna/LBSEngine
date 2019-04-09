// LBS_Demo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "LBS_Demo.h"
#include "LBS_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLBS_DemoApp

BEGIN_MESSAGE_MAP(CLBS_DemoApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLBS_DemoApp ����

CLBS_DemoApp::CLBS_DemoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLBS_DemoApp ����

CLBS_DemoApp theApp;
char gm_pszSystemDir[1024];

// CLBS_DemoApp ��ʼ��

BOOL CLBS_DemoApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//---------------------------------------------------------------------
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
	//---------------------------------------------------------------------


	CLBS_DemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
