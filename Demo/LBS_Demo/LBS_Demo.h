// LBS_Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CLBS_DemoApp:
// �йش����ʵ�֣������ LBS_Demo.cpp
//

class CLBS_DemoApp : public CWinApp
{
public:
	CLBS_DemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLBS_DemoApp theApp;
