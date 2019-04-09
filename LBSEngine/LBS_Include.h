#pragma  once

#include "./Script/LBS_ScriptDriver.h"
#include "./Script/LBS_Script_ClassBase.h"
#include "./Script/LBS_CodeEdit.h"

#ifdef _DEBUG
	#pragma  comment(lib,"LBSEngine_d.lib")
#else
	#pragma  comment(lib,"LBSEngine.lib")
#endif