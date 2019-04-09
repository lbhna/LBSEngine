#pragma			once





#define			LBS_SafeDelete(p)			{if(p!=NULL){delete p;p=NULL;}}
#define			LBS_SafeDeleteArray(p)		{if(p!=NULL){delete []p;p=NULL;}}
#define			LBS_SafeRelease(p)			{if(p!=NULL){p->Release();p=NULL;}}




typedef			DOUBLE						*PDOUBLE;

   



//-------------------------------------------------------------
#ifdef _LBS_
	#define LBS_EXPORT			__declspec(dllexport)
#else
	#define LBS_EXPORT			__declspec(dllimport)
#endif
//-------------------------------------------------------------





































