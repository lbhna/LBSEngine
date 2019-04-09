#pragma once
/********************************************************************
	created:	2012-6-6   16:32
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Script_ClassBase.h
	author:		LB
	
	purpose:	类声明基类
*********************************************************************/

#include "../Base/LBS_CodeBase.h"


class LBS_EXPORT CLBS_Script_ClassBase :
	public CLBS_CodeBase
{
protected:
	CLBS_Script_ClassBase*			m_pPrototype;	//类原型
public:
	CLBS_Script_ClassBase(CLBS_Script_ClassBase* IN pPrototype,CLBS_CodeBase* IN pParent);
	virtual ~CLBS_Script_ClassBase(void);
public:
	CLBS_Script_ClassBase*			GetPrototype(){return m_pPrototype;}
	BOOL							IsPrototypeClass(){return (m_pPrototype == NULL);}
	CLBS_Script_ClassBase*			CreateInstance(CLBS_CodeBase* IN pCurrentArea,BOOL IN bInit=TRUE);//创建一个实例对象
	LPCSTR							GetClassName();
public:
	virtual BOOL					Load(CLBS_ScriptFile* IN pFile,LPSTR& IN OUT pCode,LPSTR IN pEnd);
	virtual BOOL					Save(CString& OUT strCode);
	virtual LONG					GetMemberFunctionCount();
	virtual CLBS_ObjBase*			GetMemberFunction(LPCSTR IN pFuncName);

	
	virtual BOOL					ConstructVariableFromMem(LPVOID IN pMem,DWORD& IN OUT dwSize);

    virtual void                    CallDestructFunction();

public:
	CLBS_Script_ClassBase&			operator=(CLBS_Script_ClassBase& v);
	CLBS_Script_ClassBase*			GetClone();
};
