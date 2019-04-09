#pragma once

#include "../Base/LBS_ObjBase.h"

/*====================================================================\
		#创建日期:	2015/06/11
		#文件名  :	LBS_CodeEdit
		#作者    :	LBHNA   
		#说明    :	脚本代码编辑控件
\====================================================================*/


class LBS_EXPORT	CLBS_CodeEditStyles
{
public:
	class LBS_EXPORT	CLBS_CodeEditStyle
	{
	public:
		COLORREF		m_foregroundColor;
		COLORREF		m_backgroundColor;
		UINT			m_fontSize;
		CHAR			m_fontName[32];
        BOOL            m_bBold;
        BOOL            m_bItalic;
    public:
        CLBS_CodeEditStyle();
    public:
        VOID	                Set(COLORREF IN clr, COLORREF IN bkClr, UINT IN fontSize, CHAR* IN pFontName,BOOL IN bBold=FALSE,BOOL IN bItalic=FALSE);
        CLBS_CodeEditStyle&		operator=(CLBS_CodeEditStyle& IN v);
	};
public:
	COLORREF					m_SelectTextBkColor;
	COLORREF					m_SelectTextColor;

	COLORREF					m_CaretColor;
	UINT						m_CaretWidth;

	CLBS_CodeEditStyle			LinenumberStyle;
	CLBS_CodeEditStyle			DefaultTextStyle;
	CLBS_CodeEditStyle			OperatorTextStyle;
	CLBS_CodeEditStyle			SeparatorTextStyle;
	CLBS_CodeEditStyle			KeywordsTextStyle;
	CLBS_CodeEditStyle			NumberTextStyle;
	CLBS_CodeEditStyle			ConstantTextStyle;
	CLBS_CodeEditStyle			SysfunctionTextStyle;
	CLBS_CodeEditStyle			AppfunctionTextStyle;
	CLBS_CodeEditStyle			StringTextStyle;
	CLBS_CodeEditStyle			CommentsTextStyle;

public:
    CLBS_CodeEditStyles();
    CLBS_CodeEditStyles&		operator=(CLBS_CodeEditStyles& IN v);
};

#define			LBSFIND_WHOLEWORD	2
#define			LBSFIND_MATCHCASE	4

class LBS_EXPORT	CLBS_CodeEdit:
	public CLBS_ObjBase
{
protected:
	HWND						m_hParentWnd;
	HWND						m_hCodeEditor;
	CHAR*						m_pCodeContents;

	CLBS_CodeEditStyles			m_Styles;
public:
	CLBS_CodeEdit();
	virtual ~CLBS_CodeEdit();

public:
	BOOL					Create(HWND IN hParent,LONG IN x,LONG IN y,LONG IN cx,LONG IN cy,LPCSTR IN pSysFunctions,LPCSTR IN pAppFunctions);
	VOID					SetContents(LPCSTR IN pCodeContents);
	LPCSTR					GetContents();
	LONG					GetContentsSize();

	BOOL					HaveModifyFlag();
	VOID					ClearModifyFlag();

	VOID					SetSelectComments();
	VOID					CancelSelectComments();

	VOID					IndentLeft();
	VOID					IndentRight();
	VOID					ToUpper();
	VOID					ToLower();

	BOOL					IsWindow();
	BOOL					IsWindowVisible();
	VOID					ShowWindow();
	HWND					GetWnd();
	HWND					GetParent();
	VOID					MoveWindow(IN int X, IN int Y, IN int nWidth, IN int nHeight, IN BOOL bRepaint=TRUE);
	VOID					MoveWindow(IN RECT& rect, IN BOOL bRepaint=TRUE);
	VOID					GetWindowRect(RECT& OUT rect);


	VOID					SetStyle(CLBS_CodeEditStyles* IN pStyles);
	CLBS_CodeEditStyles*	GetStyle();
public:

	BOOL					FindNext(LPCSTR IN pText,DWORD IN searchFlag);
	BOOL					FindPrev(LPCSTR IN pText,DWORD IN searchFlag);
	BOOL					Replace(LPCSTR IN pText);
	LONG					ReplaceAll(LPCSTR IN pText,DWORD IN searchFlag,LPCSTR IN pReplaceText);

public:
	VOID					OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	LRESULT					SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0);
    VOID					SetAStyle(int style, COLORREF fore, COLORREF back = RGB(255, 255, 255), int size = -1, const char *face = 0,BOOL bBold=FALSE,BOOL bItalic=FALSE);
    VOID					SetAStyle(int style, CLBS_CodeEditStyles::CLBS_CodeEditStyle* IN pStyle);



};