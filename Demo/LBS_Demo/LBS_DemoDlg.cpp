// LBS_DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LBS_Demo.h"
#include "LBS_DemoDlg.h"
#include ".\lbs_demodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLBS_DemoDlg 对话框



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
		MessageBox(AfxGetMainWnd()->m_hWnd,(LPCSTR)*pPara,"来自：LB_Script",MB_ICONINFORMATION);
	}
	return NULL;
}







CLBS_DemoDlg::CLBS_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLBS_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CLBS_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLBS_DemoDlg, CDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EXECUTE, OnBnClickedExecute)
	ON_BN_CLICKED(IDC_CALL_SCRIPTFUNC, OnBnClickedCallScriptfunc)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_LOADSCRIPT, OnBnClickedLoadscript)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()






// CLBS_DemoDlg 消息处理程序
extern char gm_pszSystemDir[1024];

BOOL CLBS_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	//添加一个系统函数
	CLBS_ScriptDriver::GetInstance().AddSystemFunction("MessageBox",script_MessageBox);

	//载入脚本
	CHAR	FileName[1024];
	sprintf(FileName,"%s\\demo.lbs",gm_pszSystemDir);
	if(!CLBS_ScriptDriver::GetInstance().LoadScript(FileName))
	{
		AfxMessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());
	}
	CLBS_ScriptDriver::GetInstance().Execute();

	CLBS_CodeBase*	pParser =CLBS_ScriptDriver::GetInstance().GetScriptParser();
	for(LONG i=0;i<pParser->GetMemberFunctionCount();i++)
	{
		CLBS_CodeBase* pCode =(CLBS_CodeBase*)pParser->GetMemberFunction(i);
		LPCSTR p	=pCode->GetOriginCode();
	}

	CLBS_Script_CallFunctionPara	thePara;
	DWORD							ErrorCode =0;

	CLBS_Variable*	pRet =CLBS_ScriptDriver::GetInstance().CallScriptFunction("main",
		thePara,
		&ErrorCode);
	LBS_SafeRelease(pRet);


	CHAR* pEncCode =CLBS_ScriptDriver::GetInstance().GetEncCode("CLBS_ScriptDriver::GetInstance().CallScriptFunction");
	CHAR* pDecCode =CLBS_ScriptDriver::GetInstance().GetDecCode(pEncCode);

	TRACE("%s == %s\r\n",pEncCode,pDecCode);

	CLBS_ScriptDriver::GetInstance().FreeEncOrDecCode(pEncCode);
	CLBS_ScriptDriver::GetInstance().FreeEncOrDecCode(pDecCode);


	return TRUE; 
}

void CLBS_DemoDlg::OnPaint() 
{
	CDialog::OnPaint();
}

void CLBS_DemoDlg::OnClose()
{
	CLBS_ScriptDriver::ExitInstance();

	CDialog::OnClose();
}

void CLBS_DemoDlg::OnBnClickedExecute()
{
	LARGE_INTEGER litmp; 
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim; 
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	//////////////////////////////////////////////

	if(!CLBS_ScriptDriver::GetInstance().Execute())
		AfxMessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());

	///////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = (dfMinus / dfFreq)*1000.0;// 获得对应的时间值，单位为秒


	CString str;
	str.Format("用时：%.3f ms \r\n",dfTim);
	OutputDebugString(str);
}

struct testStruct
{
	LONG		a;
	
	DOUBLE		b;
	CString		c;
	testStruct *d;
};
void CLBS_DemoDlg::OnBnClickedCallScriptfunc()
{

	CLBS_Script_CallFunctionPara	thePara;
	DWORD							ErrorCode =0;
	
	CLBS_Variable*	pRet =CLBS_ScriptDriver::GetInstance().CallScriptFunction("GetStruct",
																thePara,
																&ErrorCode);
	if(ErrorCode ==LBS_AEC_SUCESS)
	{
		if(pRet->GetVarType() == LVT_CLASS)
		{
			CLBS_Script_ClassBase*	pClass =(CLBS_Script_ClassBase*) (LPVOID)*pRet;
			if(pClass != NULL)
			{
				CLBS_Variable* pV =(CLBS_Variable*)pClass->GetMemberVariable("d");
				if(pV->GetVarType()== LVT_CLASS)
				{
					testStruct ttt;
					ttt.d=new testStruct;

					pV =(CLBS_Variable*)pClass->GetMemberVariable("a");
					ttt.a =(LONG)*pV;
					pV =(CLBS_Variable*)pClass->GetMemberVariable("b");
					ttt.b=(DOUBLE)*pV;

					pV =(CLBS_Variable*)pClass->GetMemberVariable("c");
					ttt.c=(LPCSTR)*pV;

					pV =(CLBS_Variable*)pClass->GetMemberVariable("d");
					pClass =(CLBS_Script_ClassBase*) (LPVOID)*pV;
					
					pV =(CLBS_Variable*)pClass->GetMemberVariable("a");
					ttt.d->a =(LONG)*pV;

					pV =(CLBS_Variable*)pClass->GetMemberVariable("b");
					ttt.d->b=(DOUBLE)*pV;

					pV =(CLBS_Variable*)pClass->GetMemberVariable("c");
					ttt.d->c=(LPCSTR)*pV;
					
					ttt.d->d=NULL;
					delete ttt.d;
				}
				
			}
		}


	}
	LBS_SafeRelease(pRet);

	//CLBS_Script_CallFunctionPara	thePara;
	//thePara.Push(10.0);
	//DWORD				ErrorCode =0;
	//CLBS_Variable*	pRet;
	//pRet =CLBS_ScriptDriver::GetInstance().CallScriptFunction("求圆周长",
	//															thePara,
	//															&ErrorCode);
	//if(ErrorCode ==LBS_AEC_SUCESS)
	//{
	//	DOUBLE			result =(DOUBLE)*pRet;
	//	CString str;
	//	str.Format("返回值圆周长=%f\r\n",result);
	//	AfxMessageBox(str);
	//}
	//LBS_SafeRelease(pRet);
}

void CLBS_DemoDlg::OnBnClickedButton1()
{
	CLBS_Script_CallFunctionPara	thePara;
	thePara.Push(10.0);
	
	CLBS_Variable*		pRet =new CLBS_Variable(NULL);
	*pRet =0.0;
	thePara.Push(pRet);

	DWORD				ErrorCode	=0;
	CLBS_Variable*		pRet2		=CLBS_ScriptDriver::GetInstance().CallScriptFunction("求圆面积",thePara,&ErrorCode);
	if(ErrorCode ==LBS_AEC_SUCESS)
	{
		TRACE("圆面积 = %f == %f\r\n",(DOUBLE)*pRet,(DOUBLE)*pRet2);
		pRet2->Release();
		
	}
	LBS_SafeRelease(pRet);
}


void CLBS_DemoDlg::OnBnClickedLoadscript()
{
	CFileDialog		dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LBS 脚本文件(*.lbs)|*.lbs|所有文件(*.*)|*.*||",this,0);
	if(dlg.DoModal() == IDOK)
	{
		//if(!CLBS_ScriptDriver::GetInstance().LoadScript(dlg.GetPathName()))
		//{
		//	AfxMessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());
		//}

		CFile	tmpFile;
		if(tmpFile.Open(dlg.GetPathName(),CFile::modeRead))
		{
			
			CHAR*	pBuf	=new CHAR[tmpFile.GetLength()+1];
			tmpFile.SeekToBegin();
			DWORD	len =tmpFile.Read(pBuf,tmpFile.GetLength());
			tmpFile.Close();

			if(!CLBS_ScriptDriver::GetInstance().LoadScriptFromMemory(pBuf,len))
				AfxMessageBox(CLBS_ScriptDriver::GetInstance().GetLastError());
			delete []pBuf;
			CLBS_ScriptDriver::GetInstance().Execute();

		}



	}

}

#pragma pack(push,1)
struct trans_child
{
	DWORD	aa;
	DWORD	bb;
	CHAR	cc[16];
};
struct trans
{
	LONG	a;
	DOUBLE	b;
	CHAR	c[32];
	trans_child		child;
	LONG	d;
	BYTE	e;

};
#pragma pack(pop)




void CLBS_DemoDlg::OnBnClickedButton2()
{
	CLBS_Script_CallFunctionPara	thePara;
	trans data;
	data.a=11;
	data.b=22;
	strcpy(data.c,"abcdefghijklmnopqrstuvwxyz");
	data.d=33;
	data.e=44;

	data.child.aa=1111;
	data.child.bb=2222;
	strcpy(data.child.cc,"child_ccccc");

	thePara.Push((VOID*)&data,sizeof(data));
	
	DWORD				ErrorCode	=0;
	CLBS_Variable*		pRet		=CLBS_ScriptDriver::GetInstance().CallScriptFunction("结构传递",
																thePara,
																&ErrorCode);

	LBS_SafeRelease(pRet);	
}

void CLBS_DemoDlg::OnBnClickedButton4()
{
	CLBS_Script_CallFunctionPara	thePara;
	
	trans data[10];
	for(int i=0;i<10;i++)
	{
		data[i].a=1*i;
		data[i].b=2*i;
		sprintf(data[i].c,"member:%d",i);
		sprintf(data[i].child.cc,"child:%d",i);
		data[i].child.aa=i*i;
		data[i].child.bb=2*i*i;
		data[i].d=3*i;
		data[i].e=4*i;
	}

	thePara.Push((VOID*)data,sizeof(trans),10);



	DWORD				ErrorCode	=0;
	CLBS_Variable*		pRet		=CLBS_ScriptDriver::GetInstance().CallScriptFunction("结构数组传递",
		thePara,
		&ErrorCode);

	LBS_SafeRelease(pRet);	



}

void CLBS_DemoDlg::OnBnClickedButton5()
{
	CLBS_Script_CallFunctionPara	thePara;
	DWORD							ErrorCode =0;

	CLBS_Variable*	pRet =CLBS_ScriptDriver::GetInstance().CallScriptFunction("GetStructArray",
		thePara,
		&ErrorCode);
	if(ErrorCode ==LBS_AEC_SUCESS)
	{
		CLBS_ObjListBase*	pList	=(CLBS_ObjListBase*)(LPVOID)*pRet;
		if(pList->GetType()==LOT_OBJ_LIST)
		{
			CLBS_Variable*		pVar;
			for(int i=0;i<pList->GetCount();i++)
			{
				pVar =(CLBS_Variable*)pList->GetObj(i);
				if(pVar->GetVarType() == LVT_CLASS)
				{
					CLBS_Script_ClassBase*	pClass =(CLBS_Script_ClassBase*)(LPVOID)*pVar;

					CString str;
					str.Format("%s  member:(%d,%d,%d)\r\n",
						pVar->GetName(),
						(LONG)*(CLBS_Variable*)pClass->GetMemberVariable((LONG)0),
						(LONG)*(CLBS_Variable*)pClass->GetMemberVariable((LONG)1),
						(LONG)*(CLBS_Variable*)pClass->GetMemberVariable((LONG)2));
					AfxMessageBox(str);

				}
			}

		}
		LBS_SafeRelease(pList);
	}
	LBS_SafeRelease(pRet);

}

void CLBS_DemoDlg::OnBnClickedButton6()
{

	CLBS_Script_CallFunctionPara para;
	para.Push((DWORD)13456);
	CLBS_Variable*	pRet =CLBS_ScriptDriver::GetInstance().CallScriptFunction("GetIdentifyMapType",para);
	DWORD type =(DWORD)*pRet;
	
	TRACE("result= %d \r\n",type);
	LBS_SafeRelease(pRet);

}
