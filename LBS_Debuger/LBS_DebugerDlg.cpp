// LBS_DebugerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LBS_Debuger.h"
#include "LBS_DebugerDlg.h"
#include ".\lbs_debugerdlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		gm_hAppWnd				=NULL;
HANDLE		gm_hScriptThread		=NULL;
DWORD		gm_dwScriptThreadID	=0;
BOOL		gm_bHaveScript			=FALSE;
BOOL		gm_bScriptRunning		=FALSE;


CLBS_ObjBase*	script_MessageBox(CLBS_ObjListBase& IN theParaList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode)
{
	errorCode	=LBS_AEC_SUCESS;
	if(theParaList.GetCount() != 1)
	{//参数数量不正确
		errorCode	=LBS_AEC_PARA_COUNT_ERROR;
		return NULL;
	}
	CLBS_Variable* pPara =(CLBS_Variable*)theParaList.GetObj(0);
	if(pPara != NULL )
	{
		if(pPara->GetVarType() != LVT_STRING)
		{//参数类型不正确
			errorCode	=LBS_AEC_PARA_TYPE_ERROR;
			return NULL;
		}
		MessageBox(gm_hAppWnd,(LPCSTR)*pPara,"来自：LB_Script",MB_ICONINFORMATION);
	}
	return NULL;
}

// CLBS_DebugerDlg 对话框

UINT TF_ScriptThread(LPVOID lp)
{
	UNREFERENCED_PARAMETER(lp);
	OutputDebugString("!! 脚本线程启动成功...\r\n");

	MSG		thread_msg;
	BOOL	bRet	=FALSE;
	CLBS_ScriptDriver*	pDriver =NULL;
	BOOL	bRun =TRUE;

	while( (bRet = GetMessage( &thread_msg, NULL,TM_LOAD_SCRIPT,TM_EXIT)) != 0)
	{
		switch(thread_msg.message)
		{
		case TM_LOAD_SCRIPT:
			{
				CHAR*	pFileName	=(CHAR*)thread_msg.wParam;
				if(pFileName != NULL)
				{
					LBS_SafeRelease(pDriver);
					pDriver	=new CLBS_ScriptDriver();
					pDriver->LoadScript(pFileName);
					delete	[]pFileName;
				}
			}
			break;
		case TM_STOP_SCRIPT:
			{
				LBS_SafeRelease(pDriver);
				pDriver	=new CLBS_ScriptDriver();
			}
			break;
		case TM_RUN_SCRIPT:
			{
				CHAR*	pFileName	=(CHAR*)thread_msg.wParam;
				if(pFileName != NULL)
				{
					LBS_SafeRelease(pDriver);
					pDriver	=new CLBS_ScriptDriver();
					pDriver->LoadScript(pFileName);
					delete	[]pFileName;
				}				
				

				if(!pDriver->Execute())
				{
					AfxMessageBox(pDriver->GetLastError());
				}
				else
				{
					BOOL bRun =TRUE;
					while(bRun)
					{
						MSG msg;
						BOOL bRet;
						while( (bRet = PeekMessage( &msg, NULL, 0,MAXDWORD,PM_REMOVE)) != 0)
						{ 
							if (bRet == -1)
							{
								bRun=FALSE;
								break;
							}
							else
							{
								TranslateMessage(&msg); 
								DispatchMessage(&msg); 
								if(msg.message ==WM_QUIT||msg.message==TM_STOP_SCRIPT||msg.message==TM_EXIT)
								{
									LBS_SafeRelease(pDriver);
									pDriver	=new CLBS_ScriptDriver();
									bRun=FALSE;
									break;
								}
							}
						}
					}
				}
			}
			break;
		case TM_EXIT:
			{
				OutputDebugString("!! 脚本线程结束...\r\n");
				gm_hScriptThread =NULL;
				LBS_SafeRelease(pDriver);
				return 1;
			}
			break;
		}

	}
	OutputDebugString("!! 脚本线程结束...\r\n");
	gm_hScriptThread =NULL;
	LBS_SafeRelease(pDriver);
	return 0;
}


CLBS_DebugerDlg::CLBS_DebugerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLBS_DebugerDlg::IDD, pParent)
{
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDebugMonitor =NULL;
	m_strCurrentFileName.Empty();
	m_hEditBackBrush =::CreateSolidBrush(RGB(0,0,0));

	LOGFONT		lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfCharSet	=GB2312_CHARSET;
	strcpy(lf.lfFaceName,"宋体");
	lf.lfHeight		=20;
	m_bEncrypt		=FALSE;
	m_theContentsFont.CreateFontIndirect(&lf);
}
CLBS_DebugerDlg::~CLBS_DebugerDlg()
{
	if(m_hEditBackBrush!=NULL)
	{
		DeleteObject(m_hEditBackBrush);
		m_hEditBackBrush=NULL;
	}	
}


void CLBS_DebugerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUTPUT, m_ctrlOutputEdit);
}

BEGIN_MESSAGE_MAP(CLBS_DebugerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_DEBUG_STOP, OnDebugStop)
	ON_COMMAND(ID_FILE_ENCRYPTANDSAVE, OnFileEncryptandsave)
END_MESSAGE_MAP()


// CLBS_DebugerDlg 消息处理程序

BOOL CLBS_DebugerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	gm_hAppWnd =m_hWnd;
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_pDebugMonitor =new CMonitor();
	SetTimer(1,10,NULL);
	CString SysFunctions,AppFunctions;



	SysFunctions.Empty();
	AppFunctions.Empty();
	CLBS_ObjBase*	pFunc =CLBS_ScriptDriver::GetInstance().GetFirstKernelFunction();
	while(pFunc != NULL)
	{
		SysFunctions += pFunc->GetName();
		SysFunctions +=" \n";

		pFunc =CLBS_ScriptDriver::GetInstance().GetNextKernelFunction();
	}


	AppFunctions += "MessageBox ";
    //添加一个系统函数
    CLBS_ScriptDriver::GetInstance().AddSystemFunction("MessageBox", script_MessageBox);


	CRect rect;
	GetClientRect(rect);
	CRect ContentsRect =rect;
	ContentsRect.bottom =rect.top+rect.Height()*0.75;




	rect.top	=ContentsRect.bottom+5;
	m_ctrlOutputEdit.MoveWindow(rect);


	m_ctrlContentsEdit.Create(m_hWnd,ContentsRect.left,ContentsRect.top,ContentsRect.Width(),ContentsRect.Height(),SysFunctions,AppFunctions);
	m_ctrlContentsEdit.ShowWindow();


    //if (!CLBS_ScriptDriver::GetInstance().LoadScript("E:\\libo\\Projects\\LBSEngine_x64\\bin\\testcall.lbs"))
    //    MessageBox("fail");

    //if (!CLBS_ScriptDriver::GetInstance().Execute())
    //    MessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());



	extern CString gm_strOpenFileName;
	if(!gm_strOpenFileName.IsEmpty())
	{
		m_strCurrentFileName =gm_strOpenFileName;
		gm_bHaveScript			=TRUE;
		LoadFileContents();
	}
	DragAcceptFiles(TRUE);



	SECURITY_ATTRIBUTES sa;
	sa.nLength =sizeof(sa);
	sa.lpSecurityDescriptor=NULL;
	sa.bInheritHandle		=TRUE;

	gm_bScriptRunning		=FALSE;
	m_strCurrentFileName.Empty();

	gm_hScriptThread	=CreateThread(&sa,0,(LPTHREAD_START_ROUTINE)TF_ScriptThread,NULL,0,&gm_dwScriptThreadID);






/*

    CLBS_CodeEditStyles* pNewStyles = new CLBS_CodeEditStyles();

    char* gFont = "Courier New";

    pNewStyles->m_SelectTextColor = RGB(255, 255, 200);
    pNewStyles->m_SelectTextBkColor = RGB(55, 55, 200);
    pNewStyles->m_CaretColor = RGB(255, 255, 255);
    pNewStyles->m_CaretWidth = 2;

    DWORD font_size1 = 16, font_size2 = 16;
    pNewStyles->LinenumberStyle.Set(RGB(150, 250, 255), RGB(100, 100, 100), 9, gFont);
    pNewStyles->DefaultTextStyle.Set(RGB(220, 0, 0), RGB(220, 0, 200), font_size1, gFont);
    pNewStyles->OperatorTextStyle.Set(RGB(255, 80, 80), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->SeparatorTextStyle.Set(RGB(100, 180, 255), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->KeywordsTextStyle.Set(RGB(50, 120, 255), RGB(0, 0, 0), font_size2 + 2, gFont);
    pNewStyles->NumberTextStyle.Set(RGB(225, 225, 0), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->ConstantTextStyle.Set(RGB(255, 50, 255), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->SysfunctionTextStyle.Set(RGB(150, 0, 255), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->AppfunctionTextStyle.Set(RGB(200, 0, 155), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->StringTextStyle.Set(RGB(55, 200, 205), RGB(0, 0, 0), font_size2, gFont);
    pNewStyles->CommentsTextStyle.Set(RGB(0, 158, 0), RGB(0, 0, 0), font_size1, gFont);

    m_ctrlContentsEdit.SetStyle(pNewStyles);*/




	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLBS_DebugerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CLBS_DebugerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLBS_DebugerDlg::OnClose()
{
	if(m_pDebugMonitor!=NULL)
	{
		delete m_pDebugMonitor;
		m_pDebugMonitor=NULL;
	}	

	OnFileClose();

	PostThreadMessage(gm_dwScriptThreadID,TM_EXIT,0,0);


	CLBS_ScriptDriver::ExitInstance();

	CDialog::OnClose();
}

BOOL CLBS_DebugerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return FALSE;
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_F4)
		{
			OnFileOpen();
			return TRUE;
		}
		else if(pMsg->wParam == VK_F5)
		{
			if(gm_hScriptThread == NULL)
			{
				OnFileRun();
				return TRUE;
			}
		}		
		else if(pMsg->wParam == VK_F6)
		{
			if(gm_hScriptThread != NULL)
			{
				OnDebugStop();
				return TRUE;
			}
		}
		else if(pMsg->wParam =='S')
		{
			if( (GetKeyState(VK_CONTROL)&0x8000) != 0)
			{
				OnFileSave();
				return TRUE;
			}
		}
		else if(pMsg->wParam =='N')
		{
			if( (GetKeyState(VK_CONTROL)&0x8000) != 0)
			{
				OnFileNew();
				return TRUE;
			}
		}
		else if(pMsg->wParam =='O')
		{
			if( (GetKeyState(VK_CONTROL)&0x8000) != 0)
			{
				OnFileOpen();
				return TRUE;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CLBS_DebugerDlg::OnFileOpen()
{
	OnFileClose();
	CFileDialog		dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LBS 脚本文件(*.lbs)|*.lbs|所有文件(*.*)|*.*||",this,0);
	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_OUTPUT,"");
		gm_bHaveScript			=TRUE;
		m_strCurrentFileName	=dlg.GetPathName();
		LoadFileContents();
	}
}

void CLBS_DebugerDlg::OnFileRun()
{
	OnDebugStop();
	OnFileSave();
	if(m_strCurrentFileName.IsEmpty())
		return;

	if(!CLBS_ScriptDriver::GetInstance().LoadScript(m_strCurrentFileName))
	{
		AfxMessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());
		return;
	}

	CHAR*	pFileName=new CHAR[1024];
	sprintf(pFileName,"%s",m_strCurrentFileName);
	PostThreadMessage(gm_dwScriptThreadID,TM_RUN_SCRIPT,(WPARAM)pFileName,0);
	gm_bScriptRunning=TRUE;



}

void CLBS_DebugerDlg::OnFileExit()
{
	PostMessage(WM_CLOSE);
}
void CLBS_DebugerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent ==1)
	{
		if(m_pDebugMonitor!=NULL)
		{
			KillTimer(1);

			CString str;
			for(DWORD i=0;i<m_pDebugMonitor->GetStringCount();i++)
			{
				if(i==0)
				{
					m_ctrlOutputEdit.GetWindowText(str);
					m_ctrlOutputEdit.SetSel(str.GetLength(),-1);

				}
				else
				{
					m_ctrlOutputEdit.SetSel(-1,-1);
				}
				m_ctrlOutputEdit.ReplaceSel(m_pDebugMonitor->GetString(i));
			}
			m_pDebugMonitor->ClearString();
			SetTimer(1,10,NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


void CLBS_DebugerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(IsWindow(m_ctrlOutputEdit.GetSafeHwnd()))
	{
		CRect rect;
		GetClientRect(rect);
		CRect ContentsRect =rect;
		ContentsRect.bottom =rect.top+rect.Height()*0.75;
		m_ctrlContentsEdit.MoveWindow(ContentsRect);
		rect.top	=ContentsRect.bottom+5;
		m_ctrlOutputEdit.MoveWindow(rect);

	}
}

void CLBS_DebugerDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if(!bSysMenu)
	{
		UINT			enable	=MF_BYCOMMAND|MF_ENABLED;
		UINT			disable	=MF_BYCOMMAND|MF_GRAYED|MF_DISABLED;
		for(UINT i=0;i<pPopupMenu->GetMenuItemCount();i++)
		{
			UINT menu_id=pPopupMenu->GetMenuItemID(i);
			switch(menu_id)
			{
			case ID_FILE_RUN:
				{
					if(!gm_bScriptRunning && !m_strCurrentFileName.IsEmpty())
						pPopupMenu->EnableMenuItem(menu_id,enable);
					else
						pPopupMenu->EnableMenuItem(menu_id,disable);
				}
				break;
			case ID_DEBUG_STOP:
				{
					if(!gm_bScriptRunning)
						pPopupMenu->EnableMenuItem(menu_id,disable);
					else
						pPopupMenu->EnableMenuItem(menu_id,enable);
				}
				break;
			case ID_FILE_CLOSE:
				{
					if(m_strCurrentFileName.IsEmpty())
						pPopupMenu->EnableMenuItem(menu_id,disable);
					else
						pPopupMenu->EnableMenuItem(menu_id,enable);
				}
				break;
			}
		}
	}

}
#define ENC_HEADER_FLAG "xLBS.\0"

void CLBS_DebugerDlg::LoadFileContents()
{
	if(m_strCurrentFileName.IsEmpty())
		return;
	CFile tmpFile;
	if(tmpFile.Open(m_strCurrentFileName,CFile::modeRead|CFile::shareDenyNone))
	{
		DWORD	len		=tmpFile.GetLength();
		char*	pBuf =new char[len+1];
		tmpFile.SeekToBegin();
		len =tmpFile.Read(pBuf,len);
		tmpFile.Close();
		pBuf[len]=0;
		m_bEncrypt =FALSE;

		if(strncmp(pBuf,ENC_HEADER_FLAG,strlen(ENC_HEADER_FLAG))==0)
		{
			CHAR*	pDecCode =CLBS_ScriptDriver::GetInstance().GetDecCode(pBuf);
			if(pDecCode != NULL)
			{
				m_ctrlContentsEdit.SetContents(pDecCode);
				
				CLBS_ScriptDriver::GetInstance().FreeEncOrDecCode(pDecCode);
				m_bEncrypt =TRUE;
			}
			else
			{
				m_ctrlContentsEdit.SetContents(pBuf);
			}
		}
		else
		{
			m_ctrlContentsEdit.SetContents(pBuf);
		}
		delete []pBuf;

		CString str;
		str.Format("LBS_Debuger [ %s ]",m_strCurrentFileName);
		SetWindowText(str);

		CHAR*	pFileName=new CHAR[1024];
		sprintf(pFileName,"%s",m_strCurrentFileName);
		PostThreadMessage(gm_dwScriptThreadID,TM_LOAD_SCRIPT,(WPARAM)pFileName,0);
	}
}
void CLBS_DebugerDlg::SaveFileContents(BOOL IN bEncrypt)
{
	if(m_strCurrentFileName.IsEmpty())
		return;

	CHAR*	buf = (CHAR*)m_ctrlContentsEdit.GetContents();
	CFile tmpFile;
	if(tmpFile.Open(m_strCurrentFileName,CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite|CFile::modeNoInherit|CFile::shareDenyNone))
	{
		tmpFile.SeekToBegin();
		if(bEncrypt)
		{
			CHAR*	pEncCode	=CLBS_ScriptDriver::GetInstance().GetEncCode(buf);
			if(pEncCode != NULL)
			{
				tmpFile.Write(pEncCode,strlen(pEncCode));
				CLBS_ScriptDriver::GetInstance().FreeEncOrDecCode(pEncCode);
			}
		}
		else
		{
			tmpFile.Write(buf,strlen(buf));
		}
		tmpFile.Close();
		m_ctrlContentsEdit.ClearModifyFlag();

		CString str;
		str.Format("LBS_Debuger [ %s ]",m_strCurrentFileName);
		SetWindowText(str);

	}
}

void CLBS_DebugerDlg::OnFileSave()
{
	if(m_strCurrentFileName.IsEmpty())
	{
		CString str;
		if(m_ctrlContentsEdit.GetContentsSize()<2)
			return;

		CFileDialog dlg(FALSE,"lbs",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LBS 脚本文件(*.lbs)|*.lbs|所有文件(*.*)|*.*||",this,0);
		if(dlg.DoModal() != IDOK)
			return;
		m_strCurrentFileName=dlg.GetPathName();
	}
	SaveFileContents(m_bEncrypt);
}

void CLBS_DebugerDlg::OnFileSaveas()
{
	CFileDialog dlg(FALSE,"lbs",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LBS 脚本文件(*.lbs)|*.lbs|所有文件(*.*)|*.*||",this,0);
	if(dlg.DoModal() != IDOK)
		return;
	m_strCurrentFileName	=dlg.GetPathName();
	SaveFileContents(m_bEncrypt);
}

void CLBS_DebugerDlg::OnFileNew()
{
	m_strCurrentFileName.Empty();
	m_ctrlContentsEdit.SetContents("");
	m_ctrlContentsEdit.ClearModifyFlag();

	SetDlgItemText(IDC_OUTPUT,"");
	SetWindowText("LBS_Debuger");
	m_bEncrypt =FALSE;

}

HBRUSH CLBS_DebugerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_OUTPUT:
		{
			pDC->SetTextColor(RGB(255,155,0));
			pDC->SetBkColor(RGB(0,0,0));
			return m_hEditBackBrush;
		}
		break;
	}


	return hbr;
}

void CLBS_DebugerDlg::OnFileClose()
{
	OnDebugStop();

	if(m_ctrlContentsEdit.HaveModifyFlag())
	{
		if(MessageBox("是否保存已修改的内容！","提示",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
		{
			OnFileSave();
		}
	}
	m_strCurrentFileName.Empty();
	m_ctrlContentsEdit.SetContents("");
	m_ctrlContentsEdit.ClearModifyFlag();



	SetDlgItemText(IDC_OUTPUT,"");
	SetWindowText("LBS_Debuger");
	gm_bHaveScript=FALSE;

}

void CLBS_DebugerDlg::OnDropFiles(HDROP hDropInfo)
{
	CHAR szFileName[_MAX_PATH];
	::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);
	::DragFinish(hDropInfo);

	OnFileClose();
	m_strCurrentFileName	=szFileName;
	gm_bHaveScript			=TRUE;
	LoadFileContents();
	

	//CDialog::OnDropFiles(hDropInfo);
}

void CLBS_DebugerDlg::OnHelp()
{
	CHAR	Dir[1024];
	ZeroMemory(Dir,sizeof(Dir));
	GetModuleFileName(NULL,Dir,sizeof(Dir));
	for(size_t i=strlen(Dir)-1;i>0;i--)
	{
		if(Dir[i]=='\\'||Dir[i]=='/')
		{
			Dir[i]=0;
			break;
		}
	}
	CString FileName;
	FileName.Format("%s\\LBSEngine开发手册.chm",Dir);
	
	ShellExecute(NULL,"open",FileName,NULL,NULL,SW_SHOW);

}
#include "AboutDlg.h"
void CLBS_DebugerDlg::OnAbout()
{
	CAboutDlg dlg(this);
	dlg.DoModal();

}

void CLBS_DebugerDlg::OnDebugStop()
{
	PostThreadMessage(gm_dwScriptThreadID,TM_STOP_SCRIPT,0,0);
	gm_bScriptRunning=FALSE;
}

void CLBS_DebugerDlg::OnFileEncryptandsave()
{
	CFileDialog dlg(FALSE,"lbs",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LBS 脚本文件(*.lbs)|*.lbs|所有文件(*.*)|*.*||",this,0);
	if(dlg.DoModal() != IDOK)
		return;
	m_bEncrypt =TRUE;
	m_strCurrentFileName	=dlg.GetPathName();
	SaveFileContents(TRUE);

}

BOOL CLBS_DebugerDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	m_ctrlContentsEdit.OnNotify(wParam,lParam,pResult);

	return CDialog::OnNotify(wParam, lParam, pResult);
}
