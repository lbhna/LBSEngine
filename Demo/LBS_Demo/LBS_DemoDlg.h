// LBS_DemoDlg.h : ͷ�ļ�
//

#pragma once


// CLBS_DemoDlg �Ի���
class CLBS_DemoDlg : public CDialog
{

// ����
public:
	CLBS_DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LBS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedExecute();
	afx_msg void OnBnClickedCallScriptfunc();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedLoadscript();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
