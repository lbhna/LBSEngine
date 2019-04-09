md ..\LBS_Publish
md ..\LBS_Publish\INCLUDE
md ..\LBS_Publish\INCLUDE\Base
md ..\LBS_Publish\INCLUDE\Syntax
md ..\LBS_Publish\INCLUDE\Script

del ..\LBS_Publish\INCLUDE\*.* /q
del ..\LBS_Publish\INCLUDE\Base\*.* /q
del ..\LBS_Publish\INCLUDE\Syntax\*.* /q
del ..\LBS_Publish\INCLUDE\Script\*.* /q


copy .\LBS_Include.h ..\LBS_Publish\INCLUDE\
copy .\Base\*.h ..\LBS_Publish\INCLUDE\Base\
copy .\Syntax\*.h ..\LBS_Publish\INCLUDE\Syntax\



copy .\Script\LBS_Variable.h ..\LBS_Publish\INCLUDE\Script\
copy .\Script\LBS_ScriptFile.h ..\LBS_Publish\INCLUDE\Script\
copy .\Script\LBS_ScriptDriver.h ..\LBS_Publish\INCLUDE\Script\
copy .\Script\LBS_Script_CallFunctionPara.h ..\LBS_Publish\INCLUDE\Script\
copy .\Script\LBS_Script_ClassBase.h ..\LBS_Publish\INCLUDE\Script\
copy .\Script\LBS_CodeEdit.h ..\LBS_Publish\INCLUDE\Script\


