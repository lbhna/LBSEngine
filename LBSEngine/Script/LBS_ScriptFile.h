#pragma once
#include "../Base/LBS_ObjBase.h"
#include "../Syntax/LBS_SyntaxParser.h"

#define				KEY_WORDS_MAX_LEN	0x80		//关键字最大长度

#define				KWC_KEYWORDS		0x0001		//是不是关键字
#define				KWC_OPERATOR		0x0002		//有操作符属性的关键字
#define				KWC_PREFIX			0x0004		//有前缀属性的关键字
#define				KWC_SEPERATOR		0x0008		//有分割属性的关键字
#define				KWC_LOGIC_OPERATOR	0x0010		//逻辑运算符的关键字

typedef enum LBS_EXPORT _KEYWORDSTYPE
{
	KWT_ADD=0,					//加+
	KWT_SUB,					//减-
	KWT_MUL,					//乘*
	KWT_DIV,					//除/
	KWT_EQU,					//赋值等=
	KWT_MOD,					//模除%
	KWT_EQUAL,					//比较等==
	KWT_B,						//大于>
	KWT_BE,						//大于等于>=
	KWT_S,						//小于<
	KWT_SE,						//小于等于<=
	KWT_LOGIC_AND,				//逻辑与&&
	KWT_LOGIC_OR,				//逻辑或||
	KWT_LOGIC_NOT,				//逻辑非!
	KWT_AND,					//位与&
	KWT_OR,						//位或|
	KWT_NOT,					//位非~
	KWT_XOR,					//位异或^
	KWT_LMB,					//位左移<<
	KWT_RMB,					//位右移>>
	KWT_SUFFIX_START,			//前取下标符[
	KWT_SUFFIX_END,				//后取下标符]
	KWT_STRING_DOUBLE,			//双引号"
	KWT_STRING_SINGLE,			//单引号'
	KWT_CODE_BLOCK_START,		//代码块开始{
	KWT_CODE_BLOCK_END,			//代码块结束}
	KWT_PARA_LIST_START,		//参数列表开始(
	KWT_PARA_LIST_END,			//参数列表结束)
	KWT_PARA_SEPERATOR,			//参数分割,
	KWT_PRE_PROCESS_PREFIX,		//预处理指令前缀#
	KWT_WORD_SPACE,				//空格符
	KWT_WORD_TABLE,				//制表符\t
	KWT_END_LINE,				//行结束 ;
	KWT_CARRIAGE,				//回车符\r
	KWT_RETURN,					//换行符\n
	KWT_CHILD_AREA,				//子域.
	KWT_ORIGIN_AREA,			//顶级域::
	KWT_VARIABLE_DEFINE,		//变量定义var
	KWT_CLASS_DEFINE,			//类定义class
	KWT_STRUCT_DEFINE,			//结构定义struct
	KWT_FUNCTION_DEFINE,		//函数定义function
	KWT_STATEMENT_IF,			//如果语句if
	KWT_STATEMENT_ELSE,			//else
	KWT_STATEMENT_WHILE,		//循环语句while
	KWT_OPERATOR_NEW,			//new
	KWT_OPERATOR_DELETE,		//delete
	KWT_FILE_INCLUDE,			//包含include
	KWT_MACRO_DEFINE,			//宏定义
	KWT_LINE_COMMENTS,			//单行注释//
	KWT_SEGMENT_COMMENTS_START,	//段注释/*
	KWT_SEGMENT_COMMENTS_END,	//段注释结束*/
	KWT_CONST_NULL,				//常量null
	KWT_CONST_TRUE,				//常量true
	KWT_CONST_FALSE,			//常量false
	KWT_RETURN_VALUE,			//返回值 return
	KWT_NOTEQUAL,				//不等于
	KWT_BREAK,					//break;
	KWT_CONTINUE,				//continue;
	KWT_STATEMENT_FOR,			//for循环

	KWT_STATEMENT_SWITCH,		//switch条件选择语句
	KWT_STATEMENT_CASE,			//case
	KWT_STATEMENT_DEFAULT,		//default
	KWT_STATEMENT_CHILD_SPLIT,	//:子句分割

	KWT_CONST_CR,				//回车//CR \r 0x0d
	KWT_CONST_LF,				//换行//LF \n 0x0a
	KWT_CONST_TAB,				//制表//TAB \t 0x09

	KWT_CLASS_ARRAY,			//数组对象array
	KWT_CLASS_LIST,				//链表对象 list
	KWT_CLASS_MAP,				//MAP对象
	KWT_CLASS_QUEUE,			//队列对象

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
	LPSTR					m_pFileName;		//脚本文件名
	LPSTR					m_pFileDirectory;	//脚本文件所在目录
	LPSTR					m_pContents;		//脚本内容
	DWORD					m_dwTotalLine;		//脚本文件总共多少行
	LPSTR					m_pLineStartPos;	//当前处理行的指针
	DWORD					m_dwCurrentLine;	//当前处理到哪一行

private:
	DWORD					m_dwSavedLineNo;	//临时保存的行号

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
