#pragma once
#include "../Base/LBS_CodeBase.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_ScriptMacroDefine.h
	created:	2012/05/09
	author:		LB
	purpose:	脚本宏定义类
\*********************************************************************/



class LBS_EXPORT CLBS_ScriptMacroDefine :
	public CLBS_CodeBase
{
protected:
	LPSTR					m_pMacroValue;
	BOOL					m_bIsString;
public:
	CLBS_ScriptMacroDefine(CLBS_CodeBase* IN pParent);
	virtual ~CLBS_ScriptMacroDefine(void);
public:
	LPCSTR					GetValue(){return m_pMacroValue;}
	BOOL					IsString(){return m_bIsString;}

public:
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);

};
