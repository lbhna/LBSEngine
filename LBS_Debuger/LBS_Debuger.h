// LBS_Debuger.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CLBS_DebugerApp:
// �йش����ʵ�֣������ LBS_Debuger.cpp
//

class CLBS_DebugerApp : public CWinApp
{
public:
	CLBS_DebugerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLBS_DebugerApp theApp;
