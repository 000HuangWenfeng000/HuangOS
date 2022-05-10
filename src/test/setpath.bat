@echo off

if DEFINED MINGW32 goto end

set MINGW32=C:\Softwares\Developments\MSYS2\mingw32
set MSYS2_USR=C:\Softwares\Developments\MSYS2\usr
set BOCHS=C:\Softwares\Developments\Bochs-2.7
set PATH=%PATH%;%MINGW32%\bin;%BOCHS%;%MSYS2_USR%\bin;


:end
