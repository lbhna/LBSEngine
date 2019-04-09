#include "stdafx.h"
#include "./LBS_CodeEdit.h"


CHAR	gm_scriptKeyWords[]=
{
		"var "		
		"class "		
		"struct "	
		"function "	
		"if "		
		"else "		
		"while "		
		"new "		
		"delete "	
		"include "	
		"define "	
		"return "	
		"break "		
		"continue "	
		"for "		
		"switch "	
		"case "		
		"default "		
};
CHAR gm_scriptConstants[]=
{
		"true "		
		"false "		
		"null "		
		"CR "		
		"LF "		
		"TAB "	
};
//-------------------------------------------------------------------------------------------------------------------

CLBS_CodeEditStyles::CLBS_CodeEditStyle::CLBS_CodeEditStyle()
{
    m_foregroundColor = RGB(0, 0, 0);
    m_backgroundColor = RGB(255, 255, 255);
    m_fontSize = 9;
    strcpy(m_fontName, "宋体");
    m_bBold     = FALSE;
    m_bItalic   = FALSE;
}
VOID	CLBS_CodeEditStyles::CLBS_CodeEditStyle::Set(COLORREF IN clr, COLORREF IN bkClr, UINT IN fontSize, CHAR* IN pFontName, BOOL IN bBold , BOOL IN bItalic)
{
    m_foregroundColor = clr;
    m_backgroundColor = bkClr;
    m_fontSize = fontSize;
    strcpy(m_fontName, pFontName);
    m_bBold = bBold;
    m_bItalic = bItalic;
}

CLBS_CodeEditStyles::CLBS_CodeEditStyle&		CLBS_CodeEditStyles::CLBS_CodeEditStyle::operator=(CLBS_CodeEditStyle& IN v)
{
    if (this == &v)
        return *this;
    m_foregroundColor = v.m_foregroundColor;
    m_backgroundColor = v.m_backgroundColor;
    m_fontSize = v.m_fontSize;
    strcpy(m_fontName, v.m_fontName);
    m_bBold = v.m_bBold;
    m_bItalic = v.m_bItalic;
    return *this;
}
CLBS_CodeEditStyles::CLBS_CodeEditStyles()
{
    m_SelectTextColor = RGB(255, 255, 200);
    m_SelectTextBkColor = RGB(55, 55, 200);
    m_CaretColor = RGB(255, 255, 255);
    m_CaretWidth = 2;

    DWORD		font_size1 = 12, font_size2 = 14;

    LinenumberStyle.Set(RGB(150, 250, 255), RGB(100, 100, 100), 9, "宋体");
    DefaultTextStyle.Set(RGB(220, 220, 220), RGB(0, 0, 0), font_size1, "宋体");
    OperatorTextStyle.Set(RGB(255, 80, 80), RGB(0, 0, 0), font_size2, "Arial");
    SeparatorTextStyle.Set(RGB(100, 180, 255), RGB(0, 0, 0), font_size2, "Arial");
    KeywordsTextStyle.Set(RGB(50, 120, 255), RGB(0, 0, 0), font_size2 + 2, "Arial");
    NumberTextStyle.Set(RGB(225, 225, 0), RGB(0, 0, 0), font_size2, "Arial");
    ConstantTextStyle.Set(RGB(255, 50, 255), RGB(0, 0, 0), font_size2, "Arial");
    SysfunctionTextStyle.Set(RGB(150, 0, 255), RGB(0, 0, 0), font_size2, "宋体");
    AppfunctionTextStyle.Set(RGB(200, 0, 155), RGB(0, 0, 0), font_size2, "宋体");
    StringTextStyle.Set(RGB(55, 200, 205), RGB(0, 0, 0), font_size2, "宋体");
    CommentsTextStyle.Set(RGB(0, 158, 0), RGB(0, 0, 0), font_size1, "宋体");

}
CLBS_CodeEditStyles&		CLBS_CodeEditStyles::operator = (CLBS_CodeEditStyles& IN v)
{
    if (this == &v)
        return *this;
    m_SelectTextBkColor = v.m_SelectTextBkColor;
    m_SelectTextColor = v.m_SelectTextColor;
    m_CaretColor = v.m_CaretColor;
    m_CaretWidth = v.m_CaretWidth;
    LinenumberStyle = v.LinenumberStyle;
    DefaultTextStyle = v.DefaultTextStyle;
    OperatorTextStyle = v.OperatorTextStyle;
    SeparatorTextStyle = v.SeparatorTextStyle;
    KeywordsTextStyle = v.KeywordsTextStyle;
    NumberTextStyle = v.NumberTextStyle;
    ConstantTextStyle = v.ConstantTextStyle;
    SysfunctionTextStyle = v.SysfunctionTextStyle;
    AppfunctionTextStyle = v.AppfunctionTextStyle;
    StringTextStyle = v.StringTextStyle;
    CommentsTextStyle = v.CommentsTextStyle;
    return *this;

}







CLBS_CodeEdit::CLBS_CodeEdit()
{
	m_hCodeEditor		=NULL;
	m_hParentWnd		=NULL;
	m_pCodeContents		=NULL;

}
CLBS_CodeEdit::~CLBS_CodeEdit()
{

	LBS_SafeDeleteArray(m_pCodeContents);

}	
BOOL	CLBS_CodeEdit::Create(HWND IN hParent,LONG IN x,LONG IN y,LONG IN cx,LONG IN cy,LPCSTR IN pSysFunctions,LPCSTR IN pAppFunctions)
{
	m_hParentWnd	=hParent;
	m_hCodeEditor	=::CreateWindow("Scintilla","脚本代码编辑窗口",
		WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN,
		x,y,cx,cy,
		m_hParentWnd,0,GetModuleHandle(NULL),	0);
	if(m_hCodeEditor == NULL)
	{
		TRACE("CLBS_CodeEdit::Create Failure! %d\r\n",GetLastError());
		return FALSE;
	}

	::ShowWindow(m_hCodeEditor,SW_SHOW);


	COLORREF red		= RGB(255,000,000);
	COLORREF green		= RGB(000,255,000);
	COLORREF blue		= RGB(000,000,255);
	COLORREF white		= RGB(255,255,255);
	COLORREF black		= RGB(000,000,000);
	COLORREF gray		= RGB(150,150,150);
	//----------------------------------------------------------------------
	SendEditor(SCI_SETLEXER		, SCLEX_LBS);
	SendEditor(SCI_SETSTYLEBITS	, 7);
	SendEditor(SCI_SETKEYWORDS	, 0, (LPARAM)gm_scriptKeyWords);
	SendEditor(SCI_SETKEYWORDS	, 1, (LPARAM)gm_scriptConstants);
	SendEditor(SCI_SETKEYWORDS	, 2, (LPARAM)pSysFunctions);
	SendEditor(SCI_SETKEYWORDS	, 3, (LPARAM)pAppFunctions);


	SendEditor(SCI_STYLECLEARALL);
	SendEditor(SCI_SETCODEPAGE,936 );


	//设置行号相关样式
	SetAStyle(STYLE_LINENUMBER, RGB(150,250,255), RGB(100,100,100), 9, "宋体");
	SendEditor(SCI_SETMARGINMASKN,0,-1);	
	SendEditor(SCI_SETMARGINWIDTHN,0,32);
	SendEditor(SCI_SETMARGINWIDTHN,1,0);
	SendEditor(SCI_SETMARGINMASKN,2,-1);	
	SendEditor(SCI_SETMARGINWIDTHN,2,4);


	SetAStyle(STYLE_DEFAULT, white, black, 9, "宋体");
	SendEditor(SCI_SETSELFORE,true,RGB(255,255,200));	
	SendEditor(SCI_SETSELBACK,true,RGB(55,55,200));

	SendEditor(SCI_SETCARETFORE,RGB(255,255,255),0);
	SendEditor(SCI_SETCARETWIDTH,2,0);



	DWORD		font_size1 =12,font_size2 =14;

	//设置默认的样式
	SetAStyle(SCE_LBS2_DEFAULT		, RGB(220,220,220)	,black, font_size1, "宋体");
	SetAStyle(SCE_LBS2_OPERATOR		, RGB(255,80,80)	,black, font_size2, "Arial");
	SetAStyle(SCE_LBS2_SEPARATOR	, RGB(100,180,255)	,black, font_size2, "Arial");
	SetAStyle(SCE_LBS2_KEYWORDS		, RGB(50,120,255)	,black, font_size2,"Arial",TRUE);
	SetAStyle(SCE_LBS2_NUMBER		, RGB(225,225,0)	,black, font_size2, "Arial");
	SetAStyle(SCE_LBS2_CONSTANT		, RGB(255,50,255)	,black, font_size2, "Arial");
	SetAStyle(SCE_LBS2_SYSFUNCTION	, RGB(150,0,255)	,black, font_size2, "宋体");
	SetAStyle(SCE_LBS2_APPFUNCTION	, RGB(200,0,155)	,black, font_size2, "宋体");
	SetAStyle(SCE_LBS2_STRING		, RGB(55,200,205)	,black, font_size2, "宋体");
	SetAStyle(SCE_LBS2_LINECOMMENTS	, RGB(0,158,0)		,black, font_size1, "宋体");
	SetAStyle(SCE_LBS2_COMMENTS		, RGB(0,158,0)		,black, font_size1, "宋体");
	SetAStyle(SCE_LBS2_UNKNOWN		, RGB(130,133,130)	,black, font_size2, "宋体");




	return TRUE;
}


LRESULT CLBS_CodeEdit::SendEditor(UINT Msg, WPARAM wParam, LPARAM lParam) 
{
	if(m_hCodeEditor==NULL)
		return -1;
	return ::SendMessage(m_hCodeEditor, Msg, wParam, lParam);
}
VOID CLBS_CodeEdit::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face, BOOL bBold , BOOL bItalic)
{
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
    SendEditor(SCI_STYLESETBOLD, style, bBold);
    SendEditor(SCI_STYLESETITALIC, style, bItalic);
}
VOID CLBS_CodeEdit::SetAStyle(int style, CLBS_CodeEditStyles::CLBS_CodeEditStyle* IN pStyle)
{
    if (pStyle == NULL)
        return;

    SendEditor(SCI_STYLESETFORE, style, pStyle->m_foregroundColor);
    SendEditor(SCI_STYLESETBACK, style, pStyle->m_backgroundColor);
    if (pStyle->m_fontSize >= 1)
        SendEditor(SCI_STYLESETSIZE , style, pStyle->m_fontSize);
    SendEditor(SCI_STYLESETFONT     , style, reinterpret_cast<LPARAM>(pStyle->m_fontName));

    SendEditor(SCI_STYLESETBOLD     , style, pStyle->m_bBold);
    SendEditor(SCI_STYLESETITALIC   , style, pStyle->m_bItalic);


}

VOID	CLBS_CodeEdit::SetContents(LPCSTR IN pCodeContents)
{
	SendEditor(SCI_EMPTYUNDOBUFFER);
	SendEditor(SCI_CANCEL);
	SendEditor(SCI_SETUNDOCOLLECTION, 0);
	SendEditor(SCI_SETTEXT,0,(LPARAM)pCodeContents);
	SendEditor(SCI_SETUNDOCOLLECTION,1);
}
LONG					CLBS_CodeEdit::GetContentsSize()
{
	return SendEditor(SCI_GETLENGTH);
}

LPCSTR					CLBS_CodeEdit::GetContents()
{
	LBS_SafeDeleteArray(m_pCodeContents);
	LONG	len =SendEditor(SCI_GETLENGTH);
	m_pCodeContents =new CHAR[len+16];
	ZeroMemory(m_pCodeContents,len+16);
	len =SendEditor(SCI_GETTEXT,(WPARAM)(len+1),(LPARAM)m_pCodeContents);
	return m_pCodeContents;
}

BOOL					CLBS_CodeEdit::HaveModifyFlag()
{
	return SendEditor(SCI_GETMODIFY);
}
VOID					CLBS_CodeEdit::ClearModifyFlag()
{
	SendEditor(SCI_SETSAVEPOINT);
}

VOID					CLBS_CodeEdit::SetSelectComments()
{
	if(SendEditor(SCI_SELECTIONISRECTANGLE)!=S_OK)
		return;
	LONG		sel_size	=	SendEditor(SCI_GETSELTEXT);
	if(sel_size <=1)
		return;


	LONG		start_pos	=	SendEditor(SCI_GETSELECTIONSTART);
	LONG		end_pos		=	SendEditor(SCI_GETSELECTIONEND);

	LONG		start_line	=	SendEditor(SCI_LINEFROMPOSITION,(WPARAM)start_pos);
	LONG		end_line	=	SendEditor(SCI_LINEFROMPOSITION,(WPARAM)end_pos);
	if(SendEditor(SCI_POSITIONFROMLINE,end_line)==end_pos)
		end_line--;

	start_pos	=	SendEditor(SCI_POSITIONFROMLINE,(WPARAM)start_line);
	end_pos		=	SendEditor(SCI_POSITIONFROMLINE,(WPARAM)(end_line+1));

	end_pos		-=	1;
	sel_size	=	end_pos - start_pos;
	SendEditor(SCI_SETSEL,start_pos,end_pos);

	{//行注释

		sel_size += 128 + ( ( end_line - start_line ) + 2 )*2;
		CHAR*		pSelContents=	new CHAR[sel_size];
		CHAR*		pLineContents=	new CHAR[sel_size];
		LONG		len			=	0;
		LONG		pos			=	0;
		int			i,j;
		for(LONG iLine =start_line;iLine<=end_line;iLine++)
		{
			len		=	SendEditor(SCI_GETLINE,iLine,(LPARAM)pLineContents);
			for( i=0;i<len;i++)
			{
				if(pLineContents[i]==' ')
					continue;
				if(pLineContents[i]=='\t')
					continue;
				if(pLineContents[i]=='\r')
					continue;
				if(pLineContents[i]=='\n')
					continue;

				break;
			}

			for( j=0;j<i;j++)
				pSelContents[pos+j] =pLineContents[j];
			pos		+=	i;
			if(i==len)
				continue;

			pSelContents[pos+0]='/';
			pSelContents[pos+1]='/';
			pos		+=	2;

			for(j=i;j<len;j++)
			{
				pSelContents[pos] =pLineContents[j];
				pos++;
			}

		}
		pSelContents[pos]=0;

		SendEditor(SCI_REPLACESEL,0,(LPARAM)pSelContents);
		SendEditor(SCI_SETSEL,start_pos,start_pos+strlen(pSelContents));
		LBS_SafeDeleteArray(pSelContents);
		LBS_SafeDeleteArray(pLineContents);
	}

}
VOID					CLBS_CodeEdit::CancelSelectComments()
{
	if(SendEditor(SCI_SELECTIONISRECTANGLE)!=S_OK)
		return;
	LONG		sel_size	=	SendEditor(SCI_GETSELTEXT);
	if(sel_size <=1)
		return;


	LONG		start_pos	=	SendEditor(SCI_GETSELECTIONSTART);
	LONG		end_pos		=	SendEditor(SCI_GETSELECTIONEND);

	LONG		start_line	=	SendEditor(SCI_LINEFROMPOSITION,(WPARAM)start_pos);
	LONG		end_line	=	SendEditor(SCI_LINEFROMPOSITION,(WPARAM)end_pos);
	if(SendEditor(SCI_POSITIONFROMLINE,end_line)==end_pos)
		end_line--;

	start_pos	=	SendEditor(SCI_POSITIONFROMLINE,(WPARAM)start_line);
	end_pos		=	SendEditor(SCI_POSITIONFROMLINE,(WPARAM)(end_line+1));
	end_pos-=1;
	SendEditor(SCI_SETSEL,start_pos,end_pos);

	sel_size  =end_pos-start_pos+32;

	CHAR*		pSelContents=	new CHAR[sel_size];
	CHAR*		pLineContents=	new CHAR[sel_size];
	int			len			=0;
	LONG		pos			=0;
	int			i,j;
	ZeroMemory(pSelContents,sel_size);

	for(LONG iLine =start_line;iLine<=end_line;iLine++)
	{
		ZeroMemory(pLineContents,sel_size);
		len		=	SendEditor(SCI_GETLINE,iLine,(LPARAM)0);
		if(len>sel_size)
		{
			LBS_SafeDeleteArray(pLineContents);
			LBS_SafeDeleteArray(pSelContents);
			return;
		}

		len		=	SendEditor(SCI_GETLINE,iLine,(LPARAM)pLineContents);

		for(i=0;i<len;i++)
		{
			if(pLineContents[i]==' ')
				continue;
			if(pLineContents[i]=='\t')
				continue;
			if(pLineContents[i]=='\r')
				continue;
			if(pLineContents[i]=='\n')
				continue;
			break;
		}
		if(pLineContents[i]=='/'&&pLineContents[i+1]=='/')
		{
			len-=2;
			for(j=i;j<len;j++)
			{
				pLineContents[j]=pLineContents[j+2];
			}
			pLineContents[j]=0;
		}

		for(i=0;i<len;i++,pos++)
		{
			pSelContents[pos]=pLineContents[i];
		}
	}
	SendEditor(SCI_REPLACESEL,0,(LPARAM)(LPCSTR)(pSelContents));
	SendEditor(SCI_SETSEL,start_pos,start_pos+strlen(pSelContents));
	LBS_SafeDeleteArray(pLineContents);
	LBS_SafeDeleteArray(pSelContents);
}
VOID					CLBS_CodeEdit::IndentLeft()
{
	SendEditor(SCI_BACKTAB);
}
VOID					CLBS_CodeEdit::IndentRight()
{
	SendEditor(SCI_TAB);
}
VOID					CLBS_CodeEdit::ToUpper()
{
	SendEditor(SCI_UPPERCASE);
}
VOID					CLBS_CodeEdit::ToLower()
{
	SendEditor(SCI_LOWERCASE);
}
VOID					CLBS_CodeEdit::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	SCNotification*		pNotify	=(SCNotification*)lParam;
	if(pNotify == NULL)
		return;

	switch (pNotify->nmhdr.code) 
	{
	case SCN_CHARADDED:
		{
			if(pNotify->ch=='\r')
			{
				long	cur_pos		=SendEditor(SCI_GETCURRENTPOS);
				long	cur_line	=SendEditor(SCI_LINEFROMPOSITION,(WPARAM)cur_pos);
				if(cur_line<=0)
					return;
				cur_line -=1;
				long	len			=SendEditor(SCI_LINELENGTH,(WPARAM)cur_line);
				char*	pLineContents=new char[len+32];
				len		=SendEditor(SCI_GETLINE,cur_line,(LPARAM)pLineContents);
				for(long i=0;i<len;i++)
				{
					if(pLineContents[i]!=' ' && pLineContents[i]!='\t')
					{
						pLineContents[i]=0;
						break;
					}
				}
				len =strlen(pLineContents);
				if(len>0)
				{
					SendEditor(SCI_INSERTTEXT,(WPARAM)cur_pos,(LPARAM)pLineContents);
					cur_pos+=len;
					SendEditor(SCI_GOTOPOS,(WPARAM)cur_pos);

				}
				delete []pLineContents;
			}



		}
		break;    
	case SCN_SAVEPOINTREACHED:
		{

		}
		break;
	case SCN_SAVEPOINTLEFT:
		{

		}
		break;
	default:
		break;
	}
}

BOOL					CLBS_CodeEdit::IsWindow()
{
	return ::IsWindow(m_hCodeEditor);
}
BOOL					CLBS_CodeEdit::IsWindowVisible()
{
	return ::IsWindowVisible(m_hCodeEditor);
}
VOID					CLBS_CodeEdit::ShowWindow()
{
	::ShowWindow(m_hCodeEditor,SW_SHOW);
}
HWND					CLBS_CodeEdit::GetWnd()
{
	return m_hCodeEditor;
}
HWND					CLBS_CodeEdit::GetParent()
{
	return m_hParentWnd;
}

VOID					CLBS_CodeEdit::MoveWindow(IN int X, IN int Y, IN int nWidth, IN int nHeight, IN BOOL bRepaint)
{
	::MoveWindow(m_hCodeEditor,X,Y,nWidth,nHeight,bRepaint);
}
VOID					CLBS_CodeEdit::MoveWindow(IN RECT& rect, IN BOOL bRepaint)
{
	::MoveWindow(m_hCodeEditor,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,bRepaint);

}

VOID					CLBS_CodeEdit::GetWindowRect(RECT& OUT rect)
{
	::GetWindowRect(m_hCodeEditor,&rect);
}
VOID					CLBS_CodeEdit::SetStyle(CLBS_CodeEditStyles* IN pStyles)
{
	if(pStyles == NULL)
		return;

	m_Styles =*pStyles;
	SendEditor(SCI_SETSELFORE,true,pStyles->m_SelectTextColor);	
	SendEditor(SCI_SETSELBACK,true,pStyles->m_SelectTextBkColor);

	//设置行号相关样式
    SetAStyle(STYLE_LINENUMBER, &pStyles->LinenumberStyle);

	SendEditor(SCI_SETCARETFORE,pStyles->m_CaretColor,0);
	SendEditor(SCI_SETCARETWIDTH,pStyles->m_CaretWidth,0);

	//设置默认的样式
    SetAStyle(SCE_LBS2_DEFAULT, &pStyles->DefaultTextStyle);
    SetAStyle(SCE_LBS2_OPERATOR, &pStyles->OperatorTextStyle);
    SetAStyle(SCE_LBS2_SEPARATOR, &pStyles->SeparatorTextStyle);
    SetAStyle(SCE_LBS2_KEYWORDS, &pStyles->KeywordsTextStyle);
    SetAStyle(SCE_LBS2_NUMBER, &pStyles->NumberTextStyle);
    SetAStyle(SCE_LBS2_CONSTANT, &pStyles->ConstantTextStyle);
    SetAStyle(SCE_LBS2_SYSFUNCTION, &pStyles->SysfunctionTextStyle);
    SetAStyle(SCE_LBS2_APPFUNCTION, &pStyles->AppfunctionTextStyle);
    SetAStyle(SCE_LBS2_STRING, &pStyles->StringTextStyle);
    SetAStyle(SCE_LBS2_LINECOMMENTS, &pStyles->CommentsTextStyle);
    SetAStyle(SCE_LBS2_COMMENTS, &pStyles->CommentsTextStyle);
}
CLBS_CodeEditStyles*	CLBS_CodeEdit::GetStyle()
{
	return &m_Styles;
}
BOOL					CLBS_CodeEdit::FindNext(LPCSTR IN pText,DWORD IN searchFlag)
{
	if(SendEditor(SCI_GETSELTEXT) > 1)
	{
		LONG		end_pos		=SendEditor(SCI_GETSELECTIONEND);
		SendEditor(SCI_SETCURRENTPOS,end_pos);
	}
	SendEditor(SCI_SEARCHANCHOR);
	if(SendEditor(SCI_SEARCHNEXT,(WPARAM)searchFlag,(LPARAM)pText)==-1)
		return FALSE;
	SendEditor(SCI_SCROLLCARET);
	return TRUE;
}

BOOL					CLBS_CodeEdit::FindPrev(LPCSTR IN pText,DWORD IN searchFlag)
{
	SendEditor(SCI_SEARCHANCHOR);
	if(SendEditor(SCI_SEARCHPREV,(WPARAM)searchFlag,(LPARAM)pText)==-1)
		return FALSE;
	SendEditor(SCI_SCROLLCARET);
	return TRUE;
}
BOOL					CLBS_CodeEdit::Replace(LPCSTR IN pText)
{
	LONG len=SendEditor(SCI_GETSELTEXT);
	if(SendEditor(SCI_GETSELTEXT) <= 1)
		return FALSE;
	SendEditor(SCI_REPLACESEL,0,(LPARAM)(LPCSTR)pText);
	return TRUE;
}
LONG					CLBS_CodeEdit::ReplaceAll(LPCSTR IN pText,DWORD IN searchFlag,LPCSTR IN pReplaceText)
{
	LONG			count	=0;
	SendEditor(SCI_SETCURRENTPOS,0);
	while(TRUE)
	{
		SendEditor(SCI_SEARCHANCHOR);
		if(SendEditor(SCI_SEARCHNEXT,(WPARAM)searchFlag,(LPARAM)(LPCSTR)pText)==-1)
			break;
		SendEditor(SCI_REPLACESEL,0,(LPARAM)(LPCSTR)pReplaceText);
		count++;
		SendEditor(SCI_SCROLLCARET);
	}
	return count;
}

