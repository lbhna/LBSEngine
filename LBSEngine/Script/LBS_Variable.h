#pragma once
#include "../Base/LBS_CodeBase.h"

/********************************************************************\
	filename: 	g:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_Variable.h
	created:	2012/05/08
	author:		LB
	purpose:	�ű�������
\*********************************************************************/


typedef enum LBS_EXPORT _LBS_VARIABEL_TYPE
{
	LVT_UNKNOWN		=0,

	LVT_INTEGER,			//����
	LVT_STRING,				//�ִ�
	LVT_FLOAT,				//������
	LVT_BINARY_DATA,		//����������
	LVT_POINTER,			//ָ��������

	LVT_CLASS,				//��
	LVT_VAR_ARRAY,			//��������

	//--------------------------------
	LVT_FORCE_DWORD =0x7fffffff,
}LBS_VARIABEL_TYPE;


class CLBS_Variable;
typedef CLBS_Variable**  LPVariablePtr;

class LBS_EXPORT CLBS_Variable :
	public CLBS_CodeBase
{
protected:
	LBS_VARIABEL_TYPE	m_VarType;			//��������
	VOID*				m_pVarBuffer;		//�洢�������ݵĻ�����
	DWORD				m_dwVarBufferSize;	//��������С
	DWORD				m_dwVarSize;		//�������ݴ�С
protected:
	DWORD				m_dwSuffix;		//�±�ֵ
	BOOL				m_bFullRef;		//�������û����±�����
	CLBS_Variable*		m_pRefVariable;	//�����ĸ�����

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

	//�Ƿ������ñ���
	BOOL					HaveRefVariable(){return (m_pRefVariable!=NULL);}
	BOOL					IsFullRef(){return m_bFullRef;}
	//�õ����ñ���
	CLBS_Variable*			GetRefVariable(){return m_pRefVariable;}
	//�������ñ���
	VOID					SetRefVaraibleInfo(DWORD IN dwSuffix,CLBS_Variable* IN pRefVar,BOOL IN bFullRef=TRUE);
	//ͬ����ֵ�����õı���
	BOOL					SyncRefVariable();	
	//�õ����ñ�������
	DWORD					GetRefVarSuffix(){return m_dwSuffix;}

	//������Ա��չ��dwMemberCount����
	BOOL					Expand(DWORD IN dwMemberCount,BOOL IN bCut=FALSE);
	//��ת˳��
	VOID					ReverseOrder();
	//�ֽڷ�ת
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
	BOOL					Equal(CLBS_Variable& IN Oper);	//��ֵ=
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

	BOOL					Minus();//ȡ��
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
