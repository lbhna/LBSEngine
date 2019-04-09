#pragma once
/********************************************************************
	created:	2012-5-31   13:59
	filename: 	d:\Libo\projects\LBSEngine\LBSEngine\Script\LBS_ApiImplement.h
	author:		LB
	
	purpose:	�ű�ϵͳAPIʵ��
*********************************************************************/
#include <math.h>


#include "../Base/LBS_ObjListBase.h"


class CLBS_ApiImplement
{
public:
	CLBS_ApiImplement(void);
	~CLBS_ApiImplement(void);
public:
	//LBS_API_FUNCTION
	//�õ�������Ա����
	static CLBS_ObjBase*	var_length(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ������ֽ�����
	static CLBS_ObjBase*	var_bytes(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	var_getByte(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ�ַ���
	static CLBS_ObjBase*	var_toString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ16�����ִ�
	static CLBS_ObjBase*	var_toHexString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ����
	static CLBS_ObjBase*	var_toInteger(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ������
	static CLBS_ObjBase*	var_toFloat(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ�������ڴ�
	static CLBS_ObjBase*	var_toBinary(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��ΪСд�ִ�
	static CLBS_ObjBase*	var_toLowerCase(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ת��Ϊ�����ִ�
	static CLBS_ObjBase*	var_toUpperCase(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ȥ�ո�
	static CLBS_ObjBase*	var_trimSpace(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ��Ӵ�char* subString(int iStart,int iLen);
	static CLBS_ObjBase*	var_subString(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ��Ӵ���һ�γ��ֵ�λ��int indexOf(char* pStr,int iStart=0);
	static CLBS_ObjBase*	var_indexOf(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ��Ӵ����һ�γ��ֵ�λ��int lastIndexOf(char* pStr);
	static CLBS_ObjBase*	var_lastIndexOf(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//��ת�ֽ�
	static CLBS_ObjBase*	var_reverseBytes(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//������ת
	static CLBS_ObjBase*	var_reverse(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	var_removeAt(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	var_getName(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	var_getTypeName(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

	static CLBS_ObjBase*	var_getToken(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	var_reSize(CLBS_ObjListBase& IN paraList,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);


public://���ֺ���
	
	static CLBS_ObjBase*	math_sin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_cos(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_tan(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_asin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_acos(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_atan(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_sqrt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_exp(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_log(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_log10(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_floor(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_ceil(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_fmod(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	math_pow(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ȡ����� int rand(max);
	static CLBS_ObjBase*	math_rand(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//��һ�����ľ���ֵ
	static CLBS_ObjBase*	math_abs(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://�ļ�����
	static CLBS_ObjBase*	file_fopen(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fclose(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fwrite(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_freadString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fwriteString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fseek(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fseekToBegin(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fseekToEnd(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fgetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fsetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	file_fgetPosition(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://Ŀ¼���ļ�
	static CLBS_ObjBase*	explorer_createDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_setCurrentDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_getCurrentDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_deleteDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_deleteFile(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_listDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_isFile(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_isDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	explorer_fileIsExist(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://�ű�ϵͳ
	//�õ��ű�����Ŀ¼
	static CLBS_ObjBase*	script_getScriptRootDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ��ű�������Ӧ�ó�������Ŀ¼
	static CLBS_ObjBase*	script_getScriptLoaderAppDirectory(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//����һ����ʱ��
	static CLBS_ObjBase*	script_createTimer(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//ɱ��һ����ʱ��
	static CLBS_ObjBase*	script_killTimer(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

	static CLBS_ObjBase*	script_execute(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	
public://�����ļ���д
	static CLBS_ObjBase*	ini_getIniString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	ini_getIniInteger(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	ini_getIniFloat(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	ini_setIniString(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	ini_setIniInteger(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	ini_setIniFloat(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://ϵͳ
	//�õ���ǰϵͳʱ��������
	static CLBS_ObjBase*	system_getDate(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ���ǰϵͳʱ��ʱ����
	static CLBS_ObjBase*	system_getTime(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//�õ�ϵͳ������
	static CLBS_ObjBase*	system_getTickCount(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_getCpuTime(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	//���������Ϣ��Ӧ�ó��������
	static CLBS_ObjBase*	system_trace(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_shellExecute(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_executeDosCommand(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_sleep(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);


	static CLBS_ObjBase*	system_openUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_sendToUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_closeUDP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_getUDPPort(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);


	static CLBS_ObjBase*	system_openTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_connectTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_sendToTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_closeTCP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_getTCPIP(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_getTCPPort(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);



	static CLBS_ObjBase*	system_getLocalHostDefaultIp(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	system_getLocalHostIpArray(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

	static CLBS_ObjBase*	system_raisePrivileges(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://���ݽṹ
	static CLBS_ObjBase*	vector_arrayCreate(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayAdd(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayInsert(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayGetAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arraySetAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayRemoveAt(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayGetLength(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayClear(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayDestroy(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	vector_arrayFind(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

	static CLBS_ObjBase*	string_strGetToken(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://�ڴ����
	static CLBS_ObjBase*	memory_memCopy(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	memory_memCopyEx(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);

public://�߳�&����
	static CLBS_ObjBase*	thread_createThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_terminateThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_suspendThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_resumeThread(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	
	static CLBS_ObjBase*	thread_createLock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_lock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_unlock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_destroyLock(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	
	static CLBS_ObjBase*	thread_createEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_setEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_waitEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);
	static CLBS_ObjBase*	thread_destroyEvent(CLBS_ObjListBase& IN para,CLBS_ObjBase* IN pThis,DWORD& OUT errorCode);



};
