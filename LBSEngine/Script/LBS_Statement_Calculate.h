#pragma once
#include "../Base/LBS_CodeBase.h"
#include "./LBS_Statement_CalculateFactor.h"
#include "./LBS_Variable.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Statement_Calculate.h
	created:	2012/05/11
	author:		LB
	purpose:	运算行表达式
\*********************************************************************/

class CLBS_Statement_CalculateFactor;
class CLBS_Statement_Calculate :
	public CLBS_CodeBase
{
protected:
	CLBS_ObjListBase		m_theFactorList;	//算子链表
	KEYWORDS_TYPE			m_EndKeywordType;	//语句结束关键字类型
	BOOL					m_bVarDeclareStatement;
public:
	CLBS_Statement_Calculate(CLBS_CodeBase* IN pParent,BOOL IN bVarDeclareStatement=FALSE,KEYWORDS_TYPE IN EndKeywordType=KWT_END_LINE,CLBS_CodeBase* IN pCallArea=NULL);
	virtual ~CLBS_Statement_Calculate(void);
public:
	VOID					Clear();
	VOID					AddFactor(CLBS_ObjBase* IN pFactor);
	VOID					AddFactor(LPSTR IN pFactorString,LBS_CT_TYPE IN type);
	VOID					AddFactor(VOID* IN pData,DWORD IN dwSize,LBS_CT_TYPE IN type);
	BOOL					AddFactorByWord(LPSTR& IN OUT pCode,LPSTR IN pEnd,LPSTR IN pWordStart,LPSTR IN pWord);
	LONG					GetFactorCount(){return m_theFactorList.GetCount();}

	BOOL					IsChildCalculateStatement(){if(GetParent()==NULL)return FALSE;return ( GetParent()->GetType() == LOT_CODE_CALC_STATEMENT );}
public:
	virtual BOOL			Execute();
	virtual BOOL			Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL			Save(CString& OUT strCode);
	virtual CString			GetString();

	virtual BOOL			IsHaveFactor(LBS_CT_TYPE IN FactorType);
public:
	BOOL					Compile();
protected:
	BOOL					PrepareProcessStatement();
	BOOL					PrepareCompile();
	BOOL					PrepareCompile(CLBS_ObjListBase& IN FactorList,LONG& IN lStartIndex,BOOL IN bChild=FALSE);
	VOID					ProcessLogicOperator();

	CLBS_Variable*			Calculate(
		CLBS_Statement_CalculateFactor* IN pResFactor,
		CLBS_Statement_CalculateFactor* IN pOperFactor,
		CLBS_Statement_CalculateFactor* IN pDstFactor,
		BOOL& OUT bSucess);

};
