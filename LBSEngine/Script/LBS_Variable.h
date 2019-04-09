#pragma once
#include "../Base/LBS_CodeBase.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Variable.h
	created:	2012/05/08
	author:		LB
	purpose:	脚本变量类
\*********************************************************************/


typedef enum LBS_EXPORT _LBS_VARIABEL_TYPE
{
	LVT_UNKNOWN		=0,

	LVT_INTEGER,			//整数
	LVT_STRING,				//字串
	LVT_FLOAT,				//浮点数
	LVT_BINARY_DATA,		//二进制数据
	LVT_POINTER,			//指针型数据

	LVT_CLASS,				//类
	LVT_VAR_ARRAY,			//变量数组

	//--------------------------------
	LVT_FORCE_DWORD =0x7fffffff,
}LBS_VARIABEL_TYPE;


class CLBS_Variable;
typedef CLBS_Variable**  LPVariablePtr;

class LBS_EXPORT CLBS_Variable :
	public CLBS_CodeBase
{
protected:
	LBS_VARIABEL_TYPE	m_VarType;			//变量类型
	VOID*				m_pVarBuffer;		//存储变量内容的缓冲区
	DWORD				m_dwVarBufferSize;	//缓冲区大小
	DWORD				m_dwVarSize;		//变量内容大小
protected:
	DWORD				m_dwSuffix;		//下标值
	BOOL				m_bFullRef;		//整体引用还是下标引用
	CLBS_Variable*		m_pRefVariable;	//属于哪个变量

public:
	CLBS_Variable(CLBS_CodeBase* IN pParent=NULL);
	virtual ~CLBS_Variable(void);
public:
	VOID*					GetDataBuffer(){return m_pVarBuffer;}
	DWORD					GetDataSize(){return m_dwVarSize;}
	LBS_VARIABEL_TYPE		GetVarType(){return m_VarType;}
	VOID					Reset();
	inline VOID				Clear();
	CString					GetTypeName();
	BOOL					ReSize(DWORD IN dwSize);



	BOOL					SetByOriginString(LPSTR IN pString,BOOL IN bEnableString=FALSE);
	BOOL					Set(VOID* IN pData,DWORD IN dwSize,LBS_VARIABEL_TYPE IN type);
	DWORD					GetMemberCount();
	VOID*					operator[](DWORD IN dwIndex);

	//是否有引用变量
	BOOL					HaveRefVariable(){return (m_pRefVariable!=NULL);}
	BOOL					IsFullRef(){return m_bFullRef;}
	//得到引用变量
	CLBS_Variable*			GetRefVariable(){return m_pRefVariable;}
	//设置引用变量
	VOID					SetRefVaraibleInfo(DWORD IN dwSuffix,CLBS_Variable* IN pRefVar,BOOL IN bFullRef=TRUE);
	//同步数值到引用的变量
	BOOL					SyncRefVariable();	
	//得到引用变量索引
	DWORD					GetRefVarSuffix(){return m_dwSuffix;}

	//变量成员扩展到dwMemberCount数量
	BOOL					Expand(DWORD IN dwMemberCount,BOOL IN bCut=FALSE);
	//翻转顺序
	VOID					ReverseOrder();
	//字节翻转
	VOID					ReverseByte();//

	BOOL					RemoveAt(LONG IN lIndex,DWORD IN dwCount =1);

public:
	virtual BOOL			Save(CString& OUT strCode);
public:
	virtual CLBS_Variable&	operator=(LPSTR v);
	virtual CLBS_Variable&	operator=(LPCSTR v);
	virtual CLBS_Variable&	operator=(CString v);
	virtual CLBS_Variable&	operator=(LPVOID v);

	virtual CLBS_Variable&	operator=(LONG v);
	virtual CLBS_Variable&	operator=(bool v);
	virtual CLBS_Variable&	operator=(INT v);
	virtual CLBS_Variable&	operator=(DWORD v);

	virtual CLBS_Variable&	operator=(FLOAT v);
	virtual CLBS_Variable&	operator=(DOUBLE v);

	virtual CLBS_Variable&	operator=(CLBS_Variable& v);
	virtual BOOL			operator==(CLBS_Variable& v);
	virtual BOOL			operator!=(CLBS_Variable& v);

	virtual BOOL			operator>(CLBS_Variable& v);
	virtual BOOL			operator<(CLBS_Variable& v);
	virtual BOOL			operator>=(CLBS_Variable& v);
	virtual BOOL			operator<=(CLBS_Variable& v);


public:
	operator LONG();
	operator DWORD();
	operator DOUBLE();
	operator LPVOID();
	operator BOOL();

	operator LPCSTR();
	operator PLONG();
	operator PBYTE();
	operator PCHAR();
	operator PDOUBLE();
    operator LPVariablePtr();


public:
	BOOL					Equal(CLBS_Variable& IN Oper);	//赋值=
	BOOL					Logic_B(CLBS_Variable& IN v);//>
	BOOL					Logic_BE(CLBS_Variable& IN v);//>=
	BOOL					Logic_S(CLBS_Variable& IN v);//<
	BOOL					Logic_SE(CLBS_Variable& IN v);//<=
	BOOL					Logic_E(CLBS_Variable& IN v);//==
	BOOL					Logic_NE(CLBS_Variable& IN v);//!=
	//+-*/
	BOOL					Add(CLBS_Variable& IN Oper);
	BOOL					Sub(CLBS_Variable& IN Oper);
	BOOL					Mul(CLBS_Variable& IN Oper);
	BOOL					Div(CLBS_Variable& IN Oper);
	BOOL					Mod(CLBS_Variable& IN Oper);

	BOOL					AND(CLBS_Variable& IN Oper);
	BOOL					OR(CLBS_Variable& IN Oper);
	BOOL					NOT();
	BOOL					XOR(CLBS_Variable& IN Oper);

	BOOL					LMB(CLBS_Variable& IN Oper);
	BOOL					RMB(CLBS_Variable& IN Oper);




	BOOL					LAND(CLBS_Variable& IN Oper);
	BOOL					LOR(CLBS_Variable& IN Oper);
	BOOL					LNOT();

	BOOL					Minus();//取负
	BOOL					SetSuffixValue(DWORD IN dwIndex,CLBS_Variable* IN Oper);
	CLBS_Variable*			GetSuffixValue(DWORD IN dwIndex);
public:
	BOOL					ToFloat();
	BOOL					ToInteger();
	BOOL					IsNull();

public:
	virtual CLBS_ObjBase*	GetFunction(LPCSTR IN pFuncName,BOOL IN bIncludeParent/* =TRUE */);

protected:
	VOID					AdjustVarBuffer(DWORD IN dwNewSize);
};
