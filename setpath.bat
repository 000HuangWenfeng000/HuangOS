@echo off

if DEFINED MINGW64 goto end

set MINGW64=C:\Softwares\Developments\MSYS2\mingw64
set QEMU=C:\Softwares\Developments\qemu
set MSYS2_USR=C:\Softwares\Developments\MSYS2\usr
set BOCHS=C:\Softwares\Developments\Bochs-2.6
set PATH=%PATH%;%MINGW64%\bin;%BOCHS%;%MSYS2_USR%\bin;%QEMU%;

:end
