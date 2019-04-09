#include "StdAfx.h"
#include ".\lbs_scriptfile.h"
#include "./LBS_ScriptMacroDefine.h"
#include "./LBS_Variable.h"
#include "./LBS_Statement_Calculate.h"
#include "./LBS_Function.h"
#include "./LBS_Script_ClassBase.h"

//===========================================================================================


BOOL				gm_bKeywordsArrayIsOrder=FALSE;
PKEYWORDSSTRUCT		gm_pOrderKeywordsArray	=NULL;
KEYWORDSSTRUCT		gm_theKeywordsArray[]	=
{
	{KWT_ADD					, "+"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_SUB					, "-"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_MUL					, "*"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_DIV					, "/"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_EQU					, "="		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_MOD					, "%"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},

	{KWT_EQUAL					,"=="		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_B						,">"		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_BE						,">="		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_S						,"<"		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_SE						,"<="		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_LOGIC_AND				,"&&"		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_LOGIC_OR				,"||"		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_LOGIC_NOT				,"!"		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},

	{KWT_AND					,"&"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_OR						,"|"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_NOT					,"~"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_XOR					,"^"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_LMB					,"<<"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},
	{KWT_RMB					,">>"		,KWC_KEYWORDS|KWC_OPERATOR|KWC_SEPERATOR , 0},

	{KWT_SUFFIX_START			,"["		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_SUFFIX_END				,"]"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_STRING_DOUBLE			,"\""		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_STRING_SINGLE			,"'"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_CODE_BLOCK_START		,"{"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_CODE_BLOCK_END			,"}"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_PARA_LIST_START		,"("		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_PARA_LIST_END			,")"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_PARA_SEPERATOR			,","		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_PRE_PROCESS_PREFIX		,"#"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_WORD_SPACE				," "		,KWC_SEPERATOR , 0},				
	{KWT_WORD_TABLE				,"\t"		,KWC_SEPERATOR , 0},				
	{KWT_END_LINE				,";"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},				
	{KWT_CARRIAGE				,"\r"		,KWC_SEPERATOR , 0},				
	{KWT_RETURN					,"\n"		,KWC_SEPERATOR , 0},				
	{KWT_CHILD_AREA				,"."		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_ORIGIN_AREA			,"::"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_VARIABLE_DEFINE		,"var"		,KWC_KEYWORDS , 0},
	{KWT_CLASS_DEFINE			,"class"	,KWC_KEYWORDS , 0},
	{KWT_STRUCT_DEFINE			,"struct"	,KWC_KEYWORDS , 0},
	{KWT_FUNCTION_DEFINE		,"function"	,KWC_KEYWORDS , 0},
	{KWT_STATEMENT_IF			,"if"		,KWC_KEYWORDS , 0},
	{KWT_STATEMENT_ELSE			,"else"		,KWC_KEYWORDS , 0},
	{KWT_STATEMENT_WHILE		,"while"	,KWC_KEYWORDS , 0},
	{KWT_OPERATOR_NEW			,"new"		,KWC_KEYWORDS|KWC_OPERATOR , 0},
	{KWT_OPERATOR_DELETE		,"delete"	,KWC_KEYWORDS|KWC_OPERATOR , 0},
	{KWT_FILE_INCLUDE			,"include"	,KWC_KEYWORDS , 0},
	{KWT_MACRO_DEFINE			,"define"	,KWC_KEYWORDS , 0},
	{KWT_LINE_COMMENTS			,"//"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_SEGMENT_COMMENTS_START	,"/*"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_SEGMENT_COMMENTS_END	,"*/"		,KWC_KEYWORDS|KWC_SEPERATOR , 0},
	{KWT_CONST_NULL				,"null"		,KWC_KEYWORDS , 0},
	{KWT_CONST_TRUE				,"true"		,KWC_KEYWORDS , 0},
	{KWT_CONST_FALSE			,"false"	,KWC_KEYWORDS , 0},
	{KWT_RETURN_VALUE			,"return"	,KWC_KEYWORDS , 0},

	{KWT_NOTEQUAL				,"!="		,KWC_KEYWORDS|KWC_SEPERATOR|KWC_LOGIC_OPERATOR , 0},
	{KWT_BREAK					,"break"	,KWC_KEYWORDS, 0},
	{KWT_CONTINUE				,"continue"	,KWC_KEYWORDS, 0},
	{KWT_STATEMENT_FOR			,"for"		,KWC_KEYWORDS, 0},

	{KWT_STATEMENT_SWITCH		,"switch"	,KWC_KEYWORDS, 0},
	{KWT_STATEMENT_CASE			,"case"		,KWC_KEYWORDS, 0},
	{KWT_STATEMENT_DEFAULT		,"default"	,KWC_KEYWORDS, 0},
	{KWT_STATEMENT_CHILD_SPLIT	,":"		,KWC_KEYWORDS|KWC_SEPERATOR, 0},

	{KWT_CONST_CR				,"CR"		,KWC_KEYWORDS, 0},	
	{KWT_CONST_LF				,"LF"		,KWC_KEYWORDS, 0},	
	{KWT_CONST_TAB				,"TAB"		,KWC_KEYWORDS, 0},	

	{KWT_CLASS_ARRAY			,"array"	,KWC_KEYWORDS, 0},	
	{KWT_CLASS_LIST				,"list"		,KWC_KEYWORDS, 0},	
	{KWT_CLASS_MAP				,"map"		,KWC_KEYWORDS, 0},	
	{KWT_CLASS_QUEUE			,"queue"	,KWC_KEYWORDS, 0},	

};


VOID				OrderKeywords()
{
	KEYWORDSSTRUCT	tmp;
	for(LONG i=0;i<(LONG)GetKeywordsCount();i++)
	{
		for(LONG j = i-1 ; j >= 0 ; j-- )
		{
			if(gm_theKeywordsArray[j+1].KeywordsType < gm_theKeywordsArray[j].KeywordsType)
			{
				tmp							=gm_theKeywordsArray[j];
				gm_theKeywordsArray[j]		=gm_theKeywordsArray[j+1];
				gm_theKeywordsArray[j+1]	=tmp;
			}
		}
	}
}
DWORD				GetKeywordsCount()
{
	return sizeof(gm_theKeywordsArray) / sizeof(KEYWORDSSTRUCT);
}

PKEYWORDSSTRUCT		GetKeywords(DWORD IN dwIndex)
{
	if(!gm_bKeywordsArrayIsOrder)
	{
		gm_bKeywordsArrayIsOrder=TRUE;
		OrderKeywords();
	}
	return &gm_theKeywordsArray[dwIndex];
}
PKEYWORDSSTRUCT		GetOrderKeywordsArray()
{//得到按名字长度排序好的关键字数组
	if(gm_pOrderKeywordsArray == NULL)
	{
		DWORD	size	=(GetKeywordsCount()+1) * sizeof(KEYWORDSSTRUCT);
		gm_pOrderKeywordsArray	=(PKEYWORDSSTRUCT)LB_ALLOC( size );
		ZeroMemory(gm_pOrderKeywordsArray,size);

		KEYWORDSSTRUCT	tmp;
		for(LONG i=0;i<(LONG)GetKeywordsCount();i++)
		{
			gm_pOrderKeywordsArray[i]	=gm_theKeywordsArray[i];
			for(LONG j = i-1 ; j >= 0 ; j-- )
			{
				if(gm_pOrderKeywordsArray[j+1].GetKeywordsLen() > gm_pOrderKeywordsArray[j].GetKeywordsLen())
				{
					tmp							=gm_pOrderKeywordsArray[j];
					gm_pOrderKeywordsArray[j]	=gm_pOrderKeywordsArray[j+1];
					gm_pOrderKeywordsArray[j+1]	=tmp;
				}
			}
		}

	}
	return gm_pOrderKeywordsArray;
}
BOOL		IsMatchKeywords(LPSTR IN pCode,PKEYWORDSSTRUCT& OUT pKeywords,BOOL IN bAbsolute)
{
	PKEYWORDSSTRUCT	pKeywordsArray	=GetOrderKeywordsArray();
	DWORD			KeywordsCount	=GetKeywordsCount();
	for(DWORD i=0;i<KeywordsCount;i++)
	{
		if(pKeywordsArray[i].IsMatched(pCode,bAbsolute))
		{
			pKeywords	=&pKeywordsArray[i];
			return TRUE;
		}
	}
	return FALSE;
}
BOOL		IsKeywords(LPCSTR IN pCmd,KEYWORDS_TYPE IN keywordsType)
{
	if(!gm_bKeywordsArrayIsOrder)
	{
		gm_bKeywordsArrayIsOrder=TRUE;
		OrderKeywords();
	}
	return (strcmp(gm_theKeywordsArray[(LONG)keywordsType].Keywords,pCmd)==0);
}



//===========================================================================================


CLBS_ScriptFile::CLBS_ScriptFile(CLBS_SyntaxParser* IN pScriptParser)
{
	m_Type				=LOT_SCRIPT_FILE;
	m_pScriptParser		=pScriptParser;

	m_pFileName			=NULL;
	m_pFileDirectory	=NULL;
	m_pContents			=NULL;
	m_dwTotalLine		=0;
	m_dwCurrentLine		=0;
	m_dwSavedLineNo		=0;
	m_pLineStartPos		=NULL;

}

CLBS_ScriptFile::~CLBS_ScriptFile(void)
{
	Clear();
}
VOID			CLBS_ScriptFile::Clear()
{
	LB_FREE((LPVoid&)m_pFileName);
	LB_FREE((LPVoid&)m_pFileDirectory);
	LB_FREE((LPVoid&)m_pContents);
	m_pLineStartPos		=NULL;
	m_dwSavedLineNo		=0;
	m_dwCurrentLine		=0;
	m_dwTotalLine		=0;
}
BOOL			CLBS_ScriptFile::Load(LPCSTR IN pFileName)
{
	Clear();
	if(pFileName == NULL)
		return FALSE;
	CFile		tmpFile;
	if(!tmpFile.Open(pFileName,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary))
	{
		ShowDebugTrace("错误：无法打开脚本文件[ %s ]！",pFileName);
		
		return FALSE;
	}
	tmpFile.SeekToBegin();
	DWORD	len		=(DWORD)tmpFile.GetLength();
	m_pContents		=(CHAR*)LB_ALLOC(len+1);
	len =tmpFile.Read(m_pContents,len);
	m_pContents[len]=0;
	tmpFile.Close();

	if(strncmp(m_pContents,ENC_HEADER_FLAG,strlen(ENC_HEADER_FLAG))==0)
	{
		CHAR*	pCode	=m_pScriptParser->GetScriptDriver()->GetDecCode(m_pContents);
		if(pCode == NULL)
			return FALSE;
        LB_FREE((LPVoid&)m_pContents);
		m_pContents =pCode;
	}






	m_dwTotalLine	=1;
	for(DWORD i=0;i<len;i++)
	{
		if( m_pContents[i] == GetKeywords(KWT_RETURN)->Keywords[0] )
			m_dwTotalLine++;
	}

	len				=(DWORD)strlen(pFileName);
	m_pFileName		=(LPSTR)LB_ALLOC(len+1);
	m_pFileDirectory=(LPSTR)LB_ALLOC(len+1);
	ZeroMemory(m_pFileName,len+1);
	ZeroMemory(m_pFileDirectory,len+1);
	strncpy(m_pFileName,pFileName,len);
	strncpy(m_pFileDirectory,pFileName,len);
	
	for(LONG j=len-1;j>=0;j--)
	{
		if(m_pFileDirectory[j]=='\\'||m_pFileDirectory[j]=='/')
		{
			m_pFileDirectory[j]=0;
			break;
		}
	}

	m_dwCurrentLine	=1;
	BOOL bRet =ParseScript(m_pScriptParser,m_pContents);
	if(!bRet)
	{
		CErrorSystem::GetInstance().AddErrorInfo(GetFileName(),GetCurrentLineNum(),
			"#解析脚本失败！文件【 %s 】 行号【 %d 】.",GetFileName(),GetCurrentLineNum());
	}

	return bRet;
}
BOOL			CLBS_ScriptFile::LoadFromMemory(LPCSTR IN pMemFileName,VOID* IN pMem,DWORD IN dwLen)
{
	Clear();
	if(pMemFileName == NULL || pMem==NULL)
		return FALSE;



	if(strncmp((CHAR*)pMem,ENC_HEADER_FLAG,strlen(ENC_HEADER_FLAG))==0)
	{
		m_pContents	=m_pScriptParser->GetScriptDriver()->GetDecCode((CHAR*)pMem);
		if(m_pContents == NULL)
			return FALSE;
	}
	else
	{
		m_pContents		=(CHAR*)LB_ALLOC(dwLen+1);
		memcpy(m_pContents,pMem,dwLen);
		m_pContents[dwLen]=0;
	}


	m_dwTotalLine	=1;
	for(DWORD i=0;i<dwLen;i++)
	{
		if( m_pContents[i] == GetKeywords(KWT_RETURN)->Keywords[0] )
			m_dwTotalLine++;
	}

	DWORD len 		=(DWORD)strlen(pMemFileName);
	m_pFileName		=(LPSTR)LB_ALLOC(len+1);
	ZeroMemory(m_pFileName,len+1);
	strncpy(m_pFileName,pMemFileName,len);

	CHAR	FileName[1024];
	GetModuleFileName(NULL,FileName,sizeof(FileName));
	for(LONG j=strlen(FileName)-1;j>=0;j--)
	{
		if(FileName[j]=='\\'||FileName[j]=='/')
		{
			FileName[j]=0;
			break;
		}
	}
	len 		=(DWORD)strlen(FileName);
	m_pFileDirectory=(LPSTR)LB_ALLOC(len+1);
	ZeroMemory(m_pFileDirectory,len+1);
	strncpy(m_pFileDirectory,FileName,len);



	m_dwCurrentLine	=1;
	BOOL bRet =ParseScript(m_pScriptParser,m_pContents);
	if(!bRet)
	{
		CErrorSystem::GetInstance().AddErrorInfo(GetFileName(),GetCurrentLineNum(),
			"#解析脚本失败！文件【 %s 】 行号【 %d 】.",GetFileName(),GetCurrentLineNum());
	}

	return bRet;

}

BOOL			CLBS_ScriptFile::ParseScript(CLBS_CodeBase* IN pOwner,LPCSTR IN pScriptContents)
{
	if(pScriptContents == NULL)
		return FALSE;

	LPSTR			pParsePos		=(LPSTR)pScriptContents;
	LPSTR			pContentsEnd	=pParsePos + strlen(pScriptContents);
	PKEYWORDSSTRUCT pKeywords		=NULL;
	CHAR			Words[KEY_WORDS_MAX_LEN];
	LPSTR			pWordsStart		=NULL;

	while(GetNextWords(pParsePos,pContentsEnd,pWordsStart,Words))
	{
		if(IsMatchKeywords(Words,pKeywords))
		{
			if(!ParseByKeywords(pOwner,pParsePos,pContentsEnd,pKeywords))
				return FALSE;
		}
		else
		{
			if(!ParseNoKeywords(pOwner,pParsePos,pContentsEnd,pWordsStart,Words))
				return FALSE;
		}
	}
	return TRUE;
}
BOOL			CLBS_ScriptFile::ParseNoKeywords(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd,LPSTR IN pWordsStart,LPSTR IN pWords)
{
	LPSTR				pParsePos	=pPos;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	CHAR				Words[KEY_WORDS_MAX_LEN];
	LPSTR				pStart		=NULL;
	if(!GetNextWords(pParsePos,pEnd,pStart,Words))
		return FALSE;
	LPSTR				pReturnPos	=GetNextKeywordsPosition(pWordsStart,pEnd,KWT_RETURN);

	if(!IsMatchKeywords(Words,pKeywords)||(pReturnPos!=NULL&&pReturnPos<pParsePos))
	{
		CLBS_ScriptMacroDefine* pMacro =(CLBS_ScriptMacroDefine*)m_pScriptParser->GetMacro(pWords);
		if(pMacro != NULL)
		{//如果是宏定义，则解析宏内容
			return ParseScript(pOwner,pMacro->GetValue());
		}
		return FALSE;
	}
	switch(pKeywords->KeywordsType)
	{
	case KWT_EQU:
	case KWT_ADD:
	case KWT_SUB:
	case KWT_SUFFIX_START:
	case KWT_PARA_LIST_START:
	case KWT_CHILD_AREA:
		{//运算表达式结构
			pPos	=pWordsStart;
			return ParseStatementCalculate(pOwner,pPos,pEnd);
		}
		break;
	case KWT_END_LINE:
		{
			CLBS_ScriptMacroDefine* pMacro =(CLBS_ScriptMacroDefine*)m_pScriptParser->GetMacro(pWords);
			if(pMacro != NULL)
			{//如果是宏定义，则解析宏内容
				return ParseScript(pOwner,pMacro->GetValue());
			}
			return FALSE;
		}
		break;
	default:
		{
			ShowDebugTrace("#错误：脚本语法错误[%s].",pWords);
		}
		break;
	}
	return FALSE;
}
BOOL			CLBS_ScriptFile::ParseByKeywords(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT IN pKeywords)
{
	CHAR				cmd[KEY_WORDS_MAX_LEN];
	LONG				len					=0;
	LPSTR				pWordStart;
	switch(pKeywords->KeywordsType)
	{
	case KWT_PRE_PROCESS_PREFIX:
		{
			if(!GetNextWords(pPos,pEnd,pWordStart,cmd))
				return FALSE;

			if(!ParsePreprocessCmd(pOwner,cmd,pPos,pEnd))
			{
				return FALSE;
			}
		}
		break;
	case KWT_LINE_COMMENTS:
		{
			pPos = GetNextKeywordsPosition(pPos,pEnd,KWT_RETURN);
			pPos += GetKeywords(KWT_RETURN)->GetKeywordsLen();
		}
		break;
	case KWT_SEGMENT_COMMENTS_START:
		{
			pPos = GetNextKeywordsPosition(pPos,pEnd,KWT_SEGMENT_COMMENTS_END);
			pPos += GetKeywords(KWT_SEGMENT_COMMENTS_END)->GetKeywordsLen();
		}
		break;
	case KWT_CHILD_AREA:
		{
		}
		break;
	case KWT_ORIGIN_AREA:
		{
		}
		break;
	case KWT_VARIABLE_DEFINE:
		{//变量定义
			if(!ParseVariable(pOwner,pPos,pEnd))
				return FALSE;
		}
		break;
	case KWT_FUNCTION_DEFINE:
		{//函数定义
			return ParseFunction(pOwner,pPos,pEnd);
		}
		break;
	case KWT_END_LINE:
		{
			return TRUE;
		}
		break;
	case KWT_CLASS_DEFINE:
		{//类原型定义
			CLBS_Script_ClassBase*		pNewClassPrototype =new CLBS_Script_ClassBase(NULL,m_pScriptParser);
			if(!pNewClassPrototype->Load(this,pPos,pEnd))
			{
				pNewClassPrototype->Release();
				return FALSE;
			}
			//添加一个类原型
			if(!m_pScriptParser->AddClassPrototype(pNewClassPrototype))
			{
				pNewClassPrototype->Release();
				return FALSE;
			}
			pNewClassPrototype->Release();
		}
		break;
	default:
		{
			ShowDebugTrace("#错误：无法解释的关键字[ %s ]!",pKeywords->Keywords);
			return FALSE;
		}
		break;

	}

	return TRUE;
}
LPSTR			CLBS_ScriptFile::GetNextSeperatorPosition(LPSTR IN pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT& IN OUT pKeywords)
{
	LPSTR			pParsePos		=pPos;
	while(*pParsePos != 0 && pParsePos < pEnd)
	{
		if(IsMatchKeywords(pParsePos,pKeywords,FALSE))
		{
			if(pKeywords->KeywordsType == KWT_RETURN)
				LineNoCounter(pParsePos);
			if(pKeywords->IsSeperator())
			{
				if(pKeywords != NULL)

				return pParsePos;
			}
		}
		pParsePos ++;
	}
	return pParsePos;
}
LPSTR			CLBS_ScriptFile::GetNextKeywordsPosition(LPSTR IN pPos,LPSTR IN pEnd,KEYWORDS_TYPE IN keywordsType)
{
	LPSTR			pParsePos		=pPos;
	PKEYWORDSSTRUCT pKeywords		=NULL;
	LPSTR			pFindPos		=NULL;
	BOOL			bStart			=TRUE;

	while(*pParsePos != 0 && pParsePos < pEnd)
	{
		if(IsMatchKeywords(pParsePos,pKeywords,FALSE))
		{
			if(pKeywords->KeywordsType == KWT_RETURN)
				LineNoCounter(pParsePos);
			if(pKeywords->IsSeperator())
				bStart =TRUE;
			if(pFindPos != NULL)
			{
				if(pKeywords->IsSeperator())
				{
					return pFindPos;
				}
				else
				{
					pFindPos	=NULL;
					bStart		=FALSE;
					pParsePos	++;
					continue;
				}
			}
			else
			{
				if(pKeywords->KeywordsType==keywordsType)
				{
					pFindPos	=pParsePos;
					if(pKeywords->IsSeperator())
						return pParsePos;
					continue;
				}
				else
				{
					bStart =FALSE;
				}
			}

			pParsePos += pKeywords->GetKeywordsLen();
		}
		else
		{
			bStart		=FALSE;
			pFindPos	=NULL;
			pParsePos	++;
		}
	}
	return NULL;
}


LPSTR			CLBS_ScriptFile::GetNextValidKeywordsPosition(LPSTR IN pPos,LPSTR IN pEnd,PKEYWORDSSTRUCT& IN OUT pKeywords)
{
					pKeywords		=NULL;
	PKEYWORDSSTRUCT pKey			=NULL;
	LPSTR			pParsePos		=pPos;
	LPSTR			pFindPos		=NULL;

	BOOL			bStart			=TRUE;

	while(*pParsePos != 0 && pParsePos < pEnd)
	{
		if(IsMatchKeywords(pParsePos,pKey,FALSE))
		{
			if(pKey->KeywordsType == KWT_RETURN)
				LineNoCounter(pParsePos);
			if(pKey->IsSeperator())
				bStart =TRUE;

			if(!bStart)
			{
				pParsePos	++;
				continue;
			}

			if(pFindPos!=NULL)
			{
				if(pKey->IsSeperator())
				{
					return pFindPos;
				}
				else
				{
					pFindPos	=NULL;
					pKeywords	=NULL;
					bStart		=FALSE;
					pParsePos	++;
					continue;
				}
			}
			else
			{
				if(pKey->IsKeywords())
				{
					pFindPos	=	pParsePos;
					pKeywords	=	pKey;
					if(pKey->IsSeperator())
						return pFindPos;
				}
			}
			pParsePos	+=	pKey->GetKeywordsLen();
		}
		else
		{
			pParsePos	++;
			bStart		=FALSE;
			pFindPos	=NULL;
		}
	}
	pKeywords=NULL;
	return NULL;
}
LPSTR			CLBS_ScriptFile::SeekValid(LPSTR IN pPos,LPSTR IN pEnd)
{
	PKEYWORDSSTRUCT	pKeywords;
	LPSTR			pParsePos		=pPos;
	while(*pParsePos != 0 && pParsePos < pEnd)
	{
		if(IsMatchKeywords(pParsePos,pKeywords,FALSE))
		{
			if(pKeywords->KeywordsType == KWT_RETURN)
				LineNoCounter(pParsePos);
			if(pKeywords->KeywordsType == KWT_LINE_COMMENTS)
			{
				pParsePos =GetNextKeywordsPosition(pParsePos,pEnd,KWT_RETURN);
				if(pParsePos == NULL)
					return NULL;
				pParsePos += GetKeywords(KWT_RETURN)->GetKeywordsLen();
				continue;
			}
			if(pKeywords->KeywordsType == KWT_SEGMENT_COMMENTS_START)
			{
				pParsePos =GetNextKeywordsPosition(pParsePos,pEnd,KWT_SEGMENT_COMMENTS_END);
				if(pParsePos == NULL)
					return NULL;
				pParsePos += GetKeywords(KWT_SEGMENT_COMMENTS_END)->GetKeywordsLen();
				continue;
			}
			

			if(!pKeywords->IsKeywords())
			{
				pParsePos ++;
				continue;
			}
		}
		return pParsePos;
	}
	return NULL;
}
LPSTR			CLBS_ScriptFile::SeekNextWords(LPSTR IN pPos,LPSTR IN pEnd,LPSTR& OUT pWordsEnd)
{
	LPSTR		pParsePos	=SeekValid(pPos,pEnd);
	if(pParsePos == NULL)
		return NULL;
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	LPSTR		pSepPos		=GetNextSeperatorPosition(pParsePos,pEnd,pKeywords);
	if(pSepPos == NULL||pKeywords == NULL)
		return NULL;

	if(pParsePos == pSepPos)
	{
		pWordsEnd	=pSepPos + pKeywords->GetKeywordsLen();
	}
	else
	{
		pWordsEnd	=pSepPos;
	}
	if(pKeywords->KeywordsType == KWT_CHILD_AREA)
	{//要判断是不是浮点数
		BOOL IsFloat	=TRUE;
		for(LPSTR p=pParsePos;p<pWordsEnd;p++)
		{
			if(!IsDigit(*p))
			{
				IsFloat =FALSE;
				break;
			}
		}
		if(IsFloat)
		{
			pWordsEnd += pKeywords->GetKeywordsLen();
			pWordsEnd=GetNextSeperatorPosition(pWordsEnd,pEnd,pKeywords);
		}
	}	
	
	return pParsePos;
}
BOOL			CLBS_ScriptFile::GetNextWords(LPSTR& IN OUT pPos,LPSTR IN pEnd,LPSTR& OUT pWordsStart,CHAR OUT Word[KEY_WORDS_MAX_LEN])
{
	LPSTR	pWordEnd;
	pWordsStart	=SeekNextWords(pPos,pEnd,pWordEnd);
	if(pWordsStart		== NULL)
		return FALSE;

	LONG	len		=(LONG)(pWordEnd - pWordsStart);
	if(len <= 0 || len >= KEY_WORDS_MAX_LEN)
	{
		ShowDebugTrace("#错误：关键字超出长度限制！无法解析！");
		return FALSE;
	}

	pPos	=pWordEnd;
	strncpy(Word,pWordsStart,len);
	Word[len]=0;
	return TRUE;
}

BOOL			CLBS_ScriptFile::ParsePreprocessCmd(CLBS_CodeBase* IN pOwner,LPCSTR IN pCmd,LPSTR& IN OUT pPos,LPSTR IN pEnd)
{
	PKEYWORDSSTRUCT		pKeywords	=NULL;
	LPSTR				pParsePos	=pPos;
	LPSTR				pKeywordsPos=NULL;
	LONG				len			=0;
	LPSTR				pValue		=NULL;
	if(IsKeywords(pCmd,KWT_FILE_INCLUDE))
	{
		pKeywordsPos	=GetNextValidKeywordsPosition(pParsePos,pEnd,pKeywords);
		if(pKeywordsPos == NULL)
			return FALSE;
		
		pParsePos		=pKeywordsPos + pKeywords->GetKeywordsLen();
		if(pKeywords->KeywordsType == KWT_STRING_DOUBLE)
		{
			pKeywordsPos	=GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_DOUBLE);
			if(pKeywordsPos == NULL)
			{
				return FALSE;
			}
			pPos	=pKeywordsPos + GetKeywords(KWT_STRING_DOUBLE)->GetKeywordsLen();
		}
		else if(pKeywords->KeywordsType == KWT_STRING_SINGLE)
		{
			pKeywordsPos	=GetNextKeywordsPosition(pParsePos,pEnd,KWT_STRING_SINGLE);
			if(pKeywordsPos == NULL)
			{
				return FALSE;
			}
			pPos	=pKeywordsPos + GetKeywords(KWT_STRING_SINGLE)->GetKeywordsLen();

		}
		else
		{
			return FALSE;
		}
		len		=(LONG)(pKeywordsPos-pParsePos);
		if(len <= 0)
		{
			return FALSE;
		}

		pValue	=(LPSTR)LB_ALLOC(len+1);
		strncpy(pValue,pParsePos,len);pValue[len]=0;

		if(strchr(pValue,':')!=NULL)
		{//文件全路径
			if(m_pScriptParser->AddScriptFile(pValue)==NULL)
			{
                LB_FREE((LPVoid&)pValue);
				return FALSE;
			}
		}
		else
		{//相对路径
			LPSTR	pFileName	=(LPSTR)LB_ALLOC( (INT)(strlen(GetFileDirectory()) + len + 1) );
			sprintf(pFileName,"%s\\%s",GetFileDirectory(),pValue);
			if(m_pScriptParser->AddScriptFile(pFileName)==NULL)
			{
				LB_FREE((LPVoid&)pValue);
				LB_FREE((LPVoid&)pFileName);
				return FALSE;
			}
            LB_FREE((LPVoid&)pFileName);
		}
        LB_FREE((LPVoid&)pValue);


	}
	else if(IsKeywords(pCmd,KWT_MACRO_DEFINE))
	{
		CLBS_ScriptMacroDefine*		pMacro=new CLBS_ScriptMacroDefine(m_pScriptParser);
		if(!pMacro->Load(this,pPos,pEnd))
		{
			pMacro->Release();
			return FALSE;
		}
		//TRACE("Macro: %s = %s\r\n",pMacro->GetName(),pMacro->GetValue());
		m_pScriptParser->AddMacro(pMacro);
		pMacro->Release();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
BOOL		CLBS_ScriptFile::ParseVariable(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd)
{
	CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(pOwner,TRUE);
	if(!pStatement->Load(this,pPos,pEnd))
	{
		pStatement->Release();
		return FALSE;
	}
	pOwner->AddExecuteCode(pStatement);
	pStatement->Release();
	return TRUE;
}
BOOL		CLBS_ScriptFile::ParseStatementCalculate(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd)
{
	CLBS_Statement_Calculate*	pStatement =new CLBS_Statement_Calculate(pOwner);
	if(!pStatement->Load(this,pPos,pEnd))
	{
		pStatement->Release();
		return FALSE;
	}
	pOwner->AddExecuteCode(pStatement);
	pStatement->Release();
	return TRUE;
}
BOOL		CLBS_ScriptFile::ParseFunction(CLBS_CodeBase* IN pOwner,LPSTR& IN OUT pPos,LPSTR IN pEnd)
{
	CLBS_Function*	pFunction =new CLBS_Function(pOwner);
	if(!pFunction->Load(this,pPos,pEnd))
	{
		pFunction->Release();
		return FALSE;
	}
	BOOL bRet =pOwner->AddMemberFunction(pFunction);
	pFunction->Release();
	return bRet;
}
VOID		 CLBS_ScriptFile::LineNoCounter(LPSTR IN pPos)
{
	if(m_pLineStartPos >= pPos)
		return;
	m_dwCurrentLine		++;
	m_pLineStartPos		=pPos;
}
VOID		CLBS_ScriptFile::ShowDebugTrace(LPCSTR IN pszFormat,...)
{
	CString		info;
	va_list argList;
	va_start( argList, pszFormat );
	info.FormatV( pszFormat, argList );
	va_end( argList );

	/////////////////////
	//TRACE("%s\r\n",info);
	/////////////////////
	CErrorSystem::GetInstance().AddErrorInfo(GetFileName(),GetCurrentLineNum(),info);
}

