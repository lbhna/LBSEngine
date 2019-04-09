#pragma  once

/*====================================================================\
		#��������:	2015/09/26
		#�ļ���  :	LBS_EventWnd
		#����    :	LBHNA   
		#˵��    :	�ű��¼�����
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