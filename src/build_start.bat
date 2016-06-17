@echo off
:: Created by Jason 11/12/2015
:: This is a batch command script file used to do something before build in Visual Studio project
:: Please edit it in notepad++ or another text editor, instead of visual studio editor

:CmdStart
set ConfigType=%1
set ProjectPath=%2
set TargetName=%3

:CopyArmyAntLibFiles
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\inc\*.*  %ProjectPath%..\..\externals\ArmyAntLib\inc\
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\lib\*.lib  %ProjectPath%..\..\externals\ArmyAntLib\lib\
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\bin\*.dll  %ProjectPath%..\..\externals\ArmyAntLib\bin\

:CopyHLSLCodeFiles
copy %ProjectPath%..\..\src\*.hlsl  %ProjectPath%..\..\bin\

:End
@echo on