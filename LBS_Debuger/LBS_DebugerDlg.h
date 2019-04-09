// LBS_DebugerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <afxmt.h>


class CMonitor:public CWinDebugMonitor
{
	
protected:
	CCriticalSection	m_theSection;
	CStringArray		m_theOutputStringArray;
public:
	DWORD				GetStringCount()
	{
		m_theSection.Lock();
		DWORD count=m_theOutputStringArray.GetCount();
		m_theSection.Unlock();
		return count;
	}
	CString				GetString(DWORD IN dwIndex)
	{
		m_theSection.Lock();
		CString str=m_theOutputStringArray.GetAt(dwIndex);
		m_theSection.Unlock();
		return str;
	}
	VOID				ClearString()
	{
		m_theSection.Lock();
		m_theOutputStringArray.RemoveAll();
		m_theSection.Unlock();

	}
public:
	virtual void OutputWinDebugString(const char *str) 
	{
		m_theSection.Lock();
		m_theOutputStringArray.Add(str);
		m_theSection.Unlock();
	};
	
};

// CLBS_DebugerDlg 对话框
class CLBS_DebugerDlg : public CDialog
{
protected:
	CMonitor*		m_pDebugMonitor;
	CEdit			m_ctrlOutputEdit;
	CString			m_strCurrentFileName;
	HBRUSH			m_hEditBackBrush;
	CFont			m_theContentsFont;
	BOOL			m_bEncrypt;
	CLBS_CodeEdit	m_ctrlContentsEdit;





// 构造
public:
	CLBS_DebugerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CLBS_DebugerDlg();

// 对话框数据
	enum { IDD = IDD_LBS_DEBUGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	void			LoadFileContents();
	void			SaveFileContents(BOOL IN bEncrypt=FALSE);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnFileOpen();
	afx_msg void OnFileRun();
	afx_msg void OnFileExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileNew();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFileClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnHelp();
	afx_msg void OnAbout();
	afx_msg void OnDebugStop();
	afx_msg void OnFileEncryptandsave();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
