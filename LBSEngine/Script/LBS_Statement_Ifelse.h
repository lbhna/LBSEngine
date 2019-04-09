#pragma once
#include "../Base/LBS_CodeBase.h"
/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Ifelse.h
	created:	2012/05/11
	author:		LB
	purpose:	条件语句 if else
\*********************************************************************/


class CLBS_Statement_Ifelse :
	public CLBS_CodeBase
{
protected:
	CLBS_CodeBase*			m_pIfCondition;	//决断条件
	CLBS_CodeBase*			m_pIfCode;		//if	下面的子句
	CLBS_CodeBase*			m_pElseCode;	//else	下面的子句
public:
	CLBS_Statement_Ifelse(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Statement_Ifelse(void);
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
protected:
	BOOL					LoadCondition(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadIfBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);
	BOOL					LoadElseBody(LPSTR& IN OUT pCode,LPSTR IN pEnd);

};
