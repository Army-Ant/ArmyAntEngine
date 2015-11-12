@echo off
:: Created by Jason 11/12/2015
:: This is a batch command script file used to do something before build in Visual Studio project
:: Please edit it in notepad++ or another text editor, instead of visual studio editor

:CmdStart
set ConfigType=%1
set ProjectPath=%2
set TargetName=%3

:CopyArmyAntLibFiles
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\*.h  %ProjectPath%..\..\externals\ArmyAntLib\
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\include\*.h  %ProjectPath%..\..\externals\ArmyAntLib\include\
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\lib\*.lib  %ProjectPath%..\..\externals\ArmyAntLib\lib\
copy %ProjectPath%..\..\..\ArmyAntLib\ArmyAntLib\bin\*.dll  %ProjectPath%..\..\externals\ArmyAntLib\bin\

:End
@echo on