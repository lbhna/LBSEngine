#pragma once
#include "../Base/LBS_ObjBase.h"
#include "../Syntax/LBS_SyntaxParser.h"

#define				KEY_WORDS_MAX_LEN	0x80		//�ؼ�����󳤶�

#define				KWC_KEYWORDS		0x0001		//�ǲ��ǹؼ���
#define				KWC_OPERATOR		0x0002		//�в��������ԵĹؼ���
#define				KWC_PREFIX			0x0004		//��ǰ׺���ԵĹؼ���
#define				KWC_SEPERATOR		0x0008		//�зָ����ԵĹؼ���
#define				KWC_LOGIC_OPERATOR	0x0010		//�߼�������Ĺؼ���

typedef enum LBS_EXPORT _KEYWORDSTYPE
{
	KWT_ADD=0,					//��+
	KWT_SUB,					//��-
	KWT_MUL,					//��*
	KWT_DIV,					//��/
	KWT_EQU,					//��ֵ��=
	KWT_MOD,					//ģ��%
	KWT_EQUAL,					//�Ƚϵ�==
	KWT_B,						//����>
	KWT_BE,						//���ڵ���>=
	KWT_S,						//С��<
	KWT_SE,						//С�ڵ���<=
	KWT_LOGIC_AND,				//�߼���&&
	KWT_LOGIC_OR,				//�߼���||
	KWT_LOGIC_NOT,				//�߼���!
	KWT_AND,					//λ��&
	KWT_OR,						//λ��|
	KWT_NOT,					//λ��~
	KWT_XOR,					//λ���^
	KWT_LMB,					//λ����<<
	KWT_RMB,					//λ����>>
	KWT_SUFFIX_START,			//ǰȡ�±��[
	KWT_SUFFIX_END,				//��ȡ�±��]
	KWT_STRING_DOUBLE,			//˫����"
	KWT_STRING_SINGLE,			//������'
	KWT_CODE_BLOCK_START,		//����鿪ʼ{
	KWT_CODE_BLOCK_END,			//��������}
	KWT_PARA_LIST_START,		//�����б�ʼ(
	KWT_PARA_LIST_END,			//�����б����)
	KWT_PARA_SEPERATOR,			//�����ָ�,
	KWT_PRE_PROCESS_PREFIX,		//Ԥ����ָ��ǰ׺#
	KWT_WORD_SPACE,				//�ո��
	KWT_WORD_TABLE,				//�Ʊ��\t
	KWT_END_LINE,				//�н��� ;
	KWT_CARRIAGE,				//�س���\r
	KWT_RETURN,					//���з�\n
	KWT_CHILD_AREA,				//����.
	KWT_ORIGIN_AREA,			//������::
	KWT_VARIABLE_DEFINE,		//��������var
	KWT_CLASS_DEFINE,			//�ඨ��class
	KWT_STRUCT_DEFINE,			//�ṹ����struct
	KWT_FUNCTION_DEFINE,		//��������function
	KWT_STATEMENT_IF,			//������if
	KWT_STATEMENT_ELSE,			//else
	KWT_STATEMENT_WHILE,		//ѭ�����while
	KWT_OPERATOR_NEW,			//new
	KWT_OPERATOR_DELETE,		//delete
	KWT_FILE_INCLUDE,			//����include
	KWT_MACRO_DEFINE,			//�궨��
	KWT_LINE_COMMENTS,			//����ע��//
	KWT_SEGMENT_COMMENTS_START,	//��ע��/*
	KWT_SEGMENT_COMMENTS_END,	//��ע�ͽ���*/
	KWT_CONST_NULL,				//����null
	KWT_CONST_TRUE,				//����true
	KWT_CONST_FALSE,			//����false
	KWT_RETURN_VALUE,			//����ֵ return
	KWT_NOTEQUAL,				//������
	KWT_BREAK,					//break;
	KWT_CONTINUE,				//continue;
	KWT_STATEMENT_FOR,			//forѭ��

	KWT_STATEMENT_SWITCH,		//switch����ѡ�����
	KWT_STATEMENT_CASE,			//case
	KWT_STATEMENT_DEFAULT,		//default
	KWT_STATEMENT_CHILD_SPLIT,	//:�Ӿ�ָ�

	KWT_CONST_CR,				//�س�//CR \r 0x0d
	KWT_CONST_LF,				//����//LF \n 0x0a
	KWT_CONST_TAB,				//�Ʊ�//TAB \t 0x09

	KWT_CLASS_ARRAY,			//�������array
	KWT_CLASS_LIST,				//������� list
	KWT_CLASS_MAP,				//MAP����
	KWT_CLASS_QUEUE,			//���ж���

}KEYWORDS_TYPE;
typedef struct LBS_EXPORT _KEYWORDSSTRUCT
{
	KEYWORDS_TYPE		KeywordsType;
	CHAR				Keywords[KEY_WORDS_MAX_LEN];
	DWORD				KeywordsClass;

	DWORD				keywords_len;
public:
	DWORD				GetKeywordsLen()
	{
		if(keywords_len == 0)
			keywords_len =(DWORD)strlen(Keywords);
		return keywords_len;
	}
	BOOL		IsMatched(LPSTR IN pCode,BOOL IN bAbsolute)
	{
		if(pCode == NULL)
			return FALSE;
		if(pCode[0] != Keywords[0])
			return FALSE;
		if(bAbsolute)
		{
			if(strcmp(pCode,Keywords)==0)
				return TRUE;
		}
		else
		{
			if(strncmp(pCode,Keywords,GetKeywordsLen())==0)
				return TRUE;
		}
		return FALSE;
	}
	BOOL		IsKeywords()
	{
		return  ( (KWC_KEYWORDS & KeywordsClass) != 0);
	}
	BOOL		IsSeperator()
	{
		return  ( (KWC_SEPERATOR & KeywordsClass) != 0);
	}

}KEYWORDSSTRUCT,*PKEYWORDSSTRUCT;

class LBS_EXPORT CLBS_CodeBase;
class LBS_EXPORT CLBS_SyntaxParser;
class LBS_EXPORT CLBS_ScriptFile :
	public CLBS_ObjBase
{
protected:
	CLBS_SyntaxParser*		m_pScriptParser;
	LPSTR					m_pFileName;		//�ű��ļ���
	LPSTR					m_pFileDirectory;	//�ű��ļ�����Ŀ¼
	LPSTR					m_pContents;		//�ű�����
	DWORD					m_dwTotalLine;		//�ű��ļ��ܹ�������
	LPSTR					m_pLineStartPos;	//��ǰ�����е�ָ��
	DWORD					m_dwCurrentLine;	//��ǰ������һ��

private:
	DWORD					m_dwSavedLineNo;	//��ʱ������к�

public:
	CLBS_ScriptFile(CLBS_SyntaxParser* IN pScriptParser);
	virtual ~CLBS_ScriptFile(void);
public:
	LPCSTR					GetFileName(){return m_pFileName;}
	LPCSTR					GetFileDirectory(){return m_pFileDirectory;}

	DWORD					GetCurrentLineNum(){return m_dwCurrentLine;}
	CLBS_SyntaxParser*		GetParser(){return m_pScriptParser;}

	BOOL					Load(LPCSTR IN pFileName);
	BOOL					LoadFromMemory(LPCSTR IN pMemFileName,VOID* IN pMem,DWORD IN dwLen);

	BOOL					ParseScript(CLBS_CodeBase* IN pOwner,LPCSTR IN pScriptContents);
	VOID					Clear();
public:
	LPSTR					GetNextSeperatorPosition(LPSTR IN pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT& IN OUT pKeywords);
	LPSTR					GetNextValidKeywordsPosition(LPSTR IN pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT& IN OUT pKeywords);
	LPSTR					GetNextKeywordsPosition(LPSTR IN pPos,LPSTR IN pEnd,KEYWORDS_TYPE IN keywordsType);
	LPSTR					SeekNextWords(LPSTR IN pPos,LPSTR IN pEnd,LPSTR& OUT pWordsEnd);
	BOOL					GetNextWords(LPSTR& IN OUT pPos,LPSTR IN pEnd,LPSTR& OUT pWordsStart,CHAR OUT Word[KEY_WORDS_MAX_LEN]);

	LPSTR					SeekValid(LPSTR IN pPos,LPSTR IN pEnd);
	VOID					RestoreLineNo(){m_dwCurrentLine=m_dwSavedLineNo;}
	VOID					SaveLineNo(){m_dwSavedLineNo=m_dwCurrentLine;}
public:
	virtual VOID			ShowDebugTrace(LPCSTR IN pszFormat,...);
protected:
	VOID					LineNoCounter(LPSTR IN pPos);
protected:
	BOOL					ParseNoKeywords(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd,LPSTR IN pWordsStart,LPSTR IN pWords);
	BOOL					ParseByKeywords(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT IN pKeywords);
	BOOL					ParsePreprocessCmd(CLBS_CodeBase* IN pOwner,LPCSTR IN pCmd,LPSTR& IN OUT pPos,LPSTR IN pEnd);
	BOOL					ParseVariable(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd);
	BOOL					ParseStatementCalculate(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd);
	BOOL					ParseFunction(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd);
};
