#pragma  once

/*====================================================================\
		#创建日期:	2015/09/26
		#文件名  :	LBS_EventWnd
		#作者    :	LBHNA   
		#说明    :	脚本事件窗口
\====================================================================*/



class CLBS_EventWnd
{
protected:
	CWnd*					m_pEventWnd;
	static CLBS_EventWnd*	m_pSingleton;
public:
	CLBS_EventWnd();
	~CLBS_EventWnd();
public:
	static CLBS_EventWnd&			GetInstance();
	static void						ExitInstance();
public:
	HWND							GetWnd();
};